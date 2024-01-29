#pragma once

#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/Inheritance.hpp"

namespace sb::di::details::utils
{
    struct EXPORT Assert
    {
        template <class TService, class TImplementation> static void inheritance()
        {
            static_assert(InheritanceV<TService, TImplementation>,
                          "Service implementation type: TImplementation must inherit from TService");
        }

        template <class TService, class TFactoryReturnType> static void factoryInheritance()
        {
            static_assert(InheritanceV<TService, TFactoryReturnType>,
                          "Service factory return type: TFactoryReturnType must inherit from TService");
        }

        template <class TAlias, class TService> static void aliasInheritance()
        {
            static_assert(InheritanceV<TAlias, TService>, "TService must inherit from alias type: TAlias");
        }

        template <class TAlias, class TService> static void aliasNotSame()
        {
            static_assert(!std::is_same_v<TAlias, TService>, "Alias type: TAlias cannot be same as TService");
        }
    };
} // namespace sb::di::details::utils
