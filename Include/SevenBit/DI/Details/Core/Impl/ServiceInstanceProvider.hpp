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
        return tryGetInstanceWithLifeTime(serviceTypeId).first;
    }

    INLINE const OneOrList<IServiceInstance::Ptr> *ServiceInstanceProvider::tryGetInstances(const TypeId serviceTypeId)
    {
        if (auto [instances, _] = tryGetInstancesWithLifeTime(serviceTypeId); instances)
        {
            return &instances->getInnerList();
        }
        return nullptr;
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

    INLINE std::pair<IServiceInstance *, ServiceLifeTime> ServiceInstanceProvider::tryGetInstanceWithLifeTime(
        const TypeId serviceTypeId)
    {
        if (const auto [instances, lifeTime] = findRegisteredInstances(serviceTypeId); instances)
        {
            return {instances->last().get(), lifeTime};
        }
        const auto descriptors = findDescriptors(serviceTypeId);
        return descriptors ? tryCreateAndRegister(*descriptors)
                           : std::pair<IServiceInstance *, ServiceLifeTime>{nullptr, ServiceLifeTime::scoped()};
    }

    INLINE std::pair<containers::ServiceInstanceList *, ServiceLifeTime> ServiceInstanceProvider::
        tryGetInstancesWithLifeTime(const TypeId serviceTypeId)
    {
        const auto [instances, lifeTime] = findRegisteredInstances(serviceTypeId);
        if (!instances)
        {
            const auto descriptors = findDescriptors(serviceTypeId);
            return descriptors ? tryCreateAndRegisterAll(*descriptors)
                               : std::pair<containers::ServiceInstanceList *, ServiceLifeTime>{
                                     nullptr, ServiceLifeTime::scoped()};
        }
        if (!instances->isSealed())
        {
            const auto descriptors = findDescriptors(serviceTypeId);
            return descriptors ? createRestInstances(*descriptors, *instances)
                               : std::pair<containers::ServiceInstanceList *, ServiceLifeTime>{
                                     nullptr, ServiceLifeTime::scoped()};
        }
        return {instances, lifeTime};
    }

    INLINE std::pair<IServiceInstance *, ServiceLifeTime> ServiceInstanceProvider::tryCreateAndRegister(
        const containers::ServiceDescriptorList &descriptors)
    {
        auto lifeTime = descriptors.getLifeTime();
        IServiceInstance::Ptr instance;
        if (descriptors.getLifeTime().isAlias())
        {
            auto [original, originalLifeTime] =
                tryGetInstanceWithLifeTime(descriptors.last().getImplementationTypeId());
            if (original)
            {
                instance = makeResolver(descriptors).createAlias(*original);
                lifeTime = originalLifeTime;
            }
        }
        else
        {
            instance = makeResolver(descriptors).createInstanceInPlace();
        }
        if (const auto instancesMap = tryGetInstancesMap(lifeTime); instancesMap && instance)
        {
            return {instancesMap->insert(descriptors.getServiceTypeId(), std::move(instance)).last().get(), lifeTime};
        }
        return {nullptr, lifeTime};
    }

    INLINE std::pair<containers::ServiceInstanceList *, ServiceLifeTime> ServiceInstanceProvider::
        tryCreateAndRegisterAll(const containers::ServiceDescriptorList &descriptors)
    {
        auto lifeTime = descriptors.getLifeTime();
        containers::ServiceInstanceList instances{nullptr};
        if (descriptors.getLifeTime().isAlias())
        {
            auto [originals, originalLifeTime] =
                tryGetInstancesWithLifeTime(descriptors.last().getImplementationTypeId());
            if (originals)
            {
                instances = makeResolver(descriptors).createAllAliases(*originals);
                lifeTime = originalLifeTime;
            }
        }
        else
        {
            instances = makeResolver(descriptors).createAllInstancesInPlace();
        }
        if (const auto instancesMap = tryGetInstancesMap(lifeTime); instancesMap && instances.last())
        {
            return {&instancesMap->insert(descriptors.getServiceTypeId(), std::move(instances)), lifeTime};
        }
        return {nullptr, ServiceLifeTime::scoped()};
    }

    INLINE std::pair<containers::ServiceInstanceList *, ServiceLifeTime> ServiceInstanceProvider::createRestInstances(
        const containers::ServiceDescriptorList &descriptors, containers::ServiceInstanceList &instances)
    {
        auto lifeTime = descriptors.getLifeTime();
        if (descriptors.getLifeTime().isAlias())
        {
            auto [originals, originalLifeTime] =
                tryGetInstancesWithLifeTime(descriptors.last().getImplementationTypeId());
            if (originals)
            {
                return {&makeResolver(descriptors).createRestAliases(instances, *originals), originalLifeTime};
            }
        }
        else
        {
            instances = makeResolver(descriptors).createAllInstancesInPlace();
        }

        return {&makeResolver(descriptors).createRestInstancesInPlace(instances), lifeTime};
    }

    INLINE const ServiceProviderOptions &ServiceInstanceProvider::getOptions() const { return _options; }

    INLINE std::pair<containers::ServiceInstanceList *, ServiceLifeTime> ServiceInstanceProvider::
        findRegisteredInstances(const TypeId serviceTypeId)
    {
        if (const auto singletons = _root.getSingletons().findServices(serviceTypeId))
        {
            return {singletons, ServiceLifeTime::singleton()};
        }
        return {_scoped.findServices(serviceTypeId), ServiceLifeTime::scoped()};
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
