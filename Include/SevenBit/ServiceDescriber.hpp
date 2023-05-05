#pragma once

#include <type_traits>

#include "SevenBit/IServiceProvider.hpp"
#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/Exceptions.hpp"
#include "SevenBit/ServiceDescriptor.hpp"
#include "SevenBit/ServiceLifeTime.hpp"
#include "SevenBit/_Internal/ExternalServiceFactory.hpp"
#include "SevenBit/_Internal/ServiceCtorFactory.hpp"
#include "SevenBit/_Internal/ServiceFcnFactory.hpp"
#include "SevenBit/_Internal/Utils.hpp"

namespace sb
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
            internal::utils::inheritCheck<TService, TImplementation>();
            auto factory = std::make_unique<internal::ServiceCtorFactory<TImplementation>>();
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
            internal::utils::inheritCheck<TService, TImplementation>();
            auto factory = std::make_unique<internal::ExternalServiceFactory<TImplementation>>(service);
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
            using FactoryType = internal::ServiceFcnFactory<FactoryFcn>;
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
} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#endif