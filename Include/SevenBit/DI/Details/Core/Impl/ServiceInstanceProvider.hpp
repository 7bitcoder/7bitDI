#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceInstanceProvider.hpp"
#include "SevenBit/DI/Details/Services/ExternalService.hpp"
#include "SevenBit/DI/Details/Utils/RequireDescriptor.hpp"
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
        if (const auto instance = tryGetInstance(id); instance && *instance)
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
                instances = createRestNonTransient(*descriptors, *instances);
            }
        }
        return instances ? &instances->getInnerList() : nullptr;
    }

    INLINE ServiceInstance ServiceInstanceProvider::createInstance(const ServiceId &id)
    {
        if (auto instance = tryCreateInstance(id))
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
        if (auto instance = tryCreateInstanceInPlace(id))
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
            return makeResolver(*descriptors).createInPlace();
        }
        return ServiceInstance{};
    }

    INLINE ServiceInstanceList *ServiceInstanceProvider::findRegisteredInstances(const ServiceId &id)
    {
        const auto singletonsFirst = getOptions().searchInSigletonsFirst;
        auto &first = singletonsFirst ? _root.getSingletons() : _scoped;
        auto &second = singletonsFirst ? _scoped : _root.getSingletons();
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
            RequireDescriptor::nonTransient(descriptors.first());
            return makeResolver(descriptors).createOneInPlace();
        }
        auto &last = descriptors.last();
        return makeAliasResolver(descriptors)
            .createOne(tryGetInstance({last.getImplementationTypeId(), last.getImplementationKey()}));
    }

    INLINE std::optional<ServiceInstanceList> ServiceInstanceProvider::tryCreateAllNonTransient(
        const ServiceDescriptorList &descriptors)
    {
        if (!descriptors.isAlias())
        {
            RequireDescriptor::nonTransient(descriptors.first());
            return makeResolver(descriptors).createAllInPlace();
        }
        return makeAliasResolver(descriptors).createAll([&](const ServiceDescriptor &aliasDescriptor) {
            return tryGetInstances({aliasDescriptor.getImplementationTypeId(), aliasDescriptor.getImplementationKey()});
        });
    }

    INLINE ServiceInstanceList *ServiceInstanceProvider::createRestNonTransient(
        const ServiceDescriptorList &descriptors, ServiceInstanceList &instances)
    {
        if (!descriptors.isAlias())
        {
            RequireDescriptor::nonTransient(descriptors.first());
            return &makeResolver(descriptors).createRestInPlace(instances);
        }
        makeAliasResolver(descriptors).createRest(instances, [&](const ServiceDescriptor &aliasDescriptor) {
            return tryGetInstances({aliasDescriptor.getImplementationTypeId(), aliasDescriptor.getImplementationKey()});
        });
        return &instances;
    }

    INLINE ServiceInstance ServiceInstanceProvider::tryCreateTransient(const ServiceDescriptorList &descriptors)
    {
        if (!descriptors.isAlias())
        {
            RequireDescriptor::transient(descriptors.first());
            return makeResolver(descriptors).create();
        }
        auto &last = descriptors.last();
        return makeAliasResolver(descriptors)
            .map(tryCreateInstance({last.getImplementationTypeId(), last.getImplementationKey()}));
    }

    INLINE std::optional<OneOrList<ServiceInstance>> ServiceInstanceProvider::tryCreateAllTransient(
        const ServiceDescriptorList &descriptors)
    {
        if (!descriptors.isAlias())
        {
            RequireDescriptor::transient(descriptors.first());
            return std::move(makeResolver(descriptors).createAll().getInnerList());
        }
        return makeAliasResolver(descriptors).mapAll([&](const ServiceDescriptor &aliasDescriptor) {
            return tryCreateInstances(
                {aliasDescriptor.getImplementationTypeId(), aliasDescriptor.getImplementationKey()});
        });
    }

    INLINE ServiceInstancesResolver ServiceInstanceProvider::makeResolver(const ServiceDescriptorList &descriptors)
    {
        auto &creator = descriptors.getLifeTime().isSingleton() ? _root.getRootInstanceCreator() : getInstanceCreator();
        return ServiceInstancesResolver{creator, descriptors};
    }

    INLINE ServiceAliasInstancesResolver
    ServiceInstanceProvider::makeAliasResolver(const ServiceDescriptorList &descriptors)
    {
        auto &creator = descriptors.getLifeTime().isSingleton() ? _root.getRootInstanceCreator() : getInstanceCreator();
        return ServiceAliasInstancesResolver{creator, descriptors};
    }
} // namespace sb::di::details
