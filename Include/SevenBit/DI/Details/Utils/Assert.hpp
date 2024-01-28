#pragma once

#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details::utils
{
    struct EXPORT Assert
    {
        template <class TService, class TImplementation> static void inheritance()
        {
            static_assert(std::is_same_v<TService, TImplementation> || std::is_base_of_v<TService, TImplementation>,
                          "Type TImplementation must inherit from TService");
        }

        template <class TAlias, class TService> static void isNotSame()
        {
            static_assert(!std::is_same_v<TAlias, TService>, "Type TAlias cannot be same as TService");
        }
    };
} // namespace sb::di::details::utils
