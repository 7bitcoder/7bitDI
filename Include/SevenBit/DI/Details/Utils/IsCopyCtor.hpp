#pragma once

#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details
{
    template <class, class> struct IsCopyCtor : std::false_type
    {
    };

    template <class T> struct IsCopyCtor<T, T> : std::true_type
    {
    };

    template <class T> struct IsCopyCtor<T, const T> : std::true_type
    {
    };

    template <class T, class U> inline constexpr bool IsCopyCtorV = IsCopyCtor<T, U>::value;
} // namespace sb::di::details
