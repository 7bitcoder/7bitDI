#pragma once

#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/DefaultServiceProvider.hpp"
#include "SevenBit/DI/Details/IServiceProviderRoot.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"

namespace sb::di::details
{
    INLINE DefaultServiceProvider::DefaultServiceProvider(IServiceProviderRoot &root, ServiceProviderOptions options)
        : _root(root), _options(options), _services(_options.strongDestructionOrder)
    {
    }

    INLINE std::unique_ptr<IServiceProvider> DefaultServiceProvider::createScope()
    {
        return std::unique_ptr<DefaultServiceProvider>(new DefaultServiceProvider{_root, _options});
    }

    INLINE const IServiceInstance *DefaultServiceProvider::tryGetInstance(TypeId serviceTypeId)
    {
        if (auto list = getSingletons().getList(serviceTypeId))
        {
            return list->first().get();
        }
        if (auto list = getScoped().getList(serviceTypeId))
        {
            return list->first().get();
        }
        return createAndRegister(serviceTypeId);
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

    INLINE std::vector<const IServiceInstance *> DefaultServiceProvider::getInstances(TypeId serviceTypeId)
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

    INLINE IServiceInstance::Ptr DefaultServiceProvider::tryCreateInstance(TypeId serviceTypeId)
    {
        return create(serviceTypeId);
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

    INLINE std::vector<IServiceInstance::Ptr> DefaultServiceProvider::createInstances(TypeId serviceTypeId)
    {
        return createAll(serviceTypeId);
    }

    INLINE void DefaultServiceProvider::clear() { _services.clear(); }

    INLINE const IServiceInstance *DefaultServiceProvider::createAndRegister(TypeId serviceTypeId)
    {
        if (auto descriptor = getDescriptorsMap().getDescriptorsList(serviceTypeId))
        {
            return createAndRegister(descriptor->last());
        }
        return nullptr;
    }

    INLINE std::vector<const IServiceInstance *> DefaultServiceProvider::createAndRegisterAll(TypeId serviceTypeId)
    {
        if (auto descriptors = getDescriptorsMap().getDescriptorsList(serviceTypeId))
        {
            return createAndRegisterAll(*descriptors);
        }
        return {};
    }

    INLINE IServiceInstance::Ptr DefaultServiceProvider::create(TypeId serviceTypeId)
    {
        if (auto descriptor = getDescriptorsMap().getDescriptorsList(serviceTypeId))
        {
            return create(descriptor->last());
        }
        return nullptr;
    }

    INLINE std::vector<IServiceInstance::Ptr> DefaultServiceProvider::createAll(TypeId serviceTypeId)
    {
        if (auto descriptors = getDescriptorsMap().getDescriptorsList(serviceTypeId))
        {
            return createAll(*descriptors);
        }
        return {};
    }

    INLINE const IServiceInstance *DefaultServiceProvider::createAndRegister(const ServiceDescriptor &descriptor)
    {
        auto &lifeTime = descriptor.getLifeTime();
        if (lifeTime.isTransient())
        {
            return nullptr;
        }
        auto instance = createInstance(descriptor);
        auto &servicesMap = lifeTime.isSingleton() ? getSingletons() : getScoped();
        return servicesMap[descriptor.getServiceTypeId()].add(std::move(instance)).first().get();
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
        for (auto it = ++descriptors.rBegin(); it != descriptors.rEnd(); ++it) // skip main service
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
        std::vector<IServiceInstance::Ptr> result;
        if (!descriptors.getLifeTime().isTransient())
        {
            return result;
        }
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

    INLINE const ServiceProviderOptions &DefaultServiceProvider::getOptions() { return _options; }

    INLINE ServicesMap &DefaultServiceProvider::getScoped() { return _services; }

    INLINE ServicesMap &DefaultServiceProvider::getSingletons() { return _root.getSingletons(); }

    INLINE const ServiceDescriptorsMap &DefaultServiceProvider::getDescriptorsMap()
    {
        return _root.getDescriptorsMap();
    }

} // namespace sb::di::details
