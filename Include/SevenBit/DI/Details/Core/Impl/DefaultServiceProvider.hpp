#pragma once

#include <memory>
#include <utility>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/DefaultServiceProvider.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"

namespace sb::di::details::core
{
    INLINE DefaultServiceProvider::DefaultServiceProvider(const DefaultServiceProvider &provider)
        : _sharedCore(utils::Check::notNullAndGet(provider._sharedCore)),
          _scoped(_sharedCore->getOptions().strongDestructionOrder)
    {
    }

    INLINE DefaultServiceProvider::DefaultServiceProvider(IServiceProviderData::Ptr core)
        : _sharedCore(utils::Check::notNullAndGet(std::move(core))),
          _scoped(_sharedCore->getOptions().strongDestructionOrder)
    {
        if (_sharedCore->getOptions().prebuildSingletons)
        {
            prebuildSingletons();
        }
    }

    INLINE ServiceProvider::Ptr DefaultServiceProvider::createScope()
    {
        return ServiceProvider::Ptr{new DefaultServiceProvider{*this}};
    }

    INLINE const IServiceInstance &DefaultServiceProvider::getInstance(TypeId serviceTypeId)
    {
        if (auto instance = tryGetInstance(serviceTypeId))
        {
            return *instance;
        }
        throw ServiceNotFoundException{serviceTypeId,
                                       "Service was not registered or was registered as transient service"};
    }

    INLINE const IServiceInstance *DefaultServiceProvider::tryGetInstance(TypeId serviceTypeId)
    {
        if (auto instances = findRegisteredInstances(serviceTypeId))
        {
            return instances->last().get();
        }
        auto descriptors = findDescriptors(serviceTypeId);
        return descriptors ? tryCreateAndRegister(*descriptors) : nullptr;
    }

    INLINE const ServiceInstanceList *DefaultServiceProvider::tryGetInstances(TypeId serviceTypeId)
    {
        auto instances = findRegisteredInstances(serviceTypeId);
        if (!instances)
        {
            auto descriptors = findDescriptors(serviceTypeId);
            return descriptors ? tryCreateAndRegisterAll(*descriptors) : nullptr;
        }
        if (!instances->isSealed())
        {
            auto descriptors = findDescriptors(serviceTypeId);
            return descriptors ? &createRestInstances(*descriptors, *instances, true) : nullptr;
        }
        return instances;
    }

    INLINE IServiceInstance::Ptr DefaultServiceProvider::createInstance(TypeId serviceTypeId)
    {
        if (auto instance = tryCreateInstance(serviceTypeId))
        {
            return instance;
        }
        throw ServiceNotFoundException{serviceTypeId,
                                       "Service was not registered or was registered as singleton/scoped service"};
    }

    INLINE IServiceInstance::Ptr DefaultServiceProvider::createInstanceInPlace(TypeId serviceTypeId)
    {
        if (auto descriptors = findDescriptors(serviceTypeId))
        {
            return tryCreate(*descriptors, true);
        }
        throw ServiceNotFoundException{serviceTypeId,
                                       "Service was not registered or was registered as singleton/scoped service"};
    }

    INLINE IServiceInstance::Ptr DefaultServiceProvider::tryCreateInstance(TypeId serviceTypeId)
    {
        auto descriptors = findDescriptors(serviceTypeId);
        return descriptors ? tryCreate(*descriptors, false) : nullptr;
    }

    INLINE std::optional<ServiceInstanceList> DefaultServiceProvider::tryCreateInstances(TypeId serviceTypeId)
    {
        auto descriptors = findDescriptors(serviceTypeId);
        return descriptors ? tryCreateAll(*descriptors) : std::nullopt;
    }

    INLINE void DefaultServiceProvider::clear() { _scoped.clear(); }

    INLINE const IServiceInstance *DefaultServiceProvider::tryCreateAndRegister(
        const containers::ServiceDescriptorList &descriptors)
    {
        if (auto instancesMap = tryGetInstancesMap(descriptors.getLifeTime()))
        {
            auto &instances =
                instancesMap->insert(descriptors.getServiceTypeId(), createInstance(descriptors.last(), true));
            if (descriptors.size() == 1)
            {
                instances.seal();
            }
            return instances.last().get();
        }
        return nullptr;
    }

    INLINE const ServiceInstanceList *DefaultServiceProvider::tryCreateAndRegisterAll(
        const containers::ServiceDescriptorList &descriptors)
    {
        if (auto instancesMap = tryGetInstancesMap(descriptors.getLifeTime()))
        {
            auto &instances =
                instancesMap->insert(descriptors.getServiceTypeId(), createInstance(descriptors.last(), true));
            return &createRestInstances(descriptors, instances, true);
        }
        return nullptr;
    }

    INLINE ServiceInstanceList &DefaultServiceProvider::createRestInstances(
        const containers::ServiceDescriptorList &descriptors, ServiceInstanceList &instances, bool inPlace)
    {
        if (descriptors.size() > 1)
        {
            instances.reserve(descriptors.size());
            auto realFirst = createInstance(descriptors.first(), inPlace);
            auto end = --descriptors.end();
            for (auto it = ++descriptors.begin(); it != end; ++it) // skip first and last
            {
                instances.add(createInstance(*it, inPlace));
            }
            instances.add(std::move(realFirst));
            instances.first().swap(instances.last());
        }
        instances.seal();
        return instances;
    }

    INLINE IServiceInstance::Ptr DefaultServiceProvider::tryCreate(const containers::ServiceDescriptorList &descriptors,
                                                                   bool inPlace)
    {
        auto &descriptor = descriptors.last();
        if (descriptor.getLifeTime().isTransient() &&
            (!inPlace || descriptor.getImplementationTypeId() == descriptor.getServiceTypeId()))
        {
            return createInstance(descriptors.last(), inPlace);
        }
        return nullptr;
    }

    INLINE std::optional<ServiceInstanceList> DefaultServiceProvider::tryCreateAll(
        const containers::ServiceDescriptorList &descriptors)
    {
        if (descriptors.getLifeTime().isTransient())
        {
            ServiceInstanceList instances{createInstance(descriptors.last(), false)};
            createRestInstances(descriptors, instances, false);
            return std::move(instances);
        }
        return std::nullopt;
    }

    INLINE IServiceInstance::Ptr DefaultServiceProvider::createInstance(const ServiceDescriptor &descriptor,
                                                                        bool inPlace)
    {
        auto _ = _guard(descriptor.getImplementationTypeId());
        auto instance = descriptor.getImplementationFactory().createInstance(*this, inPlace);
        if (instance && instance->isValid())
        {
            return instance;
        }
        throw InvalidServiceException{descriptor.getImplementationTypeId(), "Service instance is null or is invalid"};
    }

    INLINE const ServiceProviderOptions &DefaultServiceProvider::getOptions() { return _sharedCore->getOptions(); }

    INLINE ServiceInstanceList *DefaultServiceProvider::findRegisteredInstances(TypeId serviceTypeId)
    {
        auto singletons = _sharedCore->getSingletons().findServices(serviceTypeId);
        return singletons ? singletons : _scoped.findServices(serviceTypeId);
    }

    INLINE containers::ServiceInstancesMap *DefaultServiceProvider::tryGetInstancesMap(const ServiceLifeTime &lifeTime)
    {
        return lifeTime.isTransient() ? nullptr : (lifeTime.isSingleton() ? &_sharedCore->getSingletons() : &_scoped);
    }

    INLINE const containers::ServiceDescriptorList *DefaultServiceProvider::findDescriptors(TypeId serviceTypeId) const
    {
        return _sharedCore->getDescriptorsMap().findDescriptors(serviceTypeId);
    }

    INLINE void DefaultServiceProvider::prebuildSingletons()
    {
        for (auto &[_, descriptors] : _sharedCore->getDescriptorsMap())
        {
            if (descriptors.getLifeTime().isSingleton())
            {
                tryCreateAndRegisterAll(descriptors);
            }
        }
    }
} // namespace sb::di::details::core
