#pragma once

#include <memory>
#include <utility>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceInstanceProvider.hpp"
#include "SevenBit/DI/Details/Core/ServiceInstanceProviderRoot.hpp"
#include "SevenBit/DI/Details/Services/ExternalService.hpp"
#include "SevenBit/DI/Details/Utils/Check.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

namespace sb::di::details::core
{
    INLINE ServiceInstanceProvider::ServiceInstanceProvider(const ServiceInstanceProvider &provider)
        : _options(provider._options), _root(provider._root), _scoped(_options.strongDestructionOrder)
    {
    }

    INLINE ServiceInstanceProvider::ServiceInstanceProvider(ServiceInstanceProviderRoot &root,
                                                            const ServiceProviderOptions options)
        : _options(options), _root(root), _scoped(_options.strongDestructionOrder)
    {
    }

    INLINE void ServiceInstanceProvider::init(ServiceProvider &serviceProvider)
    {
        _instanceCreator.setServiceProvider(serviceProvider);
        auto external = std::make_unique<services::ExternalService<ServiceProvider>>(&serviceProvider);
        _scoped.insert(typeid(ServiceProvider), std::move(external)).seal();
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

    INLINE const OneOrList<IServiceInstance::Ptr> *ServiceInstanceProvider::tryGetInstances(const TypeId serviceTypeId)
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
            return descriptors ? &createRestInstances(*descriptors, *instances) : nullptr;
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
        if (const auto descriptors = findDescriptors(serviceTypeId);
            descriptors && descriptors->getLifeTime().isTransient())
        {
            return makeResolver(*descriptors).createInstance();
        }
        return nullptr;
    }

    INLINE std::optional<OneOrList<IServiceInstance::Ptr>> ServiceInstanceProvider::tryCreateInstances(
        const TypeId serviceTypeId)
    {
        if (const auto descriptors = findDescriptors(serviceTypeId);
            descriptors && descriptors->getLifeTime().isTransient())
        {
            auto instances = makeResolver(*descriptors).createAllInstances();
            return std::move(instances.getInnerList());
        }
        return std::nullopt;
    }

    INLINE IServiceInstance::Ptr ServiceInstanceProvider::createInstanceInPlace(const TypeId serviceTypeId)
    {
        if (auto instance = tryCreateInstanceInPlace(serviceTypeId); utils::Check::instanceValidity(instance))
        {
            return instance;
        }
        throw ServiceNotFoundException{serviceTypeId,
                                       "Service was not registered, typeid does not match implementationId or was "
                                       "registered as singleton/scoped instance"};
    }

    INLINE IServiceInstance::Ptr ServiceInstanceProvider::tryCreateInstanceInPlace(const TypeId serviceTypeId)
    {
        if (const auto descriptors = findDescriptors(serviceTypeId);
            descriptors && descriptors->getLifeTime().isTransient() &&
            descriptors->last().getImplementationTypeId() == serviceTypeId)
        {
            return makeResolver(*descriptors).createInstanceInPlace();
        }
        return nullptr;
    }

    INLINE void ServiceInstanceProvider::clear() { _scoped.clear(); }

    INLINE IServiceInstance *ServiceInstanceProvider::tryCreateAndRegister(
        const containers::ServiceDescriptorList &descriptors)
    {
        if (const auto instancesMap = tryGetInstancesMap(descriptors.getLifeTime()))
        {
            auto instance = makeResolver(descriptors).createInstanceInPlace();
            return instancesMap->insert(descriptors.getServiceTypeId(), std::move(instance)).last().get();
        }
        return nullptr;
    }

    INLINE OneOrList<IServiceInstance::Ptr> *ServiceInstanceProvider::tryCreateAndRegisterAll(
        const containers::ServiceDescriptorList &descriptors)
    {
        if (const auto instancesMap = tryGetInstancesMap(descriptors.getLifeTime()))
        {
            auto instances = makeResolver(descriptors).createAllInstancesInPlace();
            return &instancesMap->insert(descriptors.getServiceTypeId(), std::move(instances)).getInnerList();
        }
        return nullptr;
    }

    INLINE OneOrList<IServiceInstance::Ptr> &ServiceInstanceProvider::createRestInstances(
        const containers::ServiceDescriptorList &descriptors, containers::ServiceInstanceList &instances)
    {
        return makeResolver(descriptors).createRestInstancesInPlace(instances).getInnerList();
    }

    INLINE const ServiceProviderOptions &ServiceInstanceProvider::getOptions() const { return _options; }

    INLINE containers::ServiceInstanceList *ServiceInstanceProvider::findRegisteredInstances(const TypeId serviceTypeId)
    {
        const auto singletons = _root.getSingletons().findServices(serviceTypeId);
        return singletons ? singletons : _scoped.findServices(serviceTypeId);
    }

    INLINE containers::ServiceInstancesMap *ServiceInstanceProvider::tryGetInstancesMap(const ServiceLifeTime &lifeTime)
    {
        return lifeTime.isTransient() ? nullptr : (lifeTime.isSingleton() ? &_root.getSingletons() : &_scoped);
    }

    INLINE const containers::ServiceDescriptorList *ServiceInstanceProvider::findDescriptors(
        const TypeId serviceTypeId) const
    {
        return _root.getDescriptorsMap().findDescriptors(serviceTypeId);
    }

    INLINE ServiceInstancesResolver
    ServiceInstanceProvider::makeResolver(const containers::ServiceDescriptorList &descriptors)
    {
        auto &creator = descriptors.getLifeTime().isSingleton() ? _root._instanceCreator : _instanceCreator;
        return ServiceInstancesResolver{creator, descriptors};
    }
} // namespace sb::di::details::core
