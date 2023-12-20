#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details::utils
{
    template <class... T> inline constexpr bool notSupportedType = false;
} // namespace sb::di::details::utils
