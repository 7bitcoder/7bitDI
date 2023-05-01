#pragma once

#include <memory>
#include <vector>

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/_Internal/CircularDependencyGuard.hpp"
#include "SevenBit/_Internal/Exceptions.hpp"
#include "SevenBit/_Internal/IServiceCreatorsProvider.hpp"
#include "SevenBit/_Internal/IServiceHolder.hpp"
#include "SevenBit/_Internal/IServiceInstance.hpp"
#include "SevenBit/_Internal/ServiceDescriptor.hpp"
#include "SevenBit/_Internal/ServiceDescriptorList.hpp"
#include "SevenBit/_Internal/ServiceDescriptorsMap.hpp"
#include "SevenBit/_Internal/ServiceOwner.hpp"
#include "SevenBit/_Internal/ServiceProviderOptions.hpp"
#include "SevenBit/_Internal/ServicesContainer.hpp"
#include "SevenBit/_Internal/ServicesMap.hpp"

namespace sb
{
    class ServiceCollection;

    class ServiceProvider2
    {
      public:
        using ExternalServices = std::vector<IServiceHolder::Ptr>;
        using Ptr = std::unique_ptr<ServiceProvider>;

      private:
        ServiceProvider2 *_root = nullptr;
        ServiceDescriptorsMap::Ptr _descriptorsMap;
        ServiceProviderOptions _options;
        ServicesMap _services;
        CircularDependencyGuard _guard;

        ServiceProvider2(ServiceProvider2 *root) : _root(root) {}

      public:
        template <class TDescriptorIt>
        ServiceProvider2(TDescriptorIt begin, TDescriptorIt end, ServiceProviderOptions options = {})
            : _descriptorsMap(std::make_unique<ServiceDescriptorsMap>(begin, end)), _options(std::move(options)),
              _root(this)
        {
        }

        ServiceProvider2(const ServiceProvider2 &) = delete;
        ServiceProvider2(ServiceProvider2 &&) = delete;

        ServiceProvider2 &operator=(const ServiceProvider2 &) = delete;
        ServiceProvider2 &operator=(ServiceProvider2 &&) = delete;

        ServiceProvider2 createScoped() { return ServiceProvider2{_root}; }

        template <class I> I &getRequiredService()
        {
            if (auto ptr = getService<I>())
            {
                return *ptr;
            }
            throw ServiceNotRegisteredException{typeid(I)};
        }

        template <class I> I *getService() { return (I *)getService(typeid(I)); }

        void *getService(TypeId typeId)
        {
            if (auto list = singeletons().getList(typeId))
            {
                return list->at(0);
            }
            if (auto list = scoped().getList(typeId))
            {
                return list->at(0);
            }
            if (typeId == typeid(ServiceProvider))
            {
                return this;
            }
            return createAndRegister(typeId);
        }

        template <class I> std::vector<I *> getServices()
        {
            auto services = getServices(typeid(I));
            std::vector<I *> result;
            result.reserve(services.size());
            for (auto ptr : services)
            {
                result.push_back((I *)ptr);
            }
            return result;
        }

        std::vector<void *> getServices(TypeId typeId)
        {
            if (auto list = singeletons().getList(typeId); list && list->isSealed())
            {
                return list->getAll();
            }
            if (auto list = scoped().getList(typeId); list && list->isSealed())
            {
                return list->getAll();
            }
            if (typeId == typeid(ServiceProvider))
            {
                return {this};
            }
            return createAndRegisterAll(typeId);
        }

        template <class I> std::unique_ptr<I> createService() { return createUnique<I>(); }

        template <class I> std::vector<std::unique_ptr<I>> createServices() { return createAllUnique<I>(); }

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

        template <class I> std::unique_ptr<I> createUnique()
        {
            if (auto descriptor = getDescriptorsMap().getDescriptor(typeid(I)))
            {
                return createUnique<I>(*descriptor);
            }
            return nullptr;
        }

        template <class I> std::vector<std::unique_ptr<I>> createAllUnique()
        {
            if (auto descriptors = getDescriptorsMap().getDescriptorsList(typeid(I)))
            {
                return createAllUnique<I>(*descriptors);
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
            auto instance = createServiceInstance(descriptor);
            auto &servicesMap = lifeTime.isSingeleton() ? singeletons() : scoped();
            return servicesMap[descriptor.getServiceTypeId()].add(std::move(instance)).at(0);
        }

        std::vector<void *> createAndRegisterAll(const ServiceDescriptorList &descriptors)
        {
            auto &lifeTime = descriptors.getLifeTime();
            if (lifeTime.isTransient())
            {
                throw TransientForbidException{descriptors.last().getServiceTypeId()};
            }
            auto &container = lifeTime.isSingeleton() ? singeletons() : scoped();
            ServiceList *serviceList = container.getList(descriptors.getServiceTypeId());

            if (!serviceList)
            {
                serviceList = &container[descriptors.getServiceTypeId()].add(createServiceInstance(descriptors.last()));
            }
            serviceList->reserve(descriptors.size());
            for (auto it = ++descriptors.begin(); it != descriptors.end(); ++it) // skip main service
            {
                serviceList->add(createServiceInstance(*it));
            }
            serviceList->seal();
            return serviceList->getAll();
        }

        template <class I> std::unique_ptr<I> createUnique(const ServiceDescriptor &descriptor)
        {
            auto &lifetime = descriptor.getLifeTime();
            if (!lifetime.isTransient())
            {
                throw NotTransientException{descriptor.getServiceTypeId()};
            }
            auto instance = createServiceInstance(descriptor);
            return std::unique_ptr<I>((I *)instance->moveOut());
        }

        template <class I> std::vector<std::unique_ptr<I>> createAllUnique(const ServiceDescriptorList &descriptors)
        {
            std::vector<std::unique_ptr<I>> result;
            if (!descriptors.getLifeTime().isTransient())
            {
                throw NotTransientException{typeid(I)};
            }
            result.reserve(descriptors.size());
            for (auto &descriptor : descriptors)
            {
                result.push_back(createUnique<I>(descriptor));
            }
            return result;
        }

        IServiceInstance::Ptr createServiceInstance(const ServiceDescriptor &descriptor)
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
            // todo throw ServiceCreatorProviderNotSet{};
        }

        ServicesMap &scoped() { return _services; }

        ServicesMap &singeletons()
        {
            if (_root)
            {
                return _root->_services;
            }
            // todo throw std::runtime_error("wrong service provider configuration");
        }

        bool isRoot() { return this == _root; }
    };
} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/ServiceProvider.hpp"
#endif
