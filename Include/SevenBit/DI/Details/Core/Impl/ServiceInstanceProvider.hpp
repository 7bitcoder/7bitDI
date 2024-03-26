#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceInstanceProvider.hpp"
#include "SevenBit/DI/Details/Services/ExternalService.hpp"
#include "SevenBit/DI/Details/Utils/ExtCheck.hpp"
#include "SevenBit/DI/Details/Utils/ExtRequire.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceInstance.hpp"
#include "SevenBit/DI/ServiceLifeTimes.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

namespace sb::di::details
{
    INLINE ServiceInstanceProvider::ServiceInstanceProvider(IServiceInstanceProviderRoot &root,
                                                            const ServiceProviderOptions options)
        : _options(options), _root(root), _scoped(_options.strongDestructionOrder)
    {
    }

    INLINE void ServiceInstanceProvider::init(ServiceProvider &serviceProvider)
    {
        _instanceCreator.setServiceProvider(serviceProvider);
        auto external = std::make_unique<ExternalService<ServiceProvider>>(&serviceProvider);
        _scoped.insert(ServiceId{external->getTypeId()}, ServiceInstance{std::move(external)}).seal();
    }

    INLINE IServiceInstanceProvider::Ptr ServiceInstanceProvider::createScope() const
    {
        return std::make_unique<ServiceInstanceProvider>(_root, _options);
    }

    INLINE const ServiceInstance &ServiceInstanceProvider::getInstance(const ServiceId &id)
    {
        if (const auto instance = tryGetInstance(id); ExtCheck::instanceValidity(instance))
        {
            return *instance;
        }
        throw ServiceNotFoundException{id.getTypeId(),
                                       "Service was not registered or was registered as transient instance"};
    }

    INLINE const ServiceInstance *ServiceInstanceProvider::tryGetInstance(const ServiceId &id)
    {
        auto instances = findRegisteredInstances(id);
        if (!instances)
        {
            if (const auto descriptors = findDescriptors(id, false))
            {
                instances = tryRegisterAndGet(id, *descriptors, tryCreateNonTransient(*descriptors));
            }
        }
        return instances ? &instances->last() : nullptr;
    }

    INLINE const OneOrList<ServiceInstance> *ServiceInstanceProvider::tryGetInstances(const ServiceId &id)
    {
        auto instances = findRegisteredInstances(id);
        if (!instances)
        {
            if (const auto descriptors = findDescriptors(id, false))
            {
                instances = tryRegisterAndGet(id, *descriptors, tryCreateAllNonTransient(*descriptors));
            }
        }
        else if (!instances->isSealed())
        {
            if (const auto descriptors = findDescriptors(id, false))
            {
                instances = createRestNonTransientAndGet(*descriptors, *instances);
            }
        }
        return instances ? &instances->getInnerList() : nullptr;
    }

    INLINE ServiceInstance ServiceInstanceProvider::createInstance(const ServiceId &id)
    {
        if (auto instance = tryCreateInstance(id); ExtCheck::instanceValidity(instance))
        {
            return instance;
        }
        throw ServiceNotFoundException{id.getTypeId(),
                                       "Service was not registered or was registered as singleton/scoped instance"};
    }

    INLINE ServiceInstance ServiceInstanceProvider::tryCreateInstance(const ServiceId &id)
    {
        const auto descriptors = findDescriptors(id, true);
        return descriptors ? tryCreateTransient(*descriptors) : ServiceInstance{};
    }

    INLINE std::optional<OneOrList<ServiceInstance>> ServiceInstanceProvider::tryCreateInstances(const ServiceId &id)
    {
        const auto descriptors = findDescriptors(id, true);
        return descriptors ? tryCreateAllTransient(*descriptors) : std::nullopt;
    }

    INLINE ServiceInstance ServiceInstanceProvider::createInstanceInPlace(const ServiceId &id)
    {
        if (auto instance = tryCreateInstanceInPlace(id); ExtCheck::instanceValidity(instance))
        {
            return instance;
        }
        throw ServiceNotFoundException{id.getTypeId(),
                                       "Service was not registered, typeid does not match implementationId or was "
                                       "registered as singleton/scoped instance"};
    }

    INLINE ServiceInstance ServiceInstanceProvider::tryCreateInstanceInPlace(const ServiceId &id)
    {
        if (const auto descriptors = findDescriptors(id, true);
            descriptors && !descriptors->isAlias() && descriptors->last().getImplementationTypeId() == id.getTypeId())
        {
            return makeResolver(*descriptors).createInstanceInPlace();
        }
        return ServiceInstance{};
    }

    INLINE ServiceInstanceList *ServiceInstanceProvider::findRegisteredInstances(const ServiceId &id)
    {
        const auto singletonsFirts = getOptions().searchInSigletonsFirts;
        auto &first = singletonsFirts ? _root.getSingletons() : _scoped;
        auto &second = singletonsFirts ? _scoped : _root.getSingletons();
        const auto instances = first.findInstances(id);
        return instances ? instances : second.findInstances(id);
    }

    INLINE const ServiceDescriptorList *ServiceInstanceProvider::findDescriptors(const ServiceId &id,
                                                                                 const bool transient) const
    {
        if (const auto descriptors = _root.getDescriptorsMap().findDescriptors(id))
        {
            if (const auto isTransient = descriptors->getLifeTime().isTransient();
                descriptors->isAlias() || (transient ? isTransient : !isTransient))
            {
                return descriptors;
            }
        }
        return nullptr;
    }

    INLINE ServiceInstanceList *ServiceInstanceProvider::tryRegisterAndGet(
        const ServiceId &id, const ServiceDescriptorList &descriptors, std::optional<ServiceInstanceList> &&instances)
    {
        if (instances)
        {
            auto &descriptor = descriptors.last();
            auto lifeTime = descriptor.getLifeTime();
            if (descriptor.isAlias())
            {
                const ServiceId originalId{descriptor.getImplementationTypeId(), descriptor.getImplementationKey()};
                lifeTime = _scoped.contains(originalId) ? ServiceLifeTimes::Scoped : ServiceLifeTimes::Singleton;
            }
            auto &instancesMap = lifeTime.isSingleton() ? _root.getSingletons() : _scoped;
            return &instancesMap.insert(id, std::move(*instances));
        }
        return nullptr;
    }

    INLINE std::optional<ServiceInstanceList> ServiceInstanceProvider::tryCreateNonTransient(
        const ServiceDescriptorList &descriptors)
    {
        if (!descriptors.isAlias())
        {
            ExtRequire::nonTransientDescriptor(descriptors.first());
            return makeResolver(descriptors).createOneInstanceInPlace();
        }
        auto &last = descriptors.last();
        const auto original = tryGetInstance(ServiceId{last.getImplementationTypeId(), last.getImplementationKey()});
        return original ? std::make_optional(makeResolver(descriptors).createOneAlias(*original)) : std::nullopt;
    }

    INLINE std::optional<ServiceInstanceList> ServiceInstanceProvider::tryCreateAllNonTransient(
        const ServiceDescriptorList &descriptors)
    {
        if (!descriptors.isAlias())
        {
            ExtRequire::nonTransientDescriptor(descriptors.first());
            return makeResolver(descriptors).createAllInstancesInPlace();
        }
        auto &last = descriptors.last();
        const auto originals = tryGetInstances(ServiceId{last.getImplementationTypeId(), last.getImplementationKey()});
        return originals ? std::make_optional(makeResolver(descriptors).createAllAliases(*originals)) : std::nullopt;
    }

    INLINE ServiceInstanceList *ServiceInstanceProvider::createRestNonTransientAndGet(
        const ServiceDescriptorList &descriptors, ServiceInstanceList &instances)
    {
        if (!descriptors.isAlias())
        {
            ExtRequire::nonTransientDescriptor(descriptors.first());
            return &makeResolver(descriptors).createRestInstancesInPlace(instances);
        }
        auto &last = descriptors.last();
        const auto originals = tryGetInstances(ServiceId{last.getImplementationTypeId(), last.getImplementationKey()});
        return originals ? &makeResolver(descriptors).createRestAliases(*originals, instances) : nullptr;
    }

    INLINE ServiceInstance ServiceInstanceProvider::tryCreateTransient(const ServiceDescriptorList &descriptors)
    {
        if (!descriptors.isAlias())
        {
            ExtRequire::transientDescriptor(descriptors.first());
            return makeResolver(descriptors).createInstance();
        }
        auto &last = descriptors.last();
        auto alias = tryCreateInstance(ServiceId{last.getImplementationTypeId(), last.getImplementationKey()});
        alias.addCastOffset(last.getCastOffset());
        return alias;
    }

    INLINE std::optional<OneOrList<ServiceInstance>> ServiceInstanceProvider::tryCreateAllTransient(
        const ServiceDescriptorList &descriptors)
    {
        if (!descriptors.isAlias())
        {
            ExtRequire::transientDescriptor(descriptors.first());
            return std::move(makeResolver(descriptors).createAllInstances().getInnerList());
        }
        auto &last = descriptors.last();
        auto aliases = tryCreateInstances(ServiceId{last.getImplementationTypeId(), last.getImplementationKey()});
        if (aliases && last.getCastOffset())
        {
            aliases->forEach([&](ServiceInstance &instance) { instance.addCastOffset(last.getCastOffset()); });
        }
        return aliases;
    }

    INLINE ServiceInstancesResolver ServiceInstanceProvider::makeResolver(const ServiceDescriptorList &descriptors)
    {
        auto &creator = descriptors.getLifeTime().isSingleton() ? _root.getRootInstanceCreator() : getInstanceCreator();
        return ServiceInstancesResolver{creator, descriptors};
    }
} // namespace sb::di::details
