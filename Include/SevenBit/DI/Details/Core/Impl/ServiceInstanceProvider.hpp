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
        _scoped.insert(external->getTypeId(), ServiceInstance{std::move(external)}).seal();
    }

    INLINE const ServiceProviderOptions &ServiceInstanceProvider::getOptions() const { return _options; }

    INLINE IServiceInstanceProvider::Ptr ServiceInstanceProvider::createScope() const
    {
        return std::make_unique<ServiceInstanceProvider>(_root, _options);
    }

    INLINE const ServiceInstance &ServiceInstanceProvider::getInstance(const TypeId serviceTypeId)
    {
        if (const auto instance = tryGetInstance(serviceTypeId); utils::Check::notNull(instance))
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
        return instances ? &instances->last() : nullptr;
    }

    INLINE const OneOrList<ServiceInstance> *ServiceInstanceProvider::tryGetInstances(const TypeId serviceTypeId)
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

    INLINE ServiceInstance ServiceInstanceProvider::createInstance(const TypeId serviceTypeId)
    {
        if (auto instance = tryCreateInstance(serviceTypeId); instance.isValid())
        {
            return instance;
        }
        throw ServiceNotFoundException{serviceTypeId,
                                       "Service was not registered or was registered as singleton/scoped instance"};
    }

    INLINE ServiceInstance ServiceInstanceProvider::tryCreateInstance(const TypeId serviceTypeId)
    {
        if (const auto descriptors = findDescriptors(serviceTypeId, true))
        {
            return descriptors->isAlias() ? tryCreateAlias(descriptors->last())
                                          : makeResolver(*descriptors).createInstance();
        }
        return ServiceInstance{};
    }

    INLINE std::optional<OneOrList<ServiceInstance>> ServiceInstanceProvider::tryCreateInstances(
        const TypeId serviceTypeId)
    {
        if (const auto descriptors = findDescriptors(serviceTypeId, true))
        {
            return descriptors->isAlias() ? tryCreateAliases(descriptors->last())
                                          : std::move(makeResolver(*descriptors).createAllInstances().getInnerList());
        }
        return std::nullopt;
    }

    INLINE ServiceInstance ServiceInstanceProvider::createInstanceInPlace(const TypeId serviceTypeId)
    {
        if (auto instance = tryCreateInstanceInPlace(serviceTypeId); instance.isValid())
        {
            return instance;
        }
        throw ServiceNotFoundException{serviceTypeId,
                                       "Service was not registered, typeid does not match implementationId or was "
                                       "registered as singleton/scoped instance"};
    }

    INLINE ServiceInstance ServiceInstanceProvider::tryCreateInstanceInPlace(const TypeId serviceTypeId)
    {
        if (const auto descriptors = findDescriptors(serviceTypeId, true);
            descriptors && !descriptors->isAlias() && descriptors->last().getImplementationTypeId() == serviceTypeId)
        {
            return makeResolver(*descriptors).createInstanceInPlace();
        }
        return ServiceInstance{};
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
        if (instances)
        {
            auto lifeTime = descriptors.getLifeTime();
            if (descriptors.isAlias())
            {
                const auto originalTypeId = descriptors.last().getImplementationTypeId();
                lifeTime = _scoped.contains(originalTypeId) ? ServiceLifeTimes::Scoped : ServiceLifeTimes::Singleton;
            }
            auto &instancesMap = lifeTime.isSingleton() ? _root.getSingletons() : _scoped;
            return &instancesMap.insert(descriptors.getServiceTypeId(), std::move(*instances));
        }
        return nullptr;
    }

    INLINE std::optional<containers::ServiceInstanceList> ServiceInstanceProvider::tryCreate(
        const containers::ServiceDescriptorList &descriptors)
    {
        if (descriptors.isAlias())
        {
            const auto original = tryGetInstance(descriptors.last().getImplementationTypeId());
            return original ? std::make_optional(makeResolver(descriptors).createOneAlias(*original)) : std::nullopt;
        }
        return makeResolver(descriptors).createOneInstanceInPlace();
    }

    INLINE std::optional<containers::ServiceInstanceList> ServiceInstanceProvider::tryCreateAll(
        const containers::ServiceDescriptorList &descriptors)
    {
        if (descriptors.isAlias())
        {
            const auto originals = tryGetInstances(descriptors.last().getImplementationTypeId());
            return originals ? std::make_optional(makeResolver(descriptors).createAllAliases(*originals))
                             : std::nullopt;
        }
        return makeResolver(descriptors).createAllInstancesInPlace();
    }

    INLINE containers::ServiceInstanceList *ServiceInstanceProvider::createRestAndGet(
        const containers::ServiceDescriptorList &descriptors, containers::ServiceInstanceList &instances)
    {
        if (descriptors.isAlias())
        {
            const auto originals = tryGetInstances(descriptors.last().getImplementationTypeId());
            return originals ? &makeResolver(descriptors).createRestAliases(*originals, instances) : nullptr;
        }
        return &makeResolver(descriptors).createRestInstancesInPlace(instances);
    }

    INLINE ServiceInstance ServiceInstanceProvider::tryCreateAlias(const ServiceDescriptor &descriptor)
    {
        auto alias = tryCreateInstance(descriptor.getImplementationTypeId());
        alias.addCastOffset(descriptor.getCastOffset());
        return alias;
    }

    INLINE std::optional<OneOrList<ServiceInstance>> ServiceInstanceProvider::tryCreateAliases(
        const ServiceDescriptor &descriptor)
    {
        auto aliases = tryCreateInstances(descriptor.getImplementationTypeId());
        if (aliases && descriptor.getCastOffset())
        {
            aliases->forEach([&](ServiceInstance &instance) { instance.addCastOffset(descriptor.getCastOffset()); });
        }
        return aliases;
    }

    INLINE ServiceInstancesResolver
    ServiceInstanceProvider::makeResolver(const containers::ServiceDescriptorList &descriptors)
    {
        auto &creator = descriptors.getLifeTime().isSingleton() ? _root._instanceCreator : _instanceCreator;
        return ServiceInstancesResolver{creator, descriptors};
    }
} // namespace sb::di::details::core
