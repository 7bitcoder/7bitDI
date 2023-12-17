#pragma once

#include <memory>
#include <utility>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceInstanceProvider.hpp"
#include "SevenBit/DI/Details/Services/ExternalService.hpp"
#include "SevenBit/DI/Details/Utils/Check.hpp"
#include "SevenBit/DI/Details/Utils/Require.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"

namespace sb::di::details::core
{
    INLINE ServiceInstanceProvider::ServiceInstanceProvider(const ServiceInstanceProvider &provider)
        : _sharedData(utils::Require::notNullAndGet(IServiceInstanceProviderData::SPtr{provider._sharedData})),
          _scoped(_sharedData->getOptions().strongDestructionOrder)
    {
    }

    INLINE ServiceInstanceProvider::ServiceInstanceProvider(IServiceInstanceProviderData::Ptr data)
        : _sharedData(utils::Require::notNullAndGet(std::move(data))),
          _scoped(_sharedData->getOptions().strongDestructionOrder)
    {
        if (_sharedData->getOptions().prebuildSingletons)
        {
            prebuildSingletons();
        }
    }

    INLINE void ServiceInstanceProvider::setServiceProvider(ServiceProvider &serviceProvider)
    {
        _serviceProvider = &serviceProvider;
        if (const auto list = _scoped.findServices(typeid(ServiceProvider)))
        {
            auto &external = list->first();
            if (!external)
            {
                external = std::make_unique<services::ExternalService<ServiceProvider>>(_serviceProvider);
            }
            dynamic_cast<services::ExternalService<ServiceProvider> &>(*external).update(_serviceProvider);
            list->seal();
        }
        else
        {
            auto external = std::make_unique<services::ExternalService<ServiceProvider>>(_serviceProvider);
            _scoped.insert(typeid(ServiceProvider), std::move(external)).seal();
        }
    }

    INLINE IServiceInstanceProvider::Ptr ServiceInstanceProvider::createScope() const
    {
        return IServiceInstanceProvider::Ptr{new ServiceInstanceProvider{*this}};
    }

    INLINE const IServiceInstance &ServiceInstanceProvider::getInstance(const TypeId serviceTypeId)
    {
        if (const auto instance = tryGetInstance(serviceTypeId); utils::Check::instanceValidity(instance))
        {
            return *instance;
        }
        throw ServiceNotFoundException{serviceTypeId,
                                       "Service was not registered or was registered as transient instance"};
    }

    INLINE const IServiceInstance *ServiceInstanceProvider::tryGetInstance(const TypeId serviceTypeId)
    {
        if (const auto instances = findRegisteredInstances(serviceTypeId))
        {
            return instances->last().get();
        }
        const auto descriptors = findDescriptors(serviceTypeId);
        return descriptors ? tryCreateAndRegister(*descriptors) : nullptr;
    }

    INLINE const OneOrList<IServiceInstance::Ptr> *ServiceInstanceProvider::tryGetInstances(TypeId serviceTypeId)
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

    INLINE IServiceInstance::Ptr ServiceInstanceProvider::createInstance(const TypeId serviceTypeId)
    {
        if (auto instance = tryCreateInstance(serviceTypeId); utils::Check::instanceValidity(instance))
        {
            return instance;
        }
        throw ServiceNotFoundException{serviceTypeId,
                                       "Service was not registered or was registered as singleton/scoped instance"};
    }

    INLINE IServiceInstance::Ptr ServiceInstanceProvider::tryCreateInstance(const TypeId serviceTypeId)
    {
        const auto descriptors = findDescriptors(serviceTypeId);
        return descriptors ? tryCreate(descriptors->last(), false) : nullptr;
    }

    INLINE std::optional<OneOrList<IServiceInstance::Ptr>> ServiceInstanceProvider::tryCreateInstances(
        TypeId serviceTypeId)
    {
        const auto descriptors = findDescriptors(serviceTypeId);
        return descriptors ? tryCreateAll(*descriptors) : std::nullopt;
    }

    INLINE IServiceInstance::Ptr ServiceInstanceProvider::createInstanceInPlace(TypeId serviceTypeId)
    {
        if (auto instance = tryCreateInstanceInPlace(serviceTypeId); utils::Check::instanceValidity(instance))
        {
            return instance;
        }
        throw ServiceNotFoundException{serviceTypeId,
                                       "Service was not registered, typeid does not match implementationId or was "
                                       "registered as singleton/scoped instance"};
    }

    INLINE IServiceInstance::Ptr ServiceInstanceProvider::tryCreateInstanceInPlace(TypeId serviceTypeId)
    {
        if (const auto descriptors = findDescriptors(serviceTypeId);
            descriptors && descriptors->last().getImplementationTypeId() == serviceTypeId)
        {
            return tryCreate(descriptors->last(), true);
        }
        return nullptr;
    }

    INLINE void ServiceInstanceProvider::clear() { _scoped.clear(); }

    INLINE const IServiceInstance *ServiceInstanceProvider::tryCreateAndRegister(
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

    INLINE const OneOrList<IServiceInstance::Ptr> *ServiceInstanceProvider::tryCreateAndRegisterAll(
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

    INLINE OneOrList<IServiceInstance::Ptr> &ServiceInstanceProvider::createRestInstances(
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

    INLINE IServiceInstance::Ptr ServiceInstanceProvider::tryCreate(const ServiceDescriptor &descriptor,
                                                                    const bool inPlaceRequest)
    {
        return descriptor.getLifeTime().isTransient() ? createInstance(descriptor, inPlaceRequest) : nullptr;
    }

    INLINE std::optional<OneOrList<IServiceInstance::Ptr>> ServiceInstanceProvider::tryCreateAll(
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

    INLINE IServiceInstance::Ptr ServiceInstanceProvider::createInstance(const ServiceDescriptor &descriptor,
                                                                         const bool inPlaceRequest)
    {
        auto _ = _guard(descriptor.getImplementationTypeId());
        return utils::Require::validInstanceAndGet(descriptor.getImplementationFactory().createInstance(
            *utils::Require::notNullAndGet(_serviceProvider), inPlaceRequest));
    }

    INLINE const ServiceProviderOptions &ServiceInstanceProvider::getOptions() const
    {
        return _sharedData->getOptions();
    }

    INLINE containers::ServiceInstanceList *ServiceInstanceProvider::findRegisteredInstances(const TypeId serviceTypeId)
    {
        const auto singletons = _sharedData->getSingletons().findServices(serviceTypeId);
        return singletons ? singletons : _scoped.findServices(serviceTypeId);
    }

    INLINE containers::ServiceInstancesMap *ServiceInstanceProvider::tryGetInstancesMap(const ServiceLifeTime &lifeTime)
    {
        return lifeTime.isTransient() ? nullptr : (lifeTime.isSingleton() ? &_sharedData->getSingletons() : &_scoped);
    }

    INLINE const containers::ServiceDescriptorList *ServiceInstanceProvider::findDescriptors(
        const TypeId serviceTypeId) const
    {
        return _sharedData->getDescriptorsMap().findDescriptors(serviceTypeId);
    }

    INLINE void ServiceInstanceProvider::prebuildSingletons()
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
