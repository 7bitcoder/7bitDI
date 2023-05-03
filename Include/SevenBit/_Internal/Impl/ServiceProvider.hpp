#pragma once

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/_Internal/ServiceProvider.hpp"

namespace sb::internal
{
    INLINE ServiceProvider::ServiceProvider(ServiceProvider *root) : _root(root) {}

    INLINE IServiceProvider::Ptr ServiceProvider::createScope()
    {
        return std::unique_ptr<ServiceProvider>(new ServiceProvider{_root});
    }

    INLINE void *ServiceProvider::getService(TypeId serviceTypeId)
    {
        if (auto list = singletons().getList(serviceTypeId))
        {
            return list->first();
        }
        if (auto list = scoped().getList(serviceTypeId))
        {
            return list->first();
        }
        return createAndRegister(serviceTypeId);
    }

    INLINE void *ServiceProvider::getRequiredService(TypeId serviceTypeId)
    {
        if (auto service = getService(serviceTypeId))
        {
            return service;
        }
        throw ServiceNotRegisteredException{serviceTypeId};
    }

    INLINE std::vector<void *> ServiceProvider::getServices(TypeId serviceTypeId)
    {
        if (auto list = singletons().getList(serviceTypeId); list && list->isSealed())
        {
            return list->getAll();
        }
        if (auto list = scoped().getList(serviceTypeId); list && list->isSealed())
        {
            return list->getAll();
        }
        return createAndRegisterAll(serviceTypeId);
    }

    INLINE void *ServiceProvider::createService(TypeId serviceTypeId) { return create(serviceTypeId); }

    INLINE void *ServiceProvider::createRequiredService(TypeId serviceTypeId)
    {
        if (auto service = createService(serviceTypeId))
        {
            return service;
        }
        throw ServiceNotRegisteredException{serviceTypeId};
    }

    INLINE std::vector<void *> ServiceProvider::createServices(TypeId serviceTypeId)
    {
        return createAll(serviceTypeId);
    }

    INLINE void *ServiceProvider::createAndRegister(TypeId serviceTypeId)
    {
        if (auto descriptor = getDescriptorsMap().getDescriptor(serviceTypeId))
        {
            return createAndRegister(*descriptor);
        }
        return nullptr;
    }

    INLINE std::vector<void *> ServiceProvider::createAndRegisterAll(TypeId serviceTypeId)
    {
        if (auto descriptors = getDescriptorsMap().getDescriptorsList(serviceTypeId))
        {
            return createAndRegisterAll(*descriptors);
        }
        return {};
    }

    INLINE void *ServiceProvider::create(TypeId serviceTypeId)
    {
        if (auto descriptor = getDescriptorsMap().getDescriptor(serviceTypeId))
        {
            return create(*descriptor);
        }
        return nullptr;
    }

    INLINE std::vector<void *> ServiceProvider::createAll(TypeId serviceTypeId)
    {
        if (auto descriptors = getDescriptorsMap().getDescriptorsList(serviceTypeId))
        {
            return createAll(*descriptors);
        }
        return {};
    }

    INLINE void *ServiceProvider::createAndRegister(const ServiceDescriptor &descriptor)
    {
        auto &lifeTime = descriptor.getLifeTime();
        if (lifeTime.isTransient())
        {
            throw TransientForbidException{descriptor.getServiceTypeId()};
        }
        auto instance = createInstance(descriptor);
        auto &servicesMap = lifeTime.isSingleton() ? singletons() : scoped();
        return servicesMap[descriptor.getServiceTypeId()].add(std::move(instance)).at(0);
    }

    INLINE std::vector<void *> ServiceProvider::createAndRegisterAll(const ServiceDescriptorList &descriptors)
    {
        auto &lifeTime = descriptors.getLifeTime();
        if (lifeTime.isTransient())
        {
            throw TransientForbidException{descriptors.last().getServiceTypeId()};
        }
        auto &container = lifeTime.isSingleton() ? singletons() : scoped();
        ServiceList *serviceList = container.getList(descriptors.getServiceTypeId());

        if (!serviceList)
        {
            serviceList = &container[descriptors.getServiceTypeId()].add(createInstance(descriptors.last()));
        }
        serviceList->reserve(descriptors.size());
        for (auto it = ++descriptors.rBegin(); it != descriptors.rEnd(); ++it) // skip main service
        {
            serviceList->add(createInstance(*it));
        }
        serviceList->seal();
        return serviceList->getAll();
    }

    INLINE void *ServiceProvider::create(const ServiceDescriptor &descriptor)
    {
        auto &lifetime = descriptor.getLifeTime();
        if (!lifetime.isTransient())
        {
            throw NotTransientException{descriptor.getServiceTypeId()};
        }
        auto instance = createInstance(descriptor);
        return instance->moveOut();
    }

    INLINE std::vector<void *> ServiceProvider::createAll(const ServiceDescriptorList &descriptors)
    {
        std::vector<void *> result;
        if (!descriptors.getLifeTime().isTransient())
        {
            throw NotTransientException{descriptors.getServiceTypeId()};
        }
        result.reserve(descriptors.size());
        for (auto &descriptor : descriptors)
        {
            result.push_back(create(descriptor));
        }
        return result;
    }

    INLINE IServiceInstance::Ptr ServiceProvider::createInstance(const ServiceDescriptor &descriptor)
    {
        auto scopedGuard = _guard.spawnGuard(descriptor.getImplementationTypeId());

        auto instance = descriptor.getImplementationFactory().createInstance(*this);

        if (instance && instance->isValid())
        {
            return instance;
        }
        throw InvalidServiceException{descriptor.getImplementationTypeId(), "Service instance is null or is invalid"};
    }

    INLINE const ServiceDescriptorsMap &ServiceProvider::getDescriptorsMap()
    {
        if (_root && _root->_descriptorsMap)
        {
            return *_root->_descriptorsMap;
        }
        throw NullPointnerException{"root cannot be null"};
    }

    INLINE ServicesMap &ServiceProvider::scoped() { return _services; }

    INLINE ServicesMap &ServiceProvider::singletons()
    {
        if (_root)
        {
            return _root->_services;
        }
        throw NullPointnerException{"root cannot be null"};
    }
} // namespace sb::internal
