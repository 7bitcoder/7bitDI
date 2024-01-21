#pragma once

#include <memory>
#include <utility>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceInstanceCreator.hpp"
#include "SevenBit/DI/Details/Services/ExternalService.hpp"
#include "SevenBit/DI/Details/Utils/Check.hpp"
#include "SevenBit/DI/Details/Utils/Require.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"

namespace sb::di::details::core
{
    INLINE ServiceInstanceCreator::ServiceInstanceCreator(const ServiceInstanceCreator &provider)
        : _options(provider._options), _root(provider._root), _scoped(_options.strongDestructionOrder)
    {
    }

    INLINE ServiceInstanceCreator::ServiceInstanceCreator(ServiceInstanceCreatorRoot &root,
                                                          ServiceProviderOptions options)
        : _options(options), _root(root), _scoped(_options.strongDestructionOrder)
    {
    }

    INLINE void ServiceInstanceCreator::init(ServiceProvider &serviceProvider)
    {
        _serviceProvider = &serviceProvider;
        auto external = std::make_unique<services::ExternalService<ServiceProvider>>(_serviceProvider);
        _scoped.insert(typeid(ServiceProvider), std::move(external)).seal();
    }

    INLINE IServiceInstanceCreator::Ptr ServiceInstanceCreator::createScope() const
    {
        return IServiceInstanceCreator::Ptr{new ServiceInstanceCreator{*this}};
    }

    INLINE const IServiceInstance &ServiceInstanceCreator::getInstance(const TypeId serviceTypeId)
    {
        if (const auto instance = tryGetInstance(serviceTypeId); utils::Check::instanceValidity(instance))
        {
            return *instance;
        }
        throw ServiceNotFoundException{serviceTypeId,
                                       "Service was not registered or was registered as transient instance"};
    }

    INLINE const IServiceInstance *ServiceInstanceCreator::tryGetInstance(const TypeId serviceTypeId)
    {
        if (const auto instances = findRegisteredInstances(serviceTypeId))
        {
            return instances->last().get();
        }
        const auto descriptors = findDescriptors(serviceTypeId);
        return descriptors ? tryCreateAndRegister(*descriptors) : nullptr;
    }

    INLINE const OneOrList<IServiceInstance::Ptr> *ServiceInstanceCreator::tryGetInstances(const TypeId serviceTypeId)
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

    INLINE IServiceInstance::Ptr ServiceInstanceCreator::createInstance(const TypeId serviceTypeId)
    {
        if (auto instance = tryCreateInstance(serviceTypeId); utils::Check::instanceValidity(instance))
        {
            return instance;
        }
        throw ServiceNotFoundException{serviceTypeId,
                                       "Service was not registered or was registered as singleton/scoped instance"};
    }

    INLINE IServiceInstance::Ptr ServiceInstanceCreator::tryCreateInstance(const TypeId serviceTypeId)
    {
        const auto descriptors = findDescriptors(serviceTypeId);
        return descriptors ? tryCreate(descriptors->last(), false) : nullptr;
    }

    INLINE std::optional<OneOrList<IServiceInstance::Ptr>> ServiceInstanceCreator::tryCreateInstances(
        TypeId serviceTypeId)
    {
        const auto descriptors = findDescriptors(serviceTypeId);
        return descriptors ? tryCreateAll(*descriptors) : std::nullopt;
    }

    INLINE IServiceInstance::Ptr ServiceInstanceCreator::createInstanceInPlace(const TypeId serviceTypeId)
    {
        if (auto instance = tryCreateInstanceInPlace(serviceTypeId); utils::Check::instanceValidity(instance))
        {
            return instance;
        }
        throw ServiceNotFoundException{serviceTypeId,
                                       "Service was not registered, typeid does not match implementationId or was "
                                       "registered as singleton/scoped instance"};
    }

    INLINE IServiceInstance::Ptr ServiceInstanceCreator::tryCreateInstanceInPlace(const TypeId serviceTypeId)
    {
        if (const auto descriptors = findDescriptors(serviceTypeId);
            descriptors && descriptors->last().getImplementationTypeId() == serviceTypeId)
        {
            return tryCreate(descriptors->last(), true);
        }
        return nullptr;
    }

    INLINE void ServiceInstanceCreator::clear() { _scoped.clear(); }

    INLINE const IServiceInstance *ServiceInstanceCreator::tryCreateAndRegister(
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

    INLINE const OneOrList<IServiceInstance::Ptr> *ServiceInstanceCreator::tryCreateAndRegisterAll(
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

    INLINE OneOrList<IServiceInstance::Ptr> &ServiceInstanceCreator::createRestInstances(
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

    INLINE IServiceInstance::Ptr ServiceInstanceCreator::tryCreate(const ServiceDescriptor &descriptor,
                                                                   const bool inPlaceRequest)
    {
        return descriptor.getLifeTime().isTransient() ? createInstance(descriptor, inPlaceRequest) : nullptr;
    }

    INLINE std::optional<OneOrList<IServiceInstance::Ptr>> ServiceInstanceCreator::tryCreateAll(
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

    INLINE IServiceInstance::Ptr ServiceInstanceCreator::createInstance(const ServiceDescriptor &descriptor,
                                                                        const bool inPlaceRequest)
    {
        if (descriptor.getLifeTime().isSingleton() && this != &_root)
        {
            return _root.createInstance(descriptor, inPlaceRequest);
        }
        auto _ = _root.spawnGuard(descriptor.getImplementationTypeId());
        return utils::Require::validInstanceAndGet(descriptor.getImplementationFactory().createInstance(
            *utils::Require::notNullAndGet(_serviceProvider), inPlaceRequest));
    }

    INLINE const ServiceProviderOptions &ServiceInstanceCreator::getOptions() const { return _options; }

    INLINE containers::ServiceInstanceList *ServiceInstanceCreator::findRegisteredInstances(const TypeId serviceTypeId)
    {
        const auto singletons = _root.getSingletons().findServices(serviceTypeId);
        return singletons ? singletons : _scoped.findServices(serviceTypeId);
    }

    INLINE containers::ServiceInstancesMap *ServiceInstanceCreator::tryGetInstancesMap(const ServiceLifeTime &lifeTime)
    {
        return lifeTime.isTransient() ? nullptr : (lifeTime.isSingleton() ? &_root.getSingletons() : &_scoped);
    }

    INLINE const containers::ServiceDescriptorList *ServiceInstanceCreator::findDescriptors(
        const TypeId serviceTypeId) const
    {
        return _root.getDescriptorsMap().findDescriptors(serviceTypeId);
    }
} // namespace sb::di::details::core
