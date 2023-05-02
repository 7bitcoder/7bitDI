#pragma once

#include <memory>
#include <vector>

#include "SevenBit/IServiceProvider.hpp"
#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/_Internal/CircularDependencyGuard.hpp"
#include "SevenBit/_Internal/Exceptions.hpp"
#include "SevenBit/_Internal/IServiceInstance.hpp"
#include "SevenBit/_Internal/ServiceDescriber.hpp"
#include "SevenBit/_Internal/ServiceDescriptor.hpp"
#include "SevenBit/_Internal/ServiceDescriptorList.hpp"
#include "SevenBit/_Internal/ServiceDescriptorsMap.hpp"
#include "SevenBit/_Internal/ServiceProviderOptions.hpp"
#include "SevenBit/_Internal/ServicesMap.hpp"
#include "SevenBit/_Internal/TypeId.hpp"

namespace sb
{
    class ServiceProvider final : public IServiceProvider
    {
      public:
        using Ptr = std::unique_ptr<ServiceProvider>;

      private:
        ServiceProvider *_root = nullptr;
        ServiceDescriptorsMap::Ptr _descriptorsMap;
        ServiceProviderOptions _options;
        ServicesMap _services;
        CircularDependencyGuard _guard;

        ServiceProvider(ServiceProvider *root) : _root(root) {}

      public:
        template <class TDescriptorIt>
        ServiceProvider(TDescriptorIt begin, TDescriptorIt end, ServiceProviderOptions options = {})
            : _descriptorsMap(std::make_unique<ServiceDescriptorsMap>(begin, end)), _options(std::move(options)),
              _root(this)
        {
            _descriptorsMap->add(
                ServiceDescriber::describeScoped([](IServiceProvider &provider) { return &provider; }));
        }

        ServiceProvider(const ServiceProvider &) = delete;
        ServiceProvider(ServiceProvider &&) = delete;

        ServiceProvider &operator=(const ServiceProvider &) = delete;
        ServiceProvider &operator=(ServiceProvider &&) = delete;

        IServiceProvider::Ptr createScope() { return std::unique_ptr<ServiceProvider>(new ServiceProvider{_root}); }

        void *getService(TypeId serviceTypeId) final
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

        void *getRequiredService(TypeId serviceTypeId)
        {
            if (auto service = getService(serviceTypeId))
            {
                return service;
            }
            throw ServiceNotRegisteredException{serviceTypeId};
        }

        std::vector<void *> getServices(TypeId serviceTypeId) final
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

        void *createService(TypeId serviceTypeId) { return create(serviceTypeId); }

        void *createRequiredService(TypeId serviceTypeId)
        {
            if (auto service = createService(serviceTypeId))
            {
                return service;
            }
            throw ServiceNotRegisteredException{serviceTypeId};
        }

        std::vector<void *> createServices(TypeId serviceTypeId) { return createAll(serviceTypeId); }

      private:
        void *createAndRegister(TypeId typeId)
        {
            if (auto descriptor = getDescriptorsMap().getDescriptor(typeId))
            {
                return createAndRegister(*descriptor);
            }
            return nullptr;
        }

        std::vector<void *> createAndRegisterAll(TypeId typeId)
        {
            if (auto descriptors = getDescriptorsMap().getDescriptorsList(typeId))
            {
                return createAndRegisterAll(*descriptors);
            }
            return {};
        }

        void *create(TypeId typeId)
        {
            if (auto descriptor = getDescriptorsMap().getDescriptor(typeId))
            {
                return create(*descriptor);
            }
            return nullptr;
        }

        std::vector<void *> createAll(TypeId typeId)
        {
            if (auto descriptors = getDescriptorsMap().getDescriptorsList(typeId))
            {
                return createAll(*descriptors);
            }
            return {};
        }

        void *createAndRegister(const ServiceDescriptor &descriptor)
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

        std::vector<void *> createAndRegisterAll(const ServiceDescriptorList &descriptors)
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

        void *create(const ServiceDescriptor &descriptor)
        {
            auto &lifetime = descriptor.getLifeTime();
            if (!lifetime.isTransient())
            {
                throw NotTransientException{descriptor.getServiceTypeId()};
            }
            auto instance = createInstance(descriptor);
            return instance->moveOut();
        }

        std::vector<void *> createAll(const ServiceDescriptorList &descriptors)
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

        IServiceInstance::Ptr createInstance(const ServiceDescriptor &descriptor)
        {
            auto scopedGuard = _guard.spawnGuard(descriptor.getImplementationTypeId());

            auto instance = descriptor.getImplementationFactory().createInstance(*this);

            if (instance && instance->isValid())
            {
                return instance;
            }
            throw ConstructionException{descriptor.getServiceTypeId()};
        }

        const ServiceDescriptorsMap &getDescriptorsMap()
        {
            if (_root && _root->_descriptorsMap)
            {
                return *_root->_descriptorsMap;
            }
            // todo fix
            throw ServiceCreatorProviderNotSet{};
        }

        ServicesMap &scoped() { return _services; }

        ServicesMap &singletons()
        {
            if (_root)
            {
                return _root->_services;
            }
            throw ServiceCreatorProviderNotSet{};
            // todo throw std::runtime_error("wrong service provider configuration");
        }

        bool isRoot() { return this == _root; }
    };
} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#endif
