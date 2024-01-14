#pragma once

#include <memory>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details::utils
{
    template <class T> using IsPtr = std::is_pointer<T>;

    template <class T> inline constexpr bool IsPtrV = IsPtr<T>::value;

} // namespace sb::di::details::utils
