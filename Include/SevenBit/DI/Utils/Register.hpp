#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Utils/GlobalServices.hpp"

namespace sb::di
{
    template <class TService, class TImplementation = TService, class TServicesCollection = GlobalServices>
    struct RegisterSingleton
    {
        inline static const bool registered =
            &TServicesCollection::instance().template addSingleton<TService, TImplementation>();

        static_assert(static_cast<bool>(&registered));
    };

    template <class TService, class TImplementation = TService, class TServicesCollection = GlobalServices>
    struct RegisterScoped
    {
        inline static const bool registered =
            &TServicesCollection::instance().template addScoped<TService, TImplementation>();

        static_assert(static_cast<bool>(&registered));
    };

    template <class TService, class TImplementation = TService, class TServicesCollection = GlobalServices>
    struct RegisterTransient
    {
        inline static const bool registered =
            &TServicesCollection::instance().template addTransient<TService, TImplementation>();

        static_assert(static_cast<bool>(&registered));
    };
} // namespace sb::di
