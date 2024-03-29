#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details
{
    template <class... T> inline constexpr bool notSupportedType = false;
} // namespace sb::di::details
