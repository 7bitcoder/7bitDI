#pragma once
#include <memory>
#include <typeindex>
#include <unordered_set>

#include "SevenBit/_Internal/ExternalServiceFactory.hpp"
#include "SevenBit/_Internal/ExternalServiceFcnFactory.hpp"
#include "SevenBit/_Internal/ServiceCtorFactory.hpp"
#include "SevenBit/_Internal/ServiceDescriptor.hpp"
#include "SevenBit/_Internal/ServiceFcnFactory.hpp"
#include "SevenBit/_Internal/ServiceLifeTime.hpp"

namespace sb
{
    class ServiceDescriber
    {
      public:
        template <class TService, class TImplementation = TService>
        static ServiceDescriptor describe(TImplementation *service)
        {
            inheritCheck<TService, TImplementation>();
            auto factory = std::make_unique<ExternalServiceFactory<TImplementation>>(service);
            return describe<TService>(ServiceLifeTime::singeleton(), std::move(factory));
        }

        template <class TService, class TImplementation = TService>
        static ServiceDescriptor describe(ServiceLifeTime lifetime,
                                          std::function<TImplementation *(ServiceProvider &)> factoryFcn)
        {
            inheritCheck<TService, TImplementation>();
            auto factory = std::make_unique<ExternalServiceFcnFactory<TImplementation>>(std::move(factoryFcn));
            return describe<TService>(lifetime, std::move(factory));
        }

        template <class TService, class TImplementation = TService>
        static ServiceDescriptor describe(ServiceLifeTime lifetime,
                                          std::function<std::unique_ptr<TImplementation>(ServiceProvider &)> factoryFcn)
        {
            inheritCheck<TService, TImplementation>();
            auto factory = std::make_unique<ServiceFcnFactory<TImplementation>>(std::move(factoryFcn));
            return describe<TService>(lifetime, std::move(factory));
        }

        template <class TService, class TImplementation = TService>
        static ServiceDescriptor describe(ServiceLifeTime lifetime)
        {
            inheritCheck<TService, TImplementation>();
            auto factory = std::make_unique<ServiceCtorFactory<TImplementation>>();
            return describe<TService>(lifetime, std::move(factory));
        }

        template <class TService>
        static ServiceDescriptor describe(ServiceLifeTime lifetime, IServiceFactory::Ptr implementationFactory)
        {
            return ServiceDescriptor(typeid(TService), lifetime, std::move(implementationFactory));
        }

      private:
        template <class TService, class TImplementation> static void inheritCheck()
        {
            static_assert(std::is_base_of_v<TService, TImplementation>,
                          "Type TImplementation must inherit from TService");
        }
    };
} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/ScopedGuard.hpp"
#endif