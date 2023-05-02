#pragma once

#include <type_traits>

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/ServiceDescriptor.hpp"
#include "SevenBit/_Internal/ExternalServiceFactory.hpp"
#include "SevenBit/_Internal/ExternalServiceFcnFactory.hpp"
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
            inheritCheck<TService, TImplementation>();
            auto factory = std::make_unique<internal::ServiceCtorFactory<TImplementation>>();
            return {typeid(TService), lifetime, std::move(factory)};
        }

        template <class TService, class FactoryFcn> static ServiceDescriptor describeSingleton(FactoryFcn factory)
        {
            return describe<TService, FactoryFcn>(ServiceLifeTime::singleton(), std::move(factory));
        }
        template <class TService, class FactoryFcn> static ServiceDescriptor describeScoped(FactoryFcn factory)
        {
            return describe<TService, FactoryFcn>(ServiceLifeTime::scoped(), std::move(factory));
        }
        template <class TService, class FactoryFcn> static ServiceDescriptor describeTransient(FactoryFcn factory)
        {
            return describe<TService, FactoryFcn>(ServiceLifeTime::transient(), std::move(factory));
        }

        template <class TService, class FactoryFcn>
        static ServiceDescriptor describe(ServiceLifeTime lifetime, FactoryFcn factoryFcn)
        {
            using ReturnType = std::invoke_result_t<FactoryFcn, IServiceProvider &>;
            if constexpr (sb::utils::IsUniquePtrV<ReturnType>)
            {
                using TImplementation = typename sb::utils::IsUniquePtr<ReturnType>::Type;
                return describeWithFactory<TService, TImplementation, FactoryFcn>(lifetime, std::move(factoryFcn));
            }
            else if constexpr (std::is_pointer_v<ReturnType>)
            {
                using TImplementation = typename std::remove_pointer<ReturnType>::type;
                return describeWithExternal<TService, TImplementation, FactoryFcn>(lifetime, std::move(factoryFcn));
            }
            else
            {
                unsupportedFactory<FactoryFcn>();
            }
        }

        template <class FactoryFcn> static ServiceDescriptor describeSingleton(FactoryFcn factory)
        {
            return describe(ServiceLifeTime::singleton(), std::move(factory));
        }
        template <class FactoryFcn> static ServiceDescriptor describeScoped(FactoryFcn factory)
        {
            return describe(ServiceLifeTime::scoped(), std::move(factory));
        }
        template <class FactoryFcn> static ServiceDescriptor describeTransient(FactoryFcn factory)
        {
            return describe(ServiceLifeTime::transient(), std::move(factory));
        }

        template <class FactoryFcn> static ServiceDescriptor describe(ServiceLifeTime lifetime, FactoryFcn factoryFcn)
        {
            using ReturnType = std::invoke_result_t<FactoryFcn, IServiceProvider &>;
            if constexpr (sb::utils::IsUniquePtrV<ReturnType>)
            {
                using TService = typename sb::utils::IsUniquePtr<ReturnType>::Type;
                return describeWithFactory<TService, TService, FactoryFcn>(lifetime, std::move(factoryFcn));
            }
            else if constexpr (std::is_pointer_v<ReturnType>)
            {
                using TService = typename std::remove_pointer<ReturnType>::type;
                return describeWithExternal<TService, TService, FactoryFcn>(lifetime, std::move(factoryFcn));
            }
            else
            {
                unsupportedFactory<FactoryFcn>();
            }
        }

      private:
        template <class FactoryFcn> static void unsupportedFactory()
        {
            static_assert(
                notSupportedFactory<FactoryFcn>,
                "Factory function should have this scheme: (IServiceProvider&) -> std::unique_ptr<T> | T * it "
                "should take as argument IServiceProvider& and return std::unique_ptr<T> or pointner T *");
        }

        template <class TService, class TImplementation, class FactoryFcn>
        static ServiceDescriptor describeWithExternal(ServiceLifeTime lifetime, FactoryFcn factoryFcn)
        {
            inheritCheck<TService, TImplementation>();
            nonTransientFactoryCheck(lifetime);
            auto factory = std::make_unique<internal::ExternalServiceFcnFactory<TImplementation, FactoryFcn>>(
                std::move(factoryFcn));

            return {typeid(TService), lifetime, std::move(factory)};
        }

        template <class TService, class TImplementation, class FactoryFcn>
        static ServiceDescriptor describeWithFactory(ServiceLifeTime lifetime, FactoryFcn factoryFcn)
        {
            inheritCheck<TService, TImplementation>();
            auto factory =
                std::make_unique<internal::ServiceFcnFactory<TImplementation, FactoryFcn>>(std::move(factoryFcn));

            return {typeid(TService), lifetime, std::move(factory)};
        }

        template <class... T> inline static constexpr bool notSupportedFactory = false;

        template <class TService, class TImplementation> static void inheritCheck()
        {
            static_assert(std::is_base_of_v<TService, TImplementation>,
                          "Type TImplementation must inherit from TService");
        }

        void static nonTransientFactoryCheck(const ServiceLifeTime &lifeTime)
        {
            if (lifeTime.isTransient())
            {
                // todo throw
            }
        }
    };
} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#endif