#pragma once

#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Utils/Register.hpp"
#include "SevenBit/DI/Utils/ServiceExtractor.hpp"

namespace sb::di
{
    struct Injected
    {
        explicit Injected(ServiceProvider &provider) : provider(provider) {}

      protected:
        ServiceProvider &provider;

        [[nodiscard]] ServiceExtractor inject() const { return ServiceExtractor{provider}; }
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
