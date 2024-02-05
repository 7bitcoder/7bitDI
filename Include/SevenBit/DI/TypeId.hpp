#pragma once

#include <typeindex>

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di
{
    /**
     * @brief Alias of std::type_index
     */
    using TypeId = std::type_index;

    using TypeInfo = std::type_info;

} // namespace sb::di
