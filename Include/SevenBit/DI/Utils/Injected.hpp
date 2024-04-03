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

    template <class TService, class TImplementation = TService, class TRegisterer = SingletonRegisterer>
    struct InjectedSingleton : TService, Injected, RegisterSingleton<TService, TImplementation, TRegisterer>
    {
        using Injected::Injected;
    };

    template <class TService, class TRegisterer>
    struct InjectedSingleton<TService, TService, TRegisterer> : Injected,
                                                                RegisterSingleton<TService, TService, TRegisterer>
    {
        using Injected::Injected;
    };

    template <class TService, class TImplementation = TService, class TRegisterer = ScopedRegisterer>
    struct InjectedScoped : TService, Injected, RegisterScoped<TService, TImplementation, TRegisterer>
    {
        using Injected::Injected;
    };

    template <class TService, class TRegisterer>
    struct InjectedScoped<TService, TService, TRegisterer> : Injected, RegisterScoped<TService, TService, TRegisterer>
    {
        using Injected::Injected;
    };

    template <class TService, class TImplementation = TService, class TRegisterer = TransientRegisterer>
    struct InjectedTransient : TService, Injected, RegisterTransient<TService, TImplementation, TRegisterer>
    {
        using Injected::Injected;
    };

    template <class TService, class TRegisterer>
    struct InjectedTransient<TService, TService, TRegisterer> : Injected,
                                                                RegisterTransient<TService, TService, TRegisterer>
    {
        using Injected::Injected;
    };
} // namespace sb::di
