#pragma once

#include <type_traits>

#include "SevenBit/DI/IServiceProvider.hpp"
#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/ExternalServiceFactory.hpp"
#include "SevenBit/DI/Details/ServiceCtorFactory.hpp"
#include "SevenBit/DI/Details/ServiceFcnFactory.hpp"
#include "SevenBit/DI/Details/Utils.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"

namespace sb::di
{
    class ServiceDescriber
    {
      public:
        template <class TService, class TImplementation = TService> static ServiceDescriptor describeSingleton()
        {
            return describe<TService, TImplementation>(ServiceLifeTime::singleton());
        }
        template <class TService, class TImplementation = TService> static ServiceDescriptor describeScoped()
        {
            return describe<TService, TImplementation>(ServiceLifeTime::scoped());
        }
        template <class TService, class TImplementation = TService> static ServiceDescriptor describeTransient()
        {
            return describe<TService, TImplementation>(ServiceLifeTime::transient());
        }
        template <class TService, class TImplementation = TService>
        static ServiceDescriptor describe(ServiceLifeTime lifetime)
        {
            details::utils::inheritCheck<TService, TImplementation>();
            auto factory = std::make_unique<details::ServiceCtorFactory<TImplementation>>();
            return {typeid(TService), lifetime, std::move(factory)};
        }

        template <class TService, class TImplementation = TService>
        static ServiceDescriptor describeSingleton(TImplementation &service)
        {
            return describeSingleton<TService, TImplementation>(&service);
        }

        template <class TService, class TImplementation = TService>
        static ServiceDescriptor describeSingleton(TImplementation *service)
        {
            details::utils::inheritCheck<TService, TImplementation>();
            auto factory = std::make_unique<details::ExternalServiceFactory<TImplementation>>(service);
            return {typeid(TService), ServiceLifeTime::singleton(), std::move(factory)};
        }

        template <class TService, class FactoryFcn> static ServiceDescriptor describeSingletonFrom(FactoryFcn factory)
        {
            return describeFrom<TService, FactoryFcn>(ServiceLifeTime::singleton(), std::move(factory));
        }
        template <class TService, class FactoryFcn> static ServiceDescriptor describeScopedFrom(FactoryFcn factory)
        {
            return describeFrom<TService, FactoryFcn>(ServiceLifeTime::scoped(), std::move(factory));
        }
        template <class TService, class FactoryFcn> static ServiceDescriptor describeTransientFrom(FactoryFcn factory)
        {
            return describeFrom<TService, FactoryFcn>(ServiceLifeTime::transient(), std::move(factory));
        }

        template <class TService, class FactoryFcn>
        static ServiceDescriptor describeFrom(ServiceLifeTime lifetime, FactoryFcn factoryFcn)
        {
            return describeFromAutoFactory<TService, FactoryFcn>(lifetime, std::move(factoryFcn));
        }

        template <class FactoryFcn> static ServiceDescriptor describeSingletonFrom(FactoryFcn factory)
        {
            return describeFrom<void, FactoryFcn>(ServiceLifeTime::singleton(), std::move(factory));
        }
        template <class FactoryFcn> static ServiceDescriptor describeScopedFrom(FactoryFcn factory)
        {
            return describeFrom<void, FactoryFcn>(ServiceLifeTime::scoped(), std::move(factory));
        }
        template <class FactoryFcn> static ServiceDescriptor describeTransientFrom(FactoryFcn factory)
        {
            return describeFrom<void, FactoryFcn>(ServiceLifeTime::transient(), std::move(factory));
        }

        template <class FactoryFcn>
        static ServiceDescriptor describeFrom(ServiceLifeTime lifetime, FactoryFcn factoryFcn)
        {
            return describeFrom<void, FactoryFcn>(lifetime, std::move(factoryFcn));
        }

      private:
        template <class... T> inline static constexpr bool notSupportedFactory = false;

        template <class TService, class FactoryFcn>
        static ServiceDescriptor describeFromAutoFactory(ServiceLifeTime lifetime, FactoryFcn factoryFcn)
        {
            checkCopyAndMove<FactoryFcn>();
            if constexpr (std::is_invocable_v<FactoryFcn>)
            {
                return describeFromFactory<TService>(lifetime, [=](IServiceProvider &) { return factoryFcn(); });
            }
            else if constexpr (std::is_invocable_v<FactoryFcn, IServiceProvider &>)
            {
                return describeFromFactory<TService>(lifetime, std::move(factoryFcn));
            }
            else
            {
                badFactory<FactoryFcn>();
            }
        }

        template <class TService, class FactoryFcn>
        static ServiceDescriptor describeFromFactory(ServiceLifeTime lifetime, FactoryFcn factoryFcn)
        {
            using FactoryType = details::ServiceFcnFactory<FactoryFcn>;
            if constexpr (!FactoryType::IsReturnTypeUniquePtr::value)
            {
                badFactory<FactoryFcn>();
            }
            auto factory = std::make_unique<FactoryType>(std::move(factoryFcn));

            using Service =
                typename std::conditional<std::is_void_v<TService>, typename FactoryType::ServiceType, TService>::type;
            return {typeid(Service), lifetime, std::move(factory)};
        }

        template <class FactoryFcn> static void checkCopyAndMove()
        {
            if constexpr (!std::is_copy_constructible_v<FactoryFcn> || !std::is_move_constructible_v<FactoryFcn>)
            {
                static_assert(notSupportedFactory<FactoryFcn>, "Factory function should be movable and copyable.");
            }
        }

        template <class FactoryFcn> static void badFactory()
        {
            static_assert(notSupportedFactory<FactoryFcn>,
                          "Factory function should have this scheme: (IServiceProvider &) -> std::unique_ptr<T> or () "
                          "-> std::unique_ptr<T>, it should return std::unique_ptr<T> and optionally take as argument "
                          "IServiceProvider &");
        }
    };
} // namespace sb::di

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#endif