#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/ServiceProvider.hpp"

namespace sb::di
{
    template <class... TBases> struct Injected : TBases...
    {
      private:
        ServiceProvider &_provider;

      public:
        explicit Injected(ServiceProvider &provider) : _provider(provider) {}

      protected:
        ServiceProvider &getProvider() { return _provider; }

        template <class TService> TService *tryInject() { return _provider.tryGetService<TService>(); }

        template <class TService> TService &inject() { return _provider.getService<TService>(); }

        template <class TService> std::vector<TService *> injectAll() { return _provider.getServices<TService>(); }

        template <class TService> std::unique_ptr<TService> tryMake() { return _provider.tryCreateService<TService>(); }

        template <class TService> std::unique_ptr<TService> make() { return _provider.createService<TService>(); }

        template <class TService> TService makeInPlace() { return _provider.createServiceInPlace<TService>(); }

        template <class TService> std::vector<std::unique_ptr<TService>> makeAll()
        {
            return _provider.createServices<TService>();
        }

        template <class TService> TService *tryInjectKeyed(const std::string_view serviceKey)
        {
            return _provider.tryGetKeyedService<TService>(serviceKey);
        }

        template <class TService> TService &injectKeyed(const std::string_view serviceKey)
        {
            return _provider.getKeyedService<TService>(serviceKey);
        }

        template <class TService> std::vector<TService *> injectAllKeyed(const std::string_view serviceKey)
        {
            return _provider.getKeyedServices<TService>(serviceKey);
        }

        template <class TService> std::unique_ptr<TService> tryMakeKeyed(const std::string_view serviceKey)
        {
            return _provider.tryCreateKeyedService<TService>(serviceKey);
        }

        template <class TService> std::unique_ptr<TService> makeKeyed(const std::string_view serviceKey)
        {
            return _provider.createKeyedService<TService>(serviceKey);
        }

        template <class TService> TService makeKeyedInPlace(const std::string_view serviceKey)
        {
            return _provider.createKeyedServiceInPlace<TService>(serviceKey);
        }

        template <class TService> std::vector<std::unique_ptr<TService>> makeAllKeyed(const std::string_view serviceKey)
        {
            return _provider.createKeyedServices<TService>(serviceKey);
        }
    };
} // namespace sb::di
