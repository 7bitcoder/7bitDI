#pragma once

#include <memory>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details::utils
{
    template <class T>
    struct IsInPlaceObject
        : std::integral_constant<bool, (std::is_arithmetic_v<T> || std::is_array_v<T> || std::is_enum_v<T> ||
                                        std::is_class_v<T>)&&!std::is_pointer_v<T> &&
                                           !std::is_reference_v<T>>
    {
    };
} // namespace sb::di::details::utils
