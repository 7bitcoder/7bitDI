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
    INLINE DefaultServiceProvider::DefaultServiceProvider(IServiceProviderCore::SPtr core)
        : _sharedCore(utils::Assert::ptrAndGet(std::move(core))),
          _services(_sharedCore->getOptions().strongDestructionOrder)
    {
    }

    INLINE DefaultServiceProvider::DefaultServiceProvider(IServiceProviderCore::Ptr core)
        : DefaultServiceProvider(IServiceProviderCore::SPtr{std::move(core)})
    {
        if (_sharedCore->getOptions().prebuildSingletons)
        {
            prebuildSingletons();
        }
    }

    INLINE ServiceProvider::Ptr DefaultServiceProvider::createScope()
    {
        return ServiceProvider::Ptr{new DefaultServiceProvider{_sharedCore}};
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
        return createAndRegisterMain(serviceTypeId);
    }

    INLINE std::vector<const IServiceInstance *> DefaultServiceProvider::getInstances(TypeId serviceTypeId)
    {
        if (auto services = getSingletons().getList(serviceTypeId); services && services->isSealed())
        {
            return services->getAllServices();
        }
        if (auto services = getScoped().getList(serviceTypeId); services && services->isSealed())
        {
            return services->getAllServices();
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

    INLINE void DefaultServiceProvider::clear() { _services.clear(); }

    INLINE const IServiceInstance *DefaultServiceProvider::createAndRegisterMain(TypeId serviceTypeId)
    {
        if (auto descriptors = getDescriptorsMap().tryGetList(serviceTypeId))
        {
            return createAndRegisterMain(*descriptors);
        }
        return nullptr;
    }

    INLINE std::vector<const IServiceInstance *> DefaultServiceProvider::createAndRegisterAll(TypeId serviceTypeId)
    {
        if (auto descriptors = getDescriptorsMap().tryGetList(serviceTypeId))
        {
            return createAndRegisterAll(*descriptors);
        }
        return {};
    }

    INLINE const IServiceInstance *DefaultServiceProvider::createAndRegisterMain(
        const ServiceDescriptorList &descriptors)
    {
        auto &lifeTime = descriptors.getLifeTime();
        if (lifeTime.isTransient())
        {
            return nullptr;
        }
        auto instance = createInstance(descriptors.last());
        auto &servicesMap = lifeTime.isSingleton() ? getSingletons() : getScoped();
        auto &services = servicesMap[descriptors.getServiceTypeId()];
        auto instancePtr = services.add(std::move(instance)).first().get();
        if (descriptors.size() == 1)
        {
            services.seal();
        }
        return instancePtr;
    }

    INLINE std::vector<const IServiceInstance *> DefaultServiceProvider::createAndRegisterAll(
        const ServiceDescriptorList &descriptors)
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
            auto instance = createInstance(descriptors.last());
            serviceList = &container[descriptors.getServiceTypeId()].add(std::move(instance));
        }
        serviceList->reserve(descriptors.size());
        for (auto it = ++descriptors.rBegin(); it != descriptors.rEnd(); ++it) // skip main/last service
        {
            auto instance = createInstance(*it);
            serviceList->add(std::move(instance));
        }
        serviceList->seal();
        return serviceList->getAllServices();
    }

    INLINE IServiceInstance::Ptr DefaultServiceProvider::create(const ServiceDescriptor &descriptor)
    {
        auto &lifetime = descriptor.getLifeTime();
        if (!lifetime.isTransient())
        {
            return nullptr;
        }
        return createInstance(descriptor);
    }

    INLINE std::vector<IServiceInstance::Ptr> DefaultServiceProvider::createAll(
        const ServiceDescriptorList &descriptors)
    {
        if (!descriptors.getLifeTime().isTransient())
        {
            return {};
        }
        std::vector<IServiceInstance::Ptr> result;
        result.reserve(descriptors.size());
        for (auto &descriptor : descriptors)
        {
            result.emplace_back(createInstance(descriptor));
        }
        return result;
    }

    INLINE IServiceInstance::Ptr DefaultServiceProvider::createInstance(const ServiceDescriptor &descriptor)
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

    INLINE ServicesMap &DefaultServiceProvider::getScoped() { return _services; }

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
