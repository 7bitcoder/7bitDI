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
        if (const auto instances = findRegisteredInstances(id))
        {
            return &instances->last();
        }
        if (const auto descriptors = findDescriptors(id))
        {
            const auto &descriptor = descriptors->last();
            if (auto instance = tryCreate(descriptor))
            {
                auto &inserted = getContainer(resolveLifeTime(descriptor)).insert(id, std::move(instance));
                if (!descriptors->isAlias() && descriptors->size() == 1)
                {
                    inserted.seal();
                }
                return &inserted.last();
            }
        }
        return nullptr;
    }

    INLINE const OneOrList<ServiceInstance> *ServiceInstanceProvider::tryGetInstances(const ServiceId &id)
    {
        if (const auto instances = findRegisteredInstances(id))
        {
            if (!instances->isSealed())
            {
                if (const auto descriptors = findDescriptors(id))
                {
                    createRest(*descriptors, *instances);
                    instances->seal();
                }
            }
            return &instances->getInnerList();
        }
        if (const auto descriptors = findDescriptors(id))
        {
            if (auto instances = tryCreateAll(*descriptors))
            {
                auto &inserted = getContainer(resolveLifeTime(descriptors->last())).insert(id, std::move(instances));
                inserted.seal();
                return &inserted.getInnerList();
            }
        }
        return nullptr;
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
        const auto descriptors = findTransientDescriptors(id);
        return descriptors ? tryCreateTransient(descriptors->last()) : ServiceInstance{};
    }

    INLINE OneOrList<ServiceInstance> ServiceInstanceProvider::tryCreateInstances(const ServiceId &id)
    {
        const auto descriptors = findTransientDescriptors(id);
        return descriptors ? std::move(tryCreateAllTransient(*descriptors).getInnerList())
                           : OneOrList<ServiceInstance>{};
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
        if (const auto descriptors = findTransientDescriptors(id))
        {
            if (const auto &descriptor = descriptors->last();
                !descriptor.isAlias() && descriptor.getImplementationTypeId() == id.getTypeId())
            {
                return selectCreator(descriptor).createInPlace(descriptor);
            }
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

    INLINE const ServiceDescriptorList *ServiceInstanceProvider::findDescriptors(const ServiceId &id) const
    {
        const auto descriptors = _root.getDescriptorsMap().findDescriptors(id);
        const auto isCorrect = descriptors && (!descriptors->getLifeTime().isTransient() || descriptors->isAlias());
        return isCorrect ? descriptors : nullptr;
    }

    INLINE const ServiceDescriptorList *ServiceInstanceProvider::findTransientDescriptors(const ServiceId &id) const
    {
        const auto descriptors = _root.getDescriptorsMap().findDescriptors(id);
        const auto isCorrect = descriptors && (descriptors->getLifeTime().isTransient() || descriptors->isAlias());
        return isCorrect ? descriptors : nullptr;
    }

    INLINE ServiceLifeTime ServiceInstanceProvider::resolveLifeTime(const ServiceDescriptor &descriptor) const
    {
        if (descriptor.isAlias())
        {
            const ServiceId originalId{descriptor.getImplementationTypeId(), descriptor.getImplementationKey()};
            return _scoped.contains(originalId) ? ServiceLifeTimes::Scoped : ServiceLifeTimes::Singleton;
        }
        return descriptor.getLifeTime();
    }

    INLINE ServiceInstancesMap &ServiceInstanceProvider::getContainer(const ServiceLifeTime lifetime)
    {
        return lifetime.isSingleton() ? _root.getSingletons() : _scoped;
    }

    INLINE ServiceInstance ServiceInstanceProvider::tryCreate(const ServiceDescriptor &descriptor)
    {
        if (descriptor.isAlias())
        {
            return getAliasesCreator().tryCreate(
                descriptor, tryGetInstance({descriptor.getImplementationTypeId(), descriptor.getImplementationKey()}));
        }
        RequireDescriptor::nonTransient(descriptor);
        return selectCreator(descriptor).createInPlace(descriptor);
    }

    INLINE ServiceInstanceList ServiceInstanceProvider::tryCreateAll(const ServiceDescriptorList &descriptors)
    {
        const auto &descriptor = descriptors.last();
        if (descriptor.isAlias())
        {
            return getAliasesCreator().tryCreateAll(descriptors, [&](const ServiceDescriptor &original) {
                return tryGetInstances({original.getImplementationTypeId(), original.getImplementationKey()});
            });
        }
        RequireDescriptor::nonTransient(descriptor);
        return selectCreator(descriptor).createAllInPlace(descriptors);
    }

    INLINE void ServiceInstanceProvider::createRest(const ServiceDescriptorList &descriptors,
                                                    ServiceInstanceList &instances)
    {
        const auto &descriptor = descriptors.last();
        if (descriptor.isAlias())
        {
            return getAliasesCreator().tryCreateRest(descriptors, instances, [&](const ServiceDescriptor &original) {
                return tryGetInstances({original.getImplementationTypeId(), original.getImplementationKey()});
            });
        }
        RequireDescriptor::nonTransient(descriptor);
        selectCreator(descriptor).createRestInPlace(descriptors, instances);
    }

    INLINE ServiceInstance ServiceInstanceProvider::tryCreateTransient(const ServiceDescriptor &descriptor)
    {
        if (descriptor.isAlias())
        {
            return getAliasesCreator().tryMap(descriptor, tryCreateInstance({descriptor.getImplementationTypeId(),
                                                                             descriptor.getImplementationKey()}));
        }
        RequireDescriptor::transient(descriptor);
        return selectCreator(descriptor).create(descriptor);
    }

    INLINE ServiceInstanceList ServiceInstanceProvider::tryCreateAllTransient(const ServiceDescriptorList &descriptors)
    {
        const auto &descriptor = descriptors.last();
        if (descriptor.isAlias())
        {
            return getAliasesCreator().tryMapAll(descriptors, [&](const ServiceDescriptor &original) {
                return tryCreateInstances({original.getImplementationTypeId(), original.getImplementationKey()});
            });
        }
        RequireDescriptor::transient(descriptor);
        return selectCreator(descriptor).createAll(descriptors);
    }

    INLINE ServiceInstancesCreator &ServiceInstanceProvider::selectCreator(const ServiceDescriptor &descriptor)
    {
        return descriptor.getLifeTime().isSingleton() ? _root.getRootCreator() : getCreator();
    }
} // namespace sb::di::details
