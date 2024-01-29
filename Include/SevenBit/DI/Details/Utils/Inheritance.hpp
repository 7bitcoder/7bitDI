#pragma once

#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details::utils
{
    template <class T, class I>
    inline constexpr bool InheritanceV = std::is_same_v<T, I> || std::is_base_of_v<T, I>;

} // namespace sb::di::details::utils
