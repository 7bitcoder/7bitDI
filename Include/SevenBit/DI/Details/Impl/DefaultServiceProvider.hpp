#pragma once

#include <memory>
#include <utility>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/DefaultServiceProvider.hpp"
#include "SevenBit/DI/Details/IServiceProviderRoot.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"

namespace sb::di::details
{
    INLINE DefaultServiceProvider::DefaultServiceProvider(const DefaultServiceProvider &provider)
        : _sharedCore(utils::Assert::ptrAndGet(provider._sharedCore)),
          _scoped(_sharedCore->getOptions().strongDestructionOrder)
    {
    }

    INLINE DefaultServiceProvider::DefaultServiceProvider(IServiceProviderCore::Ptr core)
        : _sharedCore(utils::Assert::ptrAndGet(std::move(core))),
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
        auto instance = tryGetInstance(serviceTypeId);
        return instance ? *instance
                        : throw ServiceNotFoundException{
                              serviceTypeId, "Service was not registered or was registered as transient service"};
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
        auto instance = tryCreateInstance(serviceTypeId);
        return instance
                   ? std::move(instance)
                   : throw ServiceNotFoundException{
                         serviceTypeId, "Service was not registered or was registered as singleton/scoped service"};
    }

    INLINE IServiceInstance::Ptr DefaultServiceProvider::createInstanceInPlace(TypeId serviceTypeId)
    {
        auto descriptors = findDescriptors(serviceTypeId);
        return descriptors
                   ? tryCreate(*descriptors, true)
                   : throw ServiceNotFoundException{
                         serviceTypeId, "Service was not registered or was registered as singleton/scoped service"};
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
        const ServiceDescriptorList &descriptors)
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
        const ServiceDescriptorList &descriptors)
    {
        if (auto instancesMap = tryGetInstancesMap(descriptors.getLifeTime()))
        {
            auto &instances =
                instancesMap->insert(descriptors.getServiceTypeId(), createInstance(descriptors.last(), true));
            return &createRestInstances(descriptors, instances, true);
        }
        return nullptr;
    }

    INLINE ServiceInstanceList &DefaultServiceProvider::createRestInstances(const ServiceDescriptorList &descriptors,
                                                                            ServiceInstanceList &instances,
                                                                            bool inPlace)
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

    INLINE IServiceInstance::Ptr DefaultServiceProvider::tryCreate(const ServiceDescriptorList &descriptors,
                                                                   bool inPlace)
    {
        return descriptors.getLifeTime().isTransient() ? createInstance(descriptors.last(), inPlace) : nullptr;
    }

    INLINE std::optional<ServiceInstanceList> DefaultServiceProvider::tryCreateAll(
        const ServiceDescriptorList &descriptors)
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
        return instance && instance->isValid()
                   ? std::move(instance)
                   : throw InvalidServiceException{descriptor.getImplementationTypeId(),
                                                   "Service instance is null or is invalid"};
    }

    INLINE const ServiceProviderOptions &DefaultServiceProvider::getOptions() { return _sharedCore->getOptions(); }

    INLINE ServiceInstanceList *DefaultServiceProvider::findRegisteredInstances(TypeId serviceTypeId)
    {
        auto singletons = _sharedCore->getSingletons().findServices(serviceTypeId);
        return singletons ? singletons : _scoped.findServices(serviceTypeId);
    }

    INLINE ServiceInstancesMap *DefaultServiceProvider::tryGetInstancesMap(const ServiceLifeTime &lifeTime)
    {
        return lifeTime.isTransient() ? nullptr : (lifeTime.isSingleton() ? &_sharedCore->getSingletons() : &_scoped);
    }

    INLINE const ServiceDescriptorList *DefaultServiceProvider::findDescriptors(TypeId serviceTypeId) const
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
} // namespace sb::di::details
