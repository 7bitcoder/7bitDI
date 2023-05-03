#pragma once

#include <type_traits>

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/Exceptions.hpp"
#include "SevenBit/ServiceDescriptor.hpp"
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
            using FactoryType = internal::ServiceFcnFactory<ReturnType, FactoryFcn>;
            using TImplementation = typename FactoryType::Type;
            internal::utils::inheritCheck<TService, TImplementation>();

            auto factory = std::make_unique<FactoryType>(lifetime, std::move(factoryFcn));

            return {typeid(TService), lifetime, std::move(factory)};
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
            using FactoryType = internal::ServiceFcnFactory<ReturnType, FactoryFcn>;
            using TImplementation = typename FactoryType::Type;

            auto factory = std::make_unique<FactoryType>(lifetime, std::move(factoryFcn));
            return {typeid(TImplementation), lifetime, std::move(factory)};
        }
    };
} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#endif