#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceInstanceProvider.hpp"
#include "SevenBit/DI/Details/Core/ServiceInstanceProviderRoot.hpp"
#include "SevenBit/DI/Details/Services/ExternalService.hpp"
#include "SevenBit/DI/Details/Utils/Check.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceInstance.hpp"
#include "SevenBit/DI/ServiceLifeTimes.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

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

    INLINE const ServiceProviderOptions &ServiceInstanceProvider::getOptions() const { return _options; }

    INLINE IServiceInstanceProvider::Ptr ServiceInstanceProvider::createScope() const
    {
        return std::make_unique<ServiceInstanceProvider>(_root, _options);
    }

    INLINE const ServiceInstance &ServiceInstanceProvider::getInstance(const TypeId serviceTypeId)
    {
        if (const auto instance = tryGetInstance(serviceTypeId); utils::Check::instanceValidity(instance))
        {
            return *instance;
        }
        throw ServiceNotFoundException{serviceTypeId,
                                       "Service was not registered or was registered as transient instance"};
    }

    INLINE const ServiceInstance *ServiceInstanceProvider::tryGetInstance(const TypeId serviceTypeId)
    {
        auto instances = findRegisteredInstances(serviceTypeId);
        if (!instances)
        {
            const auto descriptors = findDescriptors(serviceTypeId, false);
            instances = descriptors ? tryRegisterAndGet(*descriptors, tryCreate(*descriptors)) : nullptr;
        }
        return instances ? instances->last().get() : nullptr;
    }

    INLINE const OneOrList<ServiceInstance::Ptr> *ServiceInstanceProvider::tryGetInstances(const TypeId serviceTypeId)
    {
        auto instances = findRegisteredInstances(serviceTypeId);
        if (!instances)
        {
            const auto descriptors = findDescriptors(serviceTypeId, false);
            instances = descriptors ? tryRegisterAndGet(*descriptors, tryCreateAll(*descriptors)) : nullptr;
        }
        else if (!instances->isSealed())
        {
            const auto descriptors = findDescriptors(serviceTypeId, false);
            instances = descriptors ? createRestAndGet(*descriptors, *instances) : nullptr;
        }
        return instances ? &instances->getInnerList() : nullptr;
    }

    INLINE ServiceInstance::Ptr ServiceInstanceProvider::createInstance(const TypeId serviceTypeId)
    {
        if (auto instance = tryCreateInstance(serviceTypeId); utils::Check::instanceValidity(instance))
        {
            return instance;
        }
        throw ServiceNotFoundException{serviceTypeId,
                                       "Service was not registered or was registered as singleton/scoped instance"};
    }

    INLINE ServiceInstance::Ptr ServiceInstanceProvider::tryCreateInstance(const TypeId serviceTypeId)
    {
        if (const auto descriptors = findDescriptors(serviceTypeId, true))
        {
            if (descriptors->isAlias())
            {
                auto alias = tryCreateInstance(descriptors->last().getImplementationTypeId());
                tryAddOffset(alias.get(), descriptors->last());
                return alias;
            }
            return makeResolver(*descriptors).createInstance();
        }
        return nullptr;
    }

    INLINE std::optional<OneOrList<ServiceInstance::Ptr>> ServiceInstanceProvider::tryCreateInstances(
        const TypeId serviceTypeId)
    {
        if (const auto descriptors = findDescriptors(serviceTypeId, true))
        {
            if (descriptors->isAlias())
            {
                auto aliases = tryCreateInstances(descriptors->last().getImplementationTypeId());
                tryAddOffset(aliases, descriptors->last());
                return std::move(aliases);
            }
            return std::move(makeResolver(*descriptors).createAllInstances().getInnerList());
        }
        return std::nullopt;
    }

    INLINE ServiceInstance::Ptr ServiceInstanceProvider::createInstanceInPlace(const TypeId serviceTypeId)
    {
        if (auto instance = tryCreateInstanceInPlace(serviceTypeId); utils::Check::instanceValidity(instance))
        {
            return instance;
        }
        throw ServiceNotFoundException{serviceTypeId,
                                       "Service was not registered, typeid does not match implementationId or was "
                                       "registered as singleton/scoped instance"};
    }

    INLINE ServiceInstance::Ptr ServiceInstanceProvider::tryCreateInstanceInPlace(const TypeId serviceTypeId)
    {
        if (const auto descriptors = findDescriptors(serviceTypeId, true);
            descriptors && !descriptors->isAlias() && descriptors->last().getImplementationTypeId() == serviceTypeId)
        {
            return makeResolver(*descriptors).createInstanceInPlace();
        }
        return nullptr;
    }

    INLINE void ServiceInstanceProvider::clear() { _scoped.clear(); }

    INLINE containers::ServiceInstanceList *ServiceInstanceProvider::findRegisteredInstances(const TypeId serviceTypeId)
    {
        const auto singletons = _root.getSingletons().findServices(serviceTypeId);
        return singletons ? singletons : _scoped.findServices(serviceTypeId);
    }

    INLINE const containers::ServiceDescriptorList *ServiceInstanceProvider::findDescriptors(const TypeId serviceTypeId,
                                                                                             const bool transient) const
    {
        if (const auto descriptors = _root.getDescriptorsMap().findDescriptors(serviceTypeId))
        {
            if (const auto isTransient = descriptors->getLifeTime().isTransient();
                descriptors->isAlias() || (transient ? isTransient : !isTransient))
            {
                return descriptors;
            }
        }
        return nullptr;
    }

    INLINE containers::ServiceInstanceList *ServiceInstanceProvider::tryRegisterAndGet(
        const containers::ServiceDescriptorList &descriptors, std::optional<containers::ServiceInstanceList> instances)
    {
        if (!instances)
        {
            return nullptr;
        }
        auto lifeTime = descriptors.getLifeTime();
        if (descriptors.isAlias())
        {
            const auto originalTypeId = descriptors.last().getImplementationTypeId();
            lifeTime = _scoped.contains(originalTypeId) ? ServiceLifeTimes::Scoped : ServiceLifeTimes::Singleton;
        }
        auto &instancesMap = lifeTime.isSingleton() ? _root.getSingletons() : _scoped;
        return &instancesMap.insert(descriptors.getServiceTypeId(), std::move(*instances));
    }

    INLINE std::optional<containers::ServiceInstanceList> ServiceInstanceProvider::tryCreate(
        const containers::ServiceDescriptorList &descriptors)
    {
        if (descriptors.isAlias())
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
        if (descriptors.isAlias())
        {
            if (const auto originals = tryGetInstances(descriptors.last().getImplementationTypeId()))
            {
                return makeResolver(descriptors).createAllAliases(*originals);
            }
            return std::nullopt;
        }
        return makeResolver(descriptors).createAllInstancesInPlace();
    }

    INLINE containers::ServiceInstanceList *ServiceInstanceProvider::createRestAndGet(
        const containers::ServiceDescriptorList &descriptors, containers::ServiceInstanceList &instances)
    {
        if (descriptors.isAlias())
        {
            if (const auto originals = tryGetInstances(descriptors.last().getImplementationTypeId()))
            {
                return &makeResolver(descriptors).createRestAliases(*originals, instances);
            }
            return nullptr;
        }
        return &makeResolver(descriptors).createRestInstancesInPlace(instances);
    }

    INLINE ServiceInstancesResolver
    ServiceInstanceProvider::makeResolver(const containers::ServiceDescriptorList &descriptors)
    {
        auto &creator = descriptors.getLifeTime().isSingleton() ? _root._instanceCreator : _instanceCreator;
        return ServiceInstancesResolver{creator, descriptors};
    }

    INLINE void ServiceInstanceProvider::tryAddOffset(ServiceInstance *instance, const ServiceDescriptor &descriptor)
    {
        if (instance && descriptor.getCastOffset())
        {
            instance->addGetOffset(descriptor.getCastOffset());
        }
    }

    INLINE void ServiceInstanceProvider::tryAddOffset(std::optional<OneOrList<ServiceInstance::Ptr>> &instances,
                                                      const ServiceDescriptor &descriptor)
    {
        if (instances && descriptor.getCastOffset())
        {
            if (instances->size() == 1)
            {
                return tryAddOffset(instances->first().get(), descriptor);
            }
            for (auto &instance : instances->getAsList())
            {
                tryAddOffset(instance.get(), descriptor);
            }
        }
    }
} // namespace sb::di::details::core
