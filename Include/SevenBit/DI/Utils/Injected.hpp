#pragma once

#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceGetter.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"
#include "SevenBit/DI/Utils/Register.hpp"

namespace sb::di
{
    struct Injected
    {
        struct ServiceExtractor
        {
            ServiceProvider &_provider;

            template <class S, class = std::enable_if_t<!std::is_null_pointer_v<S> && !details::isInitializerListV<S>>>
            operator S()
            {
                return details::ServiceGetter<S>::get(_provider);
            }
            template <class S, class = std::enable_if_t<!details::IsUniquePtrV<S> && !std::is_null_pointer_v<S> &&
                                                        !details::IsVectorV<S> && !details::isInitializerListV<S>>>
            operator S &() const
            {
                return details::ServiceGetter<S &>::get(_provider);
            }
        };

      private:
        ServiceProvider &_provider;

      public:
        explicit Injected(ServiceProvider &provider) : _provider(provider) {}

      protected:
        [[nodiscard]] ServiceProvider &getProvider() const { return _provider; }

        [[nodiscard]] auto inject() const { return ServiceExtractor{getProvider()}; }
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
