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
        template <class TImplementation>
        using Factory = std::function<std::unique_ptr<TImplementation>(ServiceProvider &)>;
        template <class TImplementation> using ExternalProvider = std::function<TImplementation *(ServiceProvider &)>;

        template <class TService, class TImplementation = TService> static void describeSingleton()
        {
            return describe<TService, TImplementation>(ServiceLifeTime::singeleton());
        }
        template <class TService, class TImplementation = TService> static void describeScoped()
        {
            return describe<TService, TImplementation>(ServiceLifeTime::scoped());
        }
        template <class TService, class TImplementation = TService> static void describeTransient()
        {
            return describe<TService, TImplementation>(ServiceLifeTime::transient());
        }

        template <class TService, class TImplementation = TService>
        static void describeSingleton(Factory<TImplementation> factory)
        {
            return describe<TService, TImplementation>(ServiceLifeTime::singeleton(), std::move(factory));
        }
        template <class TService, class TImplementation = TService>
        static void describeScoped(Factory<TImplementation> factory)
        {
            return describe<TService, TImplementation>(ServiceLifeTime::scoped(), std::move(factory));
        }
        template <class TService, class TImplementation = TService>
        static void describeTransient(Factory<TImplementation> factory)
        {
            return describe<TService, TImplementation>(ServiceLifeTime::transient(), std::move(factory));
        }

        template <class TService, class TImplementation = TService>
        static void describeSingeleton(TImplementation &service)
        {
            return describeSingeleton<TService, TImplementation>(&service);
        }
        template <class TService, class TImplementation = TService>
        static void describeSingeleton(TImplementation *service)
        {
            inheritCheck<TService, TImplementation>();
            auto factory = std::make_unique<ExternalServiceFactory<TImplementation>>(service);
            return describe<TService>(ServiceLifeTime::singeleton(), std::move(factory));
        }

        template <class TService, class TImplementation = TService>
        static void describeSingeleton(ExternalProvider<TImplementation> provider)
        {
            return describe(ServiceDescriber::describe<TService>(ServiceLifeTime::singeleton(), std::move(provider)));
        }
        template <class TService, class TImplementation = TService>
        static void describeScoped(ExternalProvider<TImplementation> provider)
        {
            return describe(ServiceDescriber::describe<TService>(ServiceLifeTime::scoped(), std::move(provider)));
        }

        template <class TService, class TImplementation = TService>
        static ServiceDescriptor describe(ServiceLifeTime lifetime, ExternalProvider<TImplementation> provider)
        {
            inheritCheck<TService, TImplementation>();
            auto factory = std::make_unique<ExternalServiceFcnFactory<TImplementation>>(std::move(provider));
            return describe<TService>(lifetime, std::move(factory));
        }

        template <class TService, class TImplementation = TService>
        static ServiceDescriptor describe(ServiceLifeTime lifetime, Factory<TImplementation> factoryFcn)
        {
            inheritCheck<TService, TImplementation>();
            // todo check scope!= transient
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