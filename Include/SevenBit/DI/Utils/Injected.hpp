#pragma once

#include <SevenBit/DI/LibraryConfig.hpp>

#include <SevenBit/DI/Utils/Register.hpp>
#include <SevenBit/DI/Utils/ServiceExtractor.hpp>

namespace sb::di
{
    struct Injected
    {
      private:
        ServiceProvider &_provider;

      public:
        explicit Injected(ServiceProvider &provider) : _provider(provider) {}

      protected:
        [[nodiscard]] ServiceProvider &getProvider() const { return _provider; }

        [[nodiscard]] ServiceExtractor inject() const { return ServiceExtractor{getProvider()}; }
    };

    template <class TService, class TImplementation = TService>
    struct InjectedSingleton : TService, Injected, RegisterSingleton<TService, TImplementation>
    {
        using Injected::Injected;
    };

    template <class TService>
    struct InjectedSingleton<TService, TService> : Injected, RegisterSingleton<TService, TService>
    {
        using Injected::Injected;
    };

    template <class TService, class TImplementation = TService>
    struct InjectedScoped : TService, Injected, RegisterScoped<TService, TImplementation>
    {
        using Injected::Injected;
    };

    template <class TService> struct InjectedScoped<TService, TService> : Injected, RegisterScoped<TService, TService>
    {
        using Injected::Injected;
    };

    template <class TService, class TImplementation = TService>
    struct InjectedTransient : TService, Injected, RegisterTransient<TService, TImplementation>
    {
        using Injected::Injected;
    };

    template <class TService>
    struct InjectedTransient<TService, TService> : Injected, RegisterTransient<TService, TService>
    {
        using Injected::Injected;
    };
} // namespace sb::di
