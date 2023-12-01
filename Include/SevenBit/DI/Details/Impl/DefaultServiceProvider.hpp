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
        if (auto service = tryGetInstance(serviceTypeId))
        {
            return *service;
        }
        throw ServiceNotFoundException{serviceTypeId,
                                       "Service was not registered or was registered as transient service"};
    }

    INLINE const IServiceInstance *DefaultServiceProvider::tryGetInstance(TypeId serviceTypeId)
    {
        if (auto services = getSingletons().getList(serviceTypeId))
        {
            return services->first().get();
        }
        if (auto services = getScoped().getList(serviceTypeId))
        {
            return services->first().get();
        }
        return tryCreateAndRegisterMain(serviceTypeId);
    }

    INLINE std::vector<const IServiceInstance *> DefaultServiceProvider::getInstances(TypeId serviceTypeId)
    {
        if (auto services = getSingletons().getList(serviceTypeId))
        {
            return services->isSealed() ? services->getAllServices() : registerAll(serviceTypeId, *services);
        }
        if (auto services = getScoped().getList(serviceTypeId))
        {
            return services->isSealed() ? services->getAllServices() : registerAll(serviceTypeId, *services);
        }
        return createAndRegisterAll(serviceTypeId);
    }

    INLINE IServiceInstance::Ptr DefaultServiceProvider::createInstance(TypeId serviceTypeId)
    {
        if (auto service = tryCreateInstance(serviceTypeId))
        {
            return service;
        }
        throw ServiceNotFoundException{serviceTypeId,
                                       "Service was not registered or was registered as singleton/scoped service"};
    }

    INLINE IServiceInstance::Ptr DefaultServiceProvider::tryCreateInstance(TypeId serviceTypeId)
    {
        if (auto descriptors = getDescriptorsMap().tryGetList(serviceTypeId))
        {
            return create(descriptors->last());
        }
        return nullptr;
    }

    INLINE std::vector<IServiceInstance::Ptr> DefaultServiceProvider::createInstances(TypeId serviceTypeId)
    {
        if (auto descriptors = getDescriptorsMap().tryGetList(serviceTypeId))
        {
            return createAll(*descriptors);
        }
        return {};
    }

    INLINE void DefaultServiceProvider::clear() { _scoped.clear(); }

    INLINE const IServiceInstance *DefaultServiceProvider::tryCreateAndRegisterMain(TypeId serviceTypeId)
    {
        if (auto descriptors = getDescriptorsMap().tryGetList(serviceTypeId))
        {
            return createAndRegisterMainFrom(*descriptors);
        }
        return nullptr;
    }

    INLINE std::vector<const IServiceInstance *> DefaultServiceProvider::createAndRegisterAll(TypeId serviceTypeId)
    {
        if (auto descriptors = getDescriptorsMap().tryGetList(serviceTypeId))
        {
            return createAndRegisterAllFrom(*descriptors);
        }
        return {};
    }

    INLINE std::vector<const IServiceInstance *> DefaultServiceProvider::registerAll(TypeId serviceTypeId,
                                                                                     ServiceList &services)
    {
        if (auto descriptors = getDescriptorsMap().tryGetList(serviceTypeId))
        {
            return fillServiceListFrom(*descriptors, services).getAllServices();
        }
        return {};
    }

    INLINE const IServiceInstance *DefaultServiceProvider::createAndRegisterMainFrom(
        const ServiceDescriptorList &descriptors)
    {
        if (auto servicesMap = tryGetServicesMapFor(descriptors.getLifeTime()))
        {
            auto &services = servicesMap->add(descriptors.getServiceTypeId(), createInstanceFrom(descriptors.last()));
            if (descriptors.size() == 1)
            {
                services.seal();
            }
            return services.last().get();
        }
        return nullptr;
    }

    INLINE std::vector<const IServiceInstance *> DefaultServiceProvider::createAndRegisterAllFrom(
        const ServiceDescriptorList &descriptors)
    {
        if (auto servicesMap = tryGetServicesMapFor(descriptors.getLifeTime()))
        {
            auto size = descriptors.size();
            auto &services = servicesMap->create(descriptors.getServiceTypeId(), size);
            for (auto i = 0; i < size; ++i)
            {
                services.add(createInstanceFrom(descriptors[i]));
            }
            return services.getAllServices();
        }
        return {};
    }

    INLINE ServiceList &DefaultServiceProvider::fillServiceListFrom(const ServiceDescriptorList &descriptors,
                                                                    ServiceList &services)
    {
        auto size = descriptors.size();
        if (size > 1)
        {
            services.reserve(size);
            auto first = createInstanceFrom(descriptors[0]);
            for (auto i = 1; i < size - 1; ++i)
            {
                services.add(createInstanceFrom(descriptors[i]));
            }
            services.add(std::move(first));
            services.first().swap(services.last());
        }
        services.seal();
        return services;
    }

    INLINE IServiceInstance::Ptr DefaultServiceProvider::create(const ServiceDescriptor &descriptor)
    {
        if (descriptor.getLifeTime().isTransient())
        {
            return createInstanceFrom(descriptor);
        }
        return nullptr;
    }

    INLINE std::vector<IServiceInstance::Ptr> DefaultServiceProvider::createAll(
        const ServiceDescriptorList &descriptors)
    {
        std::vector<IServiceInstance::Ptr> result;
        if (descriptors.getLifeTime().isTransient())
        {
            result.reserve(descriptors.size());
            for (auto &descriptor : descriptors)
            {
                result.emplace_back(createInstanceFrom(descriptor));
            }
        }
        return result;
    }

    INLINE IServiceInstance::Ptr DefaultServiceProvider::createInstanceFrom(const ServiceDescriptor &descriptor)
    {
        auto _ = _guard(descriptor.getImplementationTypeId());

        auto instance = descriptor.getImplementationFactory().createInstance(*this);

        if (instance && instance->isValid())
        {
            return instance;
        }
        throw InvalidServiceException{descriptor.getImplementationTypeId(), "Service instance is null or is invalid"};
    }

    INLINE const ServiceProviderOptions &DefaultServiceProvider::getOptions() { return _sharedCore->getOptions(); }

    INLINE ServicesMap *DefaultServiceProvider::tryGetServicesMapFor(const ServiceLifeTime &lifeTime)
    {
        if (lifeTime.isTransient())
        {
            return nullptr;
        }
        return lifeTime.isSingleton() ? &getSingletons() : &getScoped();
    }

    INLINE ServicesMap &DefaultServiceProvider::getScoped() { return _scoped; }

    INLINE ServicesMap &DefaultServiceProvider::getSingletons() { return _sharedCore->getSingletons(); }

    INLINE const ServiceDescriptorsMap &DefaultServiceProvider::getDescriptorsMap()
    {
        return _sharedCore->getDescriptorsMap();
    }

    INLINE void DefaultServiceProvider::prebuildSingletons()
    {
        for (auto &[serviceType, list] : getDescriptorsMap())
        {
            if (list.getLifeTime().isSingleton())
            {
                getInstances(serviceType);
            }
        }
    }
} // namespace sb::di::details
