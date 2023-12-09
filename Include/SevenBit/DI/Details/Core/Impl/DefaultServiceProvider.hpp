#pragma once

#include <memory>
#include <utility>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/DefaultServiceProvider.hpp"
#include "SevenBit/DI/Details/Utils/Check.hpp"
#include "SevenBit/DI/Details/Utils/Require.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"

namespace sb::di::details::core
{
    INLINE DefaultServiceProvider::DefaultServiceProvider(const DefaultServiceProvider &provider)
        : _sharedData(utils::Require::notNullAndGet(IServiceProviderData::SPtr{provider._sharedData})),
          _scoped(_sharedData->getOptions().strongDestructionOrder)
    {
    }

    INLINE DefaultServiceProvider::DefaultServiceProvider(IServiceProviderData::Ptr core)
        : _sharedData(utils::Require::notNullAndGet(std::move(core))),
          _scoped(_sharedData->getOptions().strongDestructionOrder)
    {
        if (_sharedData->getOptions().prebuildSingletons)
        {
            prebuildSingletons();
        }
    }

    INLINE ServiceProvider::Ptr DefaultServiceProvider::createScope()
    {
        return ServiceProvider::Ptr{new DefaultServiceProvider{*this}};
    }

    INLINE const IServiceInstance &DefaultServiceProvider::getInstance(const TypeId serviceTypeId)
    {
        if (const auto instance = tryGetInstance(serviceTypeId); utils::Check::instanceValidity(instance))
        {
            return *instance;
        }
        throw ServiceNotFoundException{serviceTypeId,
                                       "Service was not registered or was registered as transient instance"};
    }

    INLINE const IServiceInstance *DefaultServiceProvider::tryGetInstance(const TypeId serviceTypeId)
    {
        if (const auto instances = findRegisteredInstances(serviceTypeId))
        {
            return instances->last().get();
        }
        const auto descriptors = findDescriptors(serviceTypeId);
        return descriptors ? tryCreateAndRegister(*descriptors) : nullptr;
    }

    INLINE const OneOrList<IServiceInstance::Ptr> *DefaultServiceProvider::tryGetInstances(TypeId serviceTypeId)
    {
        const auto instances = findRegisteredInstances(serviceTypeId);
        if (!instances)
        {
            const auto descriptors = findDescriptors(serviceTypeId);
            return descriptors ? tryCreateAndRegisterAll(*descriptors) : nullptr;
        }
        if (!instances->isSealed())
        {
            const auto descriptors = findDescriptors(serviceTypeId);
            return descriptors ? &createRestInstances(*descriptors, *instances, true) : nullptr;
        }
        return &instances->getInnerList();
    }

    INLINE IServiceInstance::Ptr DefaultServiceProvider::createInstance(const TypeId serviceTypeId)
    {
        if (auto instance = tryCreateInstance(serviceTypeId); utils::Check::instanceValidity(instance))
        {
            return instance;
        }
        throw ServiceNotFoundException{serviceTypeId,
                                       "Service was not registered or was registered as singleton/scoped instance"};
    }

    INLINE IServiceInstance::Ptr DefaultServiceProvider::tryCreateInstance(const TypeId serviceTypeId)
    {
        const auto descriptors = findDescriptors(serviceTypeId);
        return descriptors ? tryCreate(descriptors->last(), false) : nullptr;
    }

    INLINE std::optional<OneOrList<IServiceInstance::Ptr>> DefaultServiceProvider::tryCreateInstances(
        TypeId serviceTypeId)
    {
        const auto descriptors = findDescriptors(serviceTypeId);
        return descriptors ? tryCreateAll(*descriptors) : std::nullopt;
    }

    INLINE IServiceInstance::Ptr DefaultServiceProvider::createInstanceInPlace(TypeId serviceTypeId)
    {
        if (auto instance = tryCreateInstanceInPlace(serviceTypeId); utils::Check::instanceValidity(instance))
        {
            return instance;
        }
        throw ServiceNotFoundException{serviceTypeId,
                                       "Service was not registered, typeid does not match implementationId or was "
                                       "registered as singleton/scoped instance"};
    }

    INLINE IServiceInstance::Ptr DefaultServiceProvider::tryCreateInstanceInPlace(TypeId serviceTypeId)
    {
        if (const auto descriptors = findDescriptors(serviceTypeId);
            descriptors && descriptors->last().getImplementationTypeId() == serviceTypeId)
        {
            return tryCreate(descriptors->last(), true);
        }
        return nullptr;
    }

    INLINE void DefaultServiceProvider::clear() { _scoped.clear(); }

    INLINE const IServiceInstance *DefaultServiceProvider::tryCreateAndRegister(
        const containers::ServiceDescriptorList &descriptors)
    {
        if (const auto instancesMap = tryGetInstancesMap(descriptors.getLifeTime()))
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

    INLINE const OneOrList<IServiceInstance::Ptr> *DefaultServiceProvider::tryCreateAndRegisterAll(
        const containers::ServiceDescriptorList &descriptors)
    {
        if (const auto instancesMap = tryGetInstancesMap(descriptors.getLifeTime()))
        {
            auto &instances =
                instancesMap->insert(descriptors.getServiceTypeId(), createInstance(descriptors.last(), true));
            return &createRestInstances(descriptors, instances, true);
        }
        return nullptr;
    }

    INLINE OneOrList<IServiceInstance::Ptr> &DefaultServiceProvider::createRestInstances(
        const containers::ServiceDescriptorList &descriptors, containers::ServiceInstanceList &instances,
        const bool inPlaceRequest)
    {
        if (descriptors.size() > 1)
        {
            instances.reserve(descriptors.size());
            auto realFirst = createInstance(descriptors.first(), inPlaceRequest);
            const auto end = --descriptors.end();
            for (auto it = ++descriptors.begin(); it != end; ++it) // skip first and last
            {
                instances.add(createInstance(*it, inPlaceRequest));
            }
            instances.add(std::move(realFirst));
            instances.first().swap(instances.last());
        }
        instances.seal();
        return instances.getInnerList();
    }

    INLINE IServiceInstance::Ptr DefaultServiceProvider::tryCreate(const ServiceDescriptor &descriptor,
                                                                   const bool inPlaceRequest)
    {
        return descriptor.getLifeTime().isTransient() ? createInstance(descriptor, inPlaceRequest) : nullptr;
    }

    INLINE std::optional<OneOrList<IServiceInstance::Ptr>> DefaultServiceProvider::tryCreateAll(
        const containers::ServiceDescriptorList &descriptors)
    {
        if (descriptors.getLifeTime().isTransient())
        {
            containers::ServiceInstanceList instances{createInstance(descriptors.last(), false)};
            createRestInstances(descriptors, instances, false);
            return std::move(instances.getInnerList());
        }
        return std::nullopt;
    }

    INLINE IServiceInstance::Ptr DefaultServiceProvider::createInstance(const ServiceDescriptor &descriptor,
                                                                        const bool inPlaceRequest)
    {
        auto _ = _guard(descriptor.getImplementationTypeId());
        return utils::Require::validInstanceAndGet(
            descriptor.getImplementationFactory().createInstance(*this, inPlaceRequest));
    }

    INLINE const ServiceProviderOptions &DefaultServiceProvider::getOptions() const
    {
        return _sharedData->getOptions();
    }

    INLINE containers::ServiceInstanceList *DefaultServiceProvider::findRegisteredInstances(TypeId serviceTypeId)
    {
        const auto singletons = _sharedData->getSingletons().findServices(serviceTypeId);
        return singletons ? singletons : _scoped.findServices(serviceTypeId);
    }

    INLINE containers::ServiceInstancesMap *DefaultServiceProvider::tryGetInstancesMap(const ServiceLifeTime &lifeTime)
    {
        return lifeTime.isTransient() ? nullptr : (lifeTime.isSingleton() ? &_sharedData->getSingletons() : &_scoped);
    }

    INLINE const containers::ServiceDescriptorList *DefaultServiceProvider::findDescriptors(TypeId serviceTypeId) const
    {
        return _sharedData->getDescriptorsMap().findDescriptors(serviceTypeId);
    }

    INLINE void DefaultServiceProvider::prebuildSingletons()
    {
        for (auto &[_, descriptors] : _sharedData->getDescriptorsMap())
        {
            if (descriptors.getLifeTime().isSingleton())
            {
                tryCreateAndRegisterAll(descriptors);
            }
        }
    }
} // namespace sb::di::details::core
