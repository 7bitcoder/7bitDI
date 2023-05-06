#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di
{
    struct ServiceProviderOptions
    {
        bool strongDestructionOrder = false;
        bool prebuildSingeletons = false;
    };
} // namespace sb::di
