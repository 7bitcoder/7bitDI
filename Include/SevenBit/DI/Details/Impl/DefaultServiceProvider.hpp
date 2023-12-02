#pragma once

#include <memory>
#include <utility>

#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/DefaultServiceProvider.hpp"
#include "SevenBit/DI/Details/IServiceProviderRoot.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"

namespace sb::di::details
{
    INLINE DefaultServiceProvider::DefaultServiceProvider(const DefaultServiceProvider &provider)
        : _sharedCore(utils::Assert::ptrAndGet(provider._sharedCore)),
          _scoped(_sharedCore->getOptions().strongDestructionOrder)
    {
    }

    INLINE DefaultServiceProvider::DefaultServiceProvider(IServiceProviderCore::Ptr core)
        : _sharedCore(utils::Assert::ptrAndGet(std::move(core))),
          _scoped(_sharedCore->getOptions().strongDestructionOrder)
    {
        if (_sharedCore->getOptions().prebuildSingletons)
        {
            prebuildSingletons();
        }
    }

    INLINE ServiceProvider::Ptr DefaultServiceProvider::createScope()
    {
        return ServiceProvider::Ptr{new DefaultServiceProvider{*this}};
    }

    INLINE const IServiceInstance &DefaultServiceProvider::getInstance(TypeId serviceTypeId)
    {
        auto service = tryGetInstance(serviceTypeId);
        return service ? *service
                       : throw ServiceNotFoundException{
                             serviceTypeId, "Service was not registered or was registered as transient service"};
    }

    INLINE const IServiceInstance *DefaultServiceProvider::tryGetInstance(TypeId serviceTypeId)
    {
        if (auto services = findRegisteredServices(serviceTypeId))
        {
            return services->last().get();
        }
        auto descriptors = findDescriptors(serviceTypeId);
        return descriptors ? tryCreateAndRegister(*descriptors) : nullptr;
    }

    INLINE const OneOrList<IServiceInstance::Ptr> *DefaultServiceProvider::tryGetInstances(TypeId serviceTypeId)
    {
        auto services = findRegisteredServices(serviceTypeId);
        if (!services)
        {
            auto descriptors = findDescriptors(serviceTypeId);
            return descriptors ? tryCreateAndRegisterAll(*descriptors) : nullptr;
        }
        if (!services->isSealed())
        {
            auto descriptors = findDescriptors(serviceTypeId);
            return descriptors ? &fillServicesWithAll(*descriptors, *services) : nullptr;
        }
        return services;
    }

    INLINE IServiceInstance::Ptr DefaultServiceProvider::createInstance(TypeId serviceTypeId)
    {
        auto service = tryCreateInstance(serviceTypeId);
        return service ? std::move(service)
                       : throw ServiceNotFoundException{
                             serviceTypeId, "Service was not registered or was registered as singleton/scoped service"};
    }

    INLINE IServiceInstance::Ptr DefaultServiceProvider::tryCreateInstance(TypeId serviceTypeId)
    {
        auto descriptors = findDescriptors(serviceTypeId);
        return descriptors ? tryCreate(*descriptors) : nullptr;
    }

    INLINE std::optional<OneOrList<IServiceInstance::Ptr>> DefaultServiceProvider::tryCreateInstances(
        TypeId serviceTypeId)
    {
        auto descriptors = findDescriptors(serviceTypeId);
        return descriptors ? tryCreateAll(*descriptors) : std::nullopt;
    }

    INLINE void DefaultServiceProvider::clear() { _scoped.clear(); }

    INLINE const IServiceInstance *DefaultServiceProvider::tryCreateAndRegister(
        const ServiceDescriptorList &descriptors)
    {
        if (auto servicesMap = tryGetServicesMap(descriptors.getLifeTime()))
        {
            auto &services = servicesMap->insert(descriptors.getServiceTypeId(), createInstance(descriptors.last()));
            if (descriptors.size() == 1)
            {
                services.seal();
            }
            return services.last().get();
        }
        return nullptr;
    }

    INLINE const ServiceList *DefaultServiceProvider::tryCreateAndRegisterAll(const ServiceDescriptorList &descriptors)
    {
        if (auto servicesMap = tryGetServicesMap(descriptors.getLifeTime()))
        {
            auto &services = servicesMap->insert(descriptors.getServiceTypeId(), createInstance(descriptors.last()));
            return &fillServicesWithAll(descriptors, services);
        }
        return nullptr;
    }

    INLINE ServiceList &DefaultServiceProvider::fillServicesWithAll(const ServiceDescriptorList &descriptors,
                                                                    ServiceList &services)
    {
        if (descriptors.size() > 1)
        {
            services.reserve(descriptors.size());
            auto realFirst = createInstance(descriptors.first());
            auto end = --descriptors.getAsList().end();
            for (auto it = ++descriptors.getAsList().begin(); it != end; ++it) // skip first and last
            {
                services.add(createInstance(*it));
            }
            services.add(std::move(realFirst));
            services.first().swap(services.last());
        }
        services.seal();
        return services;
    }

    INLINE IServiceInstance::Ptr DefaultServiceProvider::tryCreate(const ServiceDescriptorList &descriptors)
    {
        return descriptors.getLifeTime().isTransient() ? createInstance(descriptors.last()) : nullptr;
    }

    INLINE std::optional<OneOrList<IServiceInstance::Ptr>> DefaultServiceProvider::tryCreateAll(
        const ServiceDescriptorList &descriptors)
    {
        if (descriptors.getLifeTime().isTransient())
        {
            ServiceList services{createInstance(descriptors.last())};
            fillServicesWithAll(descriptors, services);
            return services;
        }
        return std::nullopt;
    }

    INLINE IServiceInstance::Ptr DefaultServiceProvider::createInstance(const ServiceDescriptor &descriptor)
    {
        auto _ = _guard(descriptor.getImplementationTypeId());
        auto instance = descriptor.getImplementationFactory().createInstance(*this);
        return instance && instance->isValid()
                   ? std::move(instance)
                   : throw InvalidServiceException{descriptor.getImplementationTypeId(),
                                                   "Service instance is null or is invalid"};
    }

    INLINE const ServiceProviderOptions &DefaultServiceProvider::getOptions() { return _sharedCore->getOptions(); }

    INLINE ServiceList *DefaultServiceProvider::findRegisteredServices(TypeId serviceTypeId)
    {
        auto singletons = _sharedCore->getSingletons().findServices(serviceTypeId);
        return singletons ? singletons : _scoped.findServices(serviceTypeId);
    }

    INLINE ServicesMap *DefaultServiceProvider::tryGetServicesMap(const ServiceLifeTime &lifeTime)
    {
        return lifeTime.isTransient() ? nullptr : (lifeTime.isSingleton() ? &_sharedCore->getSingletons() : &_scoped);
    }

    INLINE const ServiceDescriptorList *DefaultServiceProvider::findDescriptors(TypeId serviceTypeId) const
    {
        return _sharedCore->getDescriptorsMap().findDescriptors(serviceTypeId);
    }

    INLINE void DefaultServiceProvider::prebuildSingletons()
    {
        for (auto &[serviceType, descriptors] : _sharedCore->getDescriptorsMap())
        {
            if (descriptors.getLifeTime().isSingleton())
            {
                tryGetInstances(serviceType);
            }
        }
    }
} // namespace sb::di::details
