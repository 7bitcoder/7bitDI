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
#include "SevenBit/DI/ServiceLifeTimes.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"

namespace sb::di::details::core
{
    INLINE ServiceInstanceProvider::ServiceInstanceProvider(ServiceInstanceProviderRoot &root,
                                                            const ServiceProviderOptions options)
        : _options(options), _root(root), _scoped(_options.strongDestructionOrder)
    {
    }

    INLINE void ServiceInstanceProvider::init(ServiceProvider &serviceProvider)
    {
        _instanceCreator.setServiceProvider(serviceProvider);
        auto external = std::make_unique<services::ExternalService<ServiceProvider>>(&serviceProvider);
        _scoped.insert(external->getTypeId(), std::move(external)).seal();
    }

    INLINE IServiceInstanceProvider::Ptr ServiceInstanceProvider::createScope() const
    {
        return std::make_unique<ServiceInstanceProvider>(_root, _options);
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
        auto instances = findRegisteredInstances(serviceTypeId);
        if (!instances)
        {
            const auto descriptors = findNonTransientDescriptors(serviceTypeId);
            instances = descriptors ? tryRegister(*descriptors, tryCreate(*descriptors)) : nullptr;
        }
        return instances ? instances->last().get() : nullptr;
    }

    INLINE const OneOrList<IServiceInstance::Ptr> *ServiceInstanceProvider::tryGetInstances(const TypeId serviceTypeId)
    {
        auto instances = findRegisteredInstances(serviceTypeId);
        if (!instances)
        {
            const auto descriptors = findNonTransientDescriptors(serviceTypeId);
            instances = descriptors ? tryRegister(*descriptors, tryCreateAll(*descriptors)) : nullptr;
        }
        else if (!instances->isSealed())
        {
            const auto descriptors = findNonTransientDescriptors(serviceTypeId);
            instances = descriptors ? &createRest(*descriptors, *instances) : nullptr;
        }
        return instances ? &instances->getInnerList() : nullptr;
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
        if (const auto descriptors = findTransientDescriptors(serviceTypeId))
        {
            if (descriptors->getLifeTime().isAlias())
            {
                return tryCreateInstance(descriptors->last().getImplementationTypeId());
            }
            return makeResolver(*descriptors).createInstance();
        }
        return nullptr;
    }

    INLINE std::optional<OneOrList<IServiceInstance::Ptr>> ServiceInstanceProvider::tryCreateInstances(
        const TypeId serviceTypeId)
    {
        if (const auto descriptors = findTransientDescriptors(serviceTypeId))
        {
            if (descriptors->getLifeTime().isAlias())
            {
                return tryCreateInstances(descriptors->last().getImplementationTypeId());
            }
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
        if (const auto descriptors = findTransientDescriptors(serviceTypeId);
            descriptors && descriptors->last().getImplementationTypeId() == serviceTypeId)
        {
            return makeResolver(*descriptors).createInstanceInPlace();
        }
        return nullptr;
    }

    INLINE void ServiceInstanceProvider::clear() { _scoped.clear(); }

    INLINE std::optional<containers::ServiceInstanceList> ServiceInstanceProvider::tryCreate(
        const containers::ServiceDescriptorList &descriptors)
    {
        if (descriptors.getLifeTime().isAlias())
        {
            if (const auto original = tryGetInstance(descriptors.last().getImplementationTypeId()))
            {
                return makeResolver(descriptors).createOneAlias(*original);
            }
            return std::nullopt;
        }
        return makeResolver(descriptors).createOneInstanceInPlace();
    }

    INLINE std::optional<containers::ServiceInstanceList> ServiceInstanceProvider::tryCreateAll(
        const containers::ServiceDescriptorList &descriptors)
    {
        if (descriptors.getLifeTime().isAlias())
        {
            if (const auto originals = tryGetInstances(descriptors.last().getImplementationTypeId()))
            {
                return makeResolver(descriptors).createAllAliases(*originals);
            }
            return std::nullopt;
        }
        return makeResolver(descriptors).createAllInstancesInPlace();
    }

    INLINE containers::ServiceInstanceList &ServiceInstanceProvider::createRest(
        const containers::ServiceDescriptorList &descriptors, containers::ServiceInstanceList &instances)
    {
        if (descriptors.getLifeTime().isAlias())
        {
            if (const auto originals = tryGetInstances(descriptors.last().getImplementationTypeId()))
            {
                return makeResolver(descriptors).createRestAliases(*originals, instances);
            }
        }
        return makeResolver(descriptors).createRestInstancesInPlace(instances);
    }

    INLINE containers::ServiceInstanceList *ServiceInstanceProvider::tryRegister(
        const containers::ServiceDescriptorList &descriptors, std::optional<containers::ServiceInstanceList> instances)
    {
        if (!instances)
        {
            return nullptr;
        }
        auto lifeTime = descriptors.getLifeTime();
        if (lifeTime.isAlias())
        {
            const auto originalTypeId = descriptors.last().getImplementationTypeId();
            lifeTime = _scoped.contains(originalTypeId) ? ServiceLifeTimes::Scoped : ServiceLifeTimes::Singleton;
        }
        return &getInstancesMap(lifeTime).insert(descriptors.getServiceTypeId(), std::move(*instances));
    }

    INLINE const ServiceProviderOptions &ServiceInstanceProvider::getOptions() const { return _options; }

    INLINE containers::ServiceInstanceList *ServiceInstanceProvider::findRegisteredInstances(const TypeId serviceTypeId)
    {
        const auto singletons = _root.getSingletons().findServices(serviceTypeId);
        return singletons ? singletons : _scoped.findServices(serviceTypeId);
    }

    INLINE containers::ServiceInstancesMap &ServiceInstanceProvider::getInstancesMap(const ServiceLifeTime &lifeTime)
    {
        return lifeTime.isSingleton() ? _root.getSingletons() : _scoped;
    }

    INLINE const containers::ServiceDescriptorList *ServiceInstanceProvider::findTransientDescriptors(
        const TypeId serviceTypeId) const
    {
        if (const auto descriptors = findDescriptors(serviceTypeId))
        {
            if (descriptors->getLifeTime().isAny(ServiceLifeTimes::Transient, ServiceLifeTimes::Alias))
            {
                return descriptors;
            }
        }
        return nullptr;
    }

    INLINE const containers::ServiceDescriptorList *ServiceInstanceProvider::findNonTransientDescriptors(
        const TypeId serviceTypeId) const
    {
        if (const auto descriptors = findDescriptors(serviceTypeId))
        {
            if (descriptors->getLifeTime().isAny(ServiceLifeTimes::Singleton, ServiceLifeTimes::Scoped,
                                                 ServiceLifeTimes::Alias))
            {
                return descriptors;
            }
        }
        return nullptr;
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
