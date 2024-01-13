#pragma once

#include <memory>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details::utils
{
    template <class T> struct IsUniquePtr : std::false_type
    {
        using Type = T;
    };

    template <class T> struct IsUniquePtr<std::unique_ptr<T>> : std::true_type
    {
        using Type = T;
    };

    template <class T> inline constexpr bool IsUniquePtrV = IsUniquePtr<T>::value;

    template <class T> using IsUniquePtrT = typename IsUniquePtr<T>::Type;
} // namespace sb::di::details::utils
