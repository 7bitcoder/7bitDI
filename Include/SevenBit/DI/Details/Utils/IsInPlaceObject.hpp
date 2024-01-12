#pragma once

#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details::utils
{
    template <class T>
    struct IsInPlaceObject
        : std::bool_constant<!std::is_pointer_v<T> && !std::is_reference_v<T> &&
                             (std::is_arithmetic_v<T> || std::is_array_v<T> || std::is_enum_v<T> || std::is_class_v<T>)>
    {
    };

    template <class T> inline constexpr bool IsInPlaceObjectV = IsInPlaceObject<T>::value;

    template <class T>
    struct IsInPlaceObjectConstructable : std::bool_constant<IsInPlaceObjectV<T> && (std::is_move_constructible_v<T> ||
                                                                                     std::is_copy_constructible_v<T>)>
    {
    };

    template <class T> inline constexpr bool IsInPlaceObjectConstructableV = IsInPlaceObjectConstructable<T>::value;

} // namespace sb::di::details::utils
