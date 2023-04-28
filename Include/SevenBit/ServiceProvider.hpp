#pragma once

#include <memory>
#include <vector>

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/_Internal/CircularDependencyGuard.hpp"
#include "SevenBit/_Internal/Exceptions.hpp"
#include "SevenBit/_Internal/IServiceCreatorsProvider.hpp"
#include "SevenBit/_Internal/IServiceHolder.hpp"
#include "SevenBit/_Internal/ServiceOwner.hpp"
#include "SevenBit/_Internal/ServicesContainer.hpp"

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

        ServicesContainer::Ptr _singletons;
        ServicesContainer _scoped;

        CircularDependencyGuard _guard;

        ServiceProvider(const IServiceCreatorsProvider *creatorsProvider, ServiceProvider *parent = nullptr);

      public:
        ServiceProvider(const ServiceProvider &) = delete;
        ServiceProvider(ServiceProvider &&) = default;

        ServiceProvider &operator=(const ServiceProvider &) = delete;
        ServiceProvider &operator=(ServiceProvider &&) = default;

        ServiceProvider createScoped();

        template <class I> I &getRequiredService()
        {
            if (auto ptr = getService<I>())
            {
                return *ptr;
            }
            throw ServiceNotRegisteredException{typeid(I)};
        }

        template <class I> I *getService() { return (I *)getService(typeid(I)); }

        void *getService(TypeId typeId);

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

        std::vector<void *> getServices(TypeId typeId);

        template <class I> std::unique_ptr<I> createService() { return createUnique<I>(); }

        template <class I> std::vector<std::unique_ptr<I>> createServices() { return createAllUnique<I>(); }

      private:
        void *createAndRegister(TypeId typeId);

        std::vector<void *> createAndRegisterAll(TypeId typeId);

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

        void *createAndRegister(const IServiceCreator &creator);

        std::vector<void *> createAndRegisterAll(const ServiceCreators &creators);

        template <class I> std::unique_ptr<I> createUnique(const IServiceCreator &creator)
        {
            auto &scope = creator.getServiceScope();
            if (!scope.isTransient())
            {
                throw NotTransientException{creator.getServiceTypeId()};
            }
            auto holder = createHolder(creator);
            return std::unique_ptr<I>((I *)holder->moveOutService());
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

        IServiceHolder::Ptr createHolder(const IServiceCreator &creator);

        const IServiceCreatorsProvider &creatorsProvider();

        ServicesContainer &scoped();

        ServicesContainer &singeletons();
    };
} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/ServiceProvider.hpp"
#endif
