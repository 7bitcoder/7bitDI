#pragma once

#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceInjector.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"
#include "SevenBit/DI/Utils/Register.hpp"

namespace sb::di
{
    struct Injected
    {
      private:
        ServiceProvider &_provider;

      public:
        explicit Injected(ServiceProvider &provider) : _provider(provider) {}

      protected:
        ServiceProvider &getProvider() const { return _provider; }

        auto inject() const { return details::ServiceInjector{getProvider()}; }
    };

    template <class TService, class TImplementation = TService, class TServicesCollection = GlobalServices>
    struct InjectedSingleton : TService, Injected, RegisterSingleton<TService, TImplementation, TServicesCollection>
    {
        using Injected::Injected;
    };

    template <class TService, class TServicesCollection>
    struct InjectedSingleton<TService, TService, TServicesCollection>
        : Injected, RegisterSingleton<TService, TService, TServicesCollection>
    {
        using Injected::Injected;
    };

    template <class TService, class TImplementation = TService, class TServicesCollection = GlobalServices>
    struct InjectedScoped : TService, Injected, RegisterScoped<TService, TImplementation, TServicesCollection>
    {
        using Injected::Injected;
    };

    template <class TService, class TServicesCollection>
    struct InjectedScoped<TService, TService, TServicesCollection>
        : Injected, RegisterScoped<TService, TService, TServicesCollection>
    {
        using Injected::Injected;
    };

    template <class TService, class TImplementation = TService, class TServicesCollection = GlobalServices>
    struct InjectedTransient : TService, Injected, RegisterTransient<TService, TImplementation, TServicesCollection>
    {
        using Injected::Injected;
    };

    template <class TService, class TServicesCollection>
    struct InjectedTransient<TService, TService, TServicesCollection>
        : Injected, RegisterTransient<TService, TService, TServicesCollection>
    {
        using Injected::Injected;
    };
} // namespace sb::di
