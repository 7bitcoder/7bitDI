#pragma once

#include <SevenBit/DI/LibraryConfig.hpp>

#include <SevenBit/DI/Utils/GlobalServices.hpp>

namespace sb::di
{
    template <class TService, class TImplementation, class TRegisterer> struct RegisterService
    {
        inline static const bool registered = TRegisterer::template registerService<TService, TImplementation>();

        static_assert(static_cast<bool>(&registered));
    };

    struct SingletonRegisterer
    {
        template <class TService, class TImplementation> static bool registerService()
        {
            return &GlobalServices::instance().addSingleton<TService, TImplementation>();
        }
    };

    template <class TService, class TImplementation = TService>
    using RegisterSingleton = RegisterService<TService, TImplementation, SingletonRegisterer>;

    struct ScopedRegisterer
    {
        template <class TService, class TImplementation> static bool registerService()
        {
            return &GlobalServices::instance().addScoped<TService, TImplementation>();
        }
    };

    template <class TService, class TImplementation = TService>
    using RegisterScoped = RegisterService<TService, TImplementation, ScopedRegisterer>;

    struct TransientRegisterer
    {
        template <class TService, class TImplementation> static bool registerService()
        {
            return &GlobalServices::instance().addTransient<TService, TImplementation>();
        }
    };

    template <class TService, class TImplementation = TService>
    using RegisterTransient = RegisterService<TService, TImplementation, TransientRegisterer>;
} // namespace sb::di
