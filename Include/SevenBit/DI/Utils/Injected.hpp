#pragma once

#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Utils/Register.hpp"
#include "SevenBit/DI/Utils/ServiceExtractor.hpp"

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

        template <class Type> [[nodiscard]] Type inject() const
        {
            return details::ServiceGetter<std::remove_cv_t<Type>>::get(getProvider());
        }
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
