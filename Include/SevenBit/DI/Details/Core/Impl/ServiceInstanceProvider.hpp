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
            instances = descriptors ? tryCreateAndRegister(*descriptors) : nullptr;
        }
        return instances ? instances->last().get() : nullptr;
    }

    INLINE const OneOrList<IServiceInstance::Ptr> *ServiceInstanceProvider::tryGetInstances(const TypeId serviceTypeId)
    {
        auto instances = findRegisteredInstances(serviceTypeId);
        if (!instances)
        {
            const auto descriptors = findNonTransientDescriptors(serviceTypeId);
            instances = descriptors ? tryCreateAndRegisterAll(*descriptors) : nullptr;
        }
        else if (!instances->isSealed())
        {
            const auto descriptors = findNonTransientDescriptors(serviceTypeId);
            instances = descriptors ? createRestInstances(*descriptors, *instances) : nullptr;
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
            return makeResolver(*descriptors).createInstance();
        }
        return nullptr;
    }

    INLINE std::optional<OneOrList<IServiceInstance::Ptr>> ServiceInstanceProvider::tryCreateInstances(
        const TypeId serviceTypeId)
    {
        if (const auto descriptors = findTransientDescriptors(serviceTypeId))
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
        if (const auto descriptors = findTransientDescriptors(serviceTypeId);
            descriptors && descriptors->last().getImplementationTypeId() == serviceTypeId)
        {
            return makeResolver(*descriptors).createInstanceInPlace();
        }
        return nullptr;
    }

    INLINE void ServiceInstanceProvider::clear() { _scoped.clear(); }

    INLINE containers::ServiceInstanceList *ServiceInstanceProvider::tryCreateAndRegister(
        const containers::ServiceDescriptorList &descriptors)
    {
        auto lifeTime = descriptors.getLifeTime();
        IServiceInstance::Ptr instance;
        if (lifeTime.isAlias())
        {
            const auto aliasTypeId = descriptors.last().getImplementationTypeId();
            if (const auto original = tryGetInstance(aliasTypeId))
            {
                instance = makeResolver(descriptors).createAlias(*original);
                lifeTime = _scoped.contains(aliasTypeId) ? ServiceLifeTime::scoped() : ServiceLifeTime::singleton();
            }
        }
        else
        {
            instance = makeResolver(descriptors).createInstanceInPlace();
        }
        if (const auto instancesMap = tryGetInstancesMap(lifeTime); instancesMap && instance)
        {
            return &instancesMap->insert(descriptors.getServiceTypeId(), std::move(instance));
        }
        return nullptr;
    }

    INLINE containers::ServiceInstanceList *ServiceInstanceProvider::tryCreateAndRegisterAll(
        const containers::ServiceDescriptorList &descriptors)
    {
        auto lifeTime = descriptors.getLifeTime();
        containers::ServiceInstanceList instances{nullptr};
        if (lifeTime.isAlias())
        {
            const auto aliasTypeId = descriptors.last().getImplementationTypeId();
            if (const auto originals = tryGetInstances(aliasTypeId))
            {
                instances = makeResolver(descriptors).createAllAliases(*originals);
                lifeTime = _scoped.contains(aliasTypeId) ? ServiceLifeTime::scoped() : ServiceLifeTime::singleton();
            }
        }
        else
        {
            instances = makeResolver(descriptors).createAllInstancesInPlace();
        }
        if (const auto instancesMap = tryGetInstancesMap(lifeTime))
        {
            return &instancesMap->insert(descriptors.getServiceTypeId(), std::move(instances));
        }
        return nullptr;
    }

    INLINE containers::ServiceInstanceList *ServiceInstanceProvider::createRestInstances(
        const containers::ServiceDescriptorList &descriptors, containers::ServiceInstanceList &instances)
    {
        if (descriptors.getLifeTime().isAlias())
        {
            const auto aliasTypeId = descriptors.last().getImplementationTypeId();
            if (const auto originals = tryGetInstances(aliasTypeId))
            {
                return &makeResolver(descriptors).createRestAliases(*originals, instances).getInnerList();
            }
        }
        return &makeResolver(descriptors).createRestInstancesInPlace(instances);
    }

    INLINE const ServiceProviderOptions &ServiceInstanceProvider::getOptions() const { return _options; }

    INLINE containers::ServiceInstanceList *ServiceInstanceProvider::findRegisteredInstances(const TypeId serviceTypeId)
    {
        const auto singletons = _root.getSingletons().findServices(serviceTypeId);
        return singletons ? singletons : _scoped.findServices(serviceTypeId);
    }

    INLINE containers::ServiceInstancesMap *ServiceInstanceProvider::tryGetInstancesMap(const ServiceLifeTime &lifeTime)
    {
        if (lifeTime.isTransient())
        {
            return nullptr;
        }
        return lifeTime.isSingleton() ? &_root.getSingletons() : &_scoped;
    }

    INLINE const containers::ServiceDescriptorList *ServiceInstanceProvider::findTransientDescriptors(
        const TypeId serviceTypeId) const
    {
        if (const auto descriptors = findDescriptors(serviceTypeId))
        {
            if (descriptors->getLifeTime().isAny(ServiceLifeTime::transient(), ServiceLifeTime::alias()))
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
            if (descriptors->getLifeTime().isAny(ServiceLifeTime::singleton(), ServiceLifeTime::scoped(),
                                                 ServiceLifeTime::alias()))
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
