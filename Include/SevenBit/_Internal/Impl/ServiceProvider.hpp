#pragma once

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/Exceptions.hpp"
#include "SevenBit/ServiceProviderOptions.hpp"
#include "SevenBit/_Internal/ServiceProvider.hpp"
#include "SevenBit/_Internal/ServiceProviderRoot.hpp"

namespace sb::internal
{
    INLINE ServiceProvider::ServiceProvider(IServiceProviderRoot &root, ServiceProviderOptions options)
        : _root(root), _options(options), _services(_options.strongDestructionOrder)
    {
    }

    INLINE IServiceProvider::Ptr ServiceProvider::createScope()
    {
        return std::unique_ptr<ServiceProvider>(new ServiceProvider{_root, _options});
    }

    INLINE void *ServiceProvider::getService(TypeId serviceTypeId)
    {
        if (auto list = getSingletons().getList(serviceTypeId))
        {
            return list->first()->get();
        }
        if (auto list = getScoped().getList(serviceTypeId))
        {
            return list->first()->get();
        }
        return createAndRegister(serviceTypeId);
    }

    INLINE void *ServiceProvider::getRequiredService(TypeId serviceTypeId)
    {
        if (auto service = getService(serviceTypeId))
        {
            return service;
        }
        throw ServiceNotFoundException{serviceTypeId,
                                       "Service was not registered or was registered as transient service"};
    }

    INLINE std::vector<void *> ServiceProvider::getServices(TypeId serviceTypeId)
    {
        if (auto list = getSingletons().getList(serviceTypeId); list && list->isSealed())
        {
            return list->getAllServices();
        }
        if (auto list = getScoped().getList(serviceTypeId); list && list->isSealed())
        {
            return list->getAllServices();
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
        throw ServiceNotFoundException{serviceTypeId,
                                       "Service was not registered or was registered as singleton/scoped service"};
    }

    INLINE std::vector<void *> ServiceProvider::createServices(TypeId serviceTypeId)
    {
        return createAll(serviceTypeId);
    }

    INLINE void ServiceProvider::clear() { _services.clear(); }

    INLINE void *ServiceProvider::createAndRegister(TypeId serviceTypeId)
    {
        if (auto descriptor = getDescriptorsMap().getDescriptorsList(serviceTypeId))
        {
            return createAndRegister(descriptor->last());
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
        if (auto descriptor = getDescriptorsMap().getDescriptorsList(serviceTypeId))
        {
            return create(descriptor->last());
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
            return nullptr;
        }
        auto instance = createInstance(descriptor);
        auto &servicesMap = lifeTime.isSingleton() ? getSingletons() : getScoped();
        return servicesMap[descriptor.getServiceTypeId()].add(std::move(instance)).first()->get();
    }

    INLINE std::vector<void *> ServiceProvider::createAndRegisterAll(const ServiceDescriptorList &descriptors)
    {
        auto &lifeTime = descriptors.getLifeTime();
        if (lifeTime.isTransient())
        {
            return {};
        }
        auto &container = lifeTime.isSingleton() ? getSingletons() : getScoped();
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
        return serviceList->getAllServices();
    }

    INLINE void *ServiceProvider::create(const ServiceDescriptor &descriptor)
    {
        auto &lifetime = descriptor.getLifeTime();
        if (!lifetime.isTransient())
        {
            return nullptr;
        }
        auto instance = createInstance(descriptor);
        return instance->moveOut();
    }

    INLINE std::vector<void *> ServiceProvider::createAll(const ServiceDescriptorList &descriptors)
    {
        std::vector<void *> result;
        if (!descriptors.getLifeTime().isTransient())
        {
            return result;
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
        auto _ = _guard(descriptor.getImplementationTypeId());

        auto instance = descriptor.getImplementationFactory().createInstance(*this);

        if (instance && instance->isValid())
        {
            return instance;
        }
        throw InvalidServiceException{descriptor.getImplementationTypeId(), "Service instance is null or is invalid"};
    }

    INLINE const ServiceProviderOptions &ServiceProvider::getOptions() { return _options; }

    INLINE ServicesMap &ServiceProvider::getScoped() { return _services; }

    INLINE ServicesMap &ServiceProvider::getSingletons() { return _root.getSingletons(); }

    INLINE const ServiceDescriptorsMap &ServiceProvider::getDescriptorsMap() { return _root.getDescriptorsMap(); }

} // namespace sb::internal
