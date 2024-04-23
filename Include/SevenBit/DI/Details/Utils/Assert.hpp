#pragma once

#include <SevenBit/DI/LibraryConfig.hpp>

#include <SevenBit/DI/Details/Utils/Meta.hpp>

namespace sb::di::details
{
    struct EXPORT Assert
    {
        template <class Type> static void serviceType()
        {
            static_assert(IsServiceTypeV<Type> && !std::is_const_v<Type> && !std::is_volatile_v<Type> ||
                              notSupportedType<Type>,
                          "Service type cannot be pointer, reference, constant, or volatile, and should be one of the "
                          "following: arithmetic, array, enum or class/struct");
        }

        template <class TService, class TImplementation> static void serviceTypes()
        {
            serviceType<TService>();
            serviceType<TImplementation>();

            static_assert(InheritanceV<TService, TImplementation>,
                          "Service implementation type: TImplementation must inherit from TService");
        }

        template <class TService, class TFactoryReturnType> static void factoryTypes()
        {
            serviceType<TService>();
            serviceType<TFactoryReturnType>();

            static_assert(InheritanceV<TService, TFactoryReturnType>,
                          "Service factory return type: TFactoryReturnType must inherit from TService");
        }

        template <class TAlias, class TService> static void aliasTypes()
        {
            serviceType<TAlias>();
            serviceType<TService>();

            static_assert(InheritanceV<TAlias, TService>, "TService must inherit from alias type: TAlias");

            static_assert(!std::is_same_v<TAlias, TService>, "Alias type: TAlias cannot be same as TService");
        }
    };
} // namespace sb::di::details
