#pragma once

#include "SevenBit/LibraryConfig.hpp"

namespace sb
{
    struct ServiceProviderOptions
    {
        bool strongDestructionOrder = false;
        bool prebuildSingeletons = false;
    };
} // namespace sb
