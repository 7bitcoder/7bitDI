#pragma once

#include <memory>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details
{
    template <class T> struct IsUniquePtr : std::false_type
    {
    };

    template <class T> struct IsUniquePtr<std::unique_ptr<T>> : std::true_type
    {
    };

    template <class T> inline constexpr bool IsUniquePtrV = IsUniquePtr<T>::value;

} // namespace sb::di::details
