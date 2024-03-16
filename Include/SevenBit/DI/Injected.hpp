#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/ServiceProvider.hpp"

namespace sb::di
{
    template <class... TBases> struct Injected : TBases...
    {
      private:
        ServiceProvider &provider;

      public:
        explicit Injected(ServiceProvider &provider) : provider(provider) {}

      protected:
        ServiceProvider &getProvider() { return provider; }

        template <class TService> TService *tryInject() { return provider.tryGetService<TService>(); }

        template <class TService> TService &inject() { return provider.getService<TService>(); }

        template <class TService> std::vector<TService *> injectAll() { return provider.getServices<TService>(); }

        template <class TService> std::unique_ptr<TService> tryMake() { return provider.tryCreateService<TService>(); }

        template <class TService> std::unique_ptr<TService> make() { return provider.createService<TService>(); }

        template <class TService> TService makeInPlace() { return provider.createServiceInPlace<TService>(); }

        template <class TService> std::vector<std::unique_ptr<TService>> makeAll()
        {
            return provider.createServices<TService>();
        }
    };
} // namespace sb::di
