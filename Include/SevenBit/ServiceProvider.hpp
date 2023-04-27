#pragma once

#include <cstddef>
#include <exception>
#include <forward_list>
#include <list>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "SevenBit/_Internal/CircularDependencyGuard.hpp"
#include "SevenBit/_Internal/CtorReflection.hpp"
#include "SevenBit/_Internal/Exceptions.hpp"
#include "SevenBit/_Internal/IServiceCreatorsProvider.hpp"
#include "SevenBit/_Internal/IServiceHolder.hpp"
#include "SevenBit/_Internal/ServiceOwner.hpp"
#include "SevenBit/_Internal/ServiceScope.hpp"
#include "SevenBit/_Internal/ServicesContainer.hpp"
#include "SevenBit/_Internal/Utils.hpp"
#include "_Internal/IServiceCreatorsProvider.hpp"

namespace sb
{
    class ServiceCollection;

    class ServiceProvider
    {
      public:
        using ExternalServices = std::vector<IServiceHolder::Ptr>;
        using Ptr = std::unique_ptr<ServiceProvider>;

      private:
        friend class ServiceCollection;

        ServiceProvider *_parent = nullptr;
        const IServiceCreatorsProvider *_creatorsProvider = nullptr;

        ServiceContainer::Ptr _singletons;
        ServiceContainer _scoped;

        CircularDependencyGuard _guard;

        ServiceProvider(const IServiceCreatorsProvider *creatorsProvider, ServiceProvider *parent = nullptr)
            : _creatorsProvider{creatorsProvider}, _parent(parent)
        {
            if (!_parent)
            {
                _singletons = std::make_unique<ServiceContainer>();
            }
        }

      public:
        ServiceProvider(const ServiceProvider &) = delete;
        ServiceProvider(ServiceProvider &&) = default;

        ServiceProvider &operator=(const ServiceProvider &) = delete;
        ServiceProvider &operator=(ServiceProvider &&) = default;

        ServiceProvider createScoped() { return ServiceProvider{_creatorsProvider, this}; }

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
            if (auto creator = creatorsProvider().getMainCreator(typeId))
            {
                return createAndRegister(*creator);
            }
            return nullptr;
        }

        std::vector<void *> createAndRegisterAll(TypeId typeId)
        {
            if (auto creators = creatorsProvider().getCreators(typeId))
            {
                return createAndRegisterAll(*creators);
            }
            return {};
        }

        template <class I> std::unique_ptr<I> createUnique()
        {
            if (auto creator = creatorsProvider().getMainCreator(typeid(I)))
            {
                return createUnique<I>(*creator);
            }
            return nullptr;
        }

        template <class I> std::vector<std::unique_ptr<I>> createAllUnique()
        {
            if (auto creators = creatorsProvider().getCreators(typeid(I)))
            {
                return createAllUnique<I>(*creators);
            }
            return {};
        }

        void *createAndRegister(const IServiceCreator &creator)
        {
            auto &scope = creator.getServiceScope();
            if (scope.isTransient())
            {
                throw TransientForbidException{creator.getServiceTypeId()};
            }
            auto holder = createHolder(creator);
            auto &container = scope.isSingeleton() ? singeletons() : scoped();
            return container.addAndGetList(std::move(holder))->at(0);
        }

        std::vector<void *> createAndRegisterAll(const ServiceCreators &creators)
        {
            if (creators.getServicesScope().isTransient())
            {
                throw TransientForbidException{creators.getInterfaceTypeId()};
            }
            auto &container = creators.getServicesScope().isSingeleton() ? singeletons() : scoped();
            ServiceList *serviceList = container.getList(creators.getInterfaceTypeId());

            if (!serviceList)
            {
                serviceList = container.addAndGetList(createHolder(creators.getMainCreator()));
            }
            serviceList->reserve(creators.size());
            for (auto it = ++creators.begin(); it != creators.end(); ++it) // skip main service
            {
                serviceList->add(createHolder(**it));
            }
            serviceList->seal();
            return serviceList->getAll();
        }

        template <class I> std::unique_ptr<I> createUnique(const IServiceCreator &creator)
        {
            auto &scope = creator.getServiceScope();
            if (!scope.isTransient())
            {
                throw NotTransientException{creator.getServiceTypeId()};
            }
            auto holder = createHolder(creator);
            auto &casted = static_cast<ServiceOwner<I, I> &>(*holder);
            return casted.moveOutService();
        }

        template <class I> std::vector<std::unique_ptr<I>> createAllUnique(const ServiceCreators &creators)
        {
            std::vector<std::unique_ptr<I>> result;
            if (!creators.getServicesScope().isTransient())
            {
                throw NotTransientException{typeid(I)};
            }
            result.reserve(creators.size());
            for (auto &creator : creators)
            {
                result.push_back(createUnique<I>(*creator));
            }
            return result;
        }

        IServiceHolder::Ptr createHolder(const IServiceCreator &creator)
        {
            auto scopedGuard = _guard.spawnGuard(creator.getServiceTypeId());

            auto holder = creator.create(*this);

            if (holder && holder->isValid())
            {
                return holder;
            }
            throw ConstructionException{creator.getServiceTypeId()};
        }

        const IServiceCreatorsProvider &creatorsProvider()
        {
            if (!_creatorsProvider)
            {
                throw ServiceCreatorProviderNotSet{};
            }
            return *_creatorsProvider;
        }

        ServiceContainer &scoped() { return _scoped; }

        ServiceContainer &singeletons()
        {
            if (_singletons)
            {
                return *_singletons;
            }
            if (!_parent)
            {
                throw std::runtime_error("wrong service provider configuration");
            }
            return _parent->singeletons();
        }
    };

    namespace serviceExtractors
    {
        using SP = ServiceProvider;
        template <class T> struct R
        {
        };

        template <class T> auto get(R<T *>, SP &sp) { return &sp.getRequiredService<T>(); }
        template <class T> auto get(R<T *const>, SP &sp) { return &sp.getRequiredService<T>(); }
        template <class T> auto get(R<const T *>, SP &sp) { return &sp.getRequiredService<T>(); }
        template <class T> auto get(R<const T *const>, SP &sp) { return &sp.getRequiredService<T>(); }

        template <class T> auto &get(R<T &>, SP &sp) { return sp.getRequiredService<T>(); }
        template <class T> auto &get(R<const T &>, SP &sp) { return sp.getRequiredService<T>(); }

        template <class T> auto get(R<std::unique_ptr<T>>, SP &sp) { return sp.createService<T>(); }
        template <class T> auto get(R<const std::unique_ptr<T>>, SP &sp) { return sp.createService<T>(); }

        template <class T> auto get(R<std::vector<T *>>, SP &sp) { return sp.getServices<T>(); }
        template <class T> auto get(R<const std::vector<T *>>, SP &sp) { return sp.getServices<T>(); }

        template <class T> auto get(R<std::vector<std::unique_ptr<T>>>, SP &sp) { return sp.createServices<T>(); }
        template <class T> auto get(R<const std::vector<std::unique_ptr<T>>>, SP &sp) { return sp.createServices<T>(); }

        template <class T> auto get(R<std::vector<T>>, SP &)
        {
            static_assert(utils::notSupportedType<T>,
                          "Vector should contain pointners or unique pointners for transient services");
        }

        template <class T> T get(R<T>, SP &)
        {
            static_assert(utils::notSupportedType<T>, "Type is not supported as function augument parameter");
        }
    } // namespace serviceExtractors

    template <class T> T getService(ServiceProvider &provider)
    {
        return serviceExtractors::get(serviceExtractors::R<T>{}, provider);
    }
} // namespace sb
