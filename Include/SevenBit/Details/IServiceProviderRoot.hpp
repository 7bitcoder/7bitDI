#pragma once

#include "SevenBit/Details/ServiceDescriptorsMap.hpp"
#include "SevenBit/Details/ServicesMap.hpp"

namespace sb::details
{
    struct IServiceProviderRoot
    {
        virtual const ServiceDescriptorsMap &getDescriptorsMap() = 0;

        virtual ServicesMap &getSingletons() = 0;

        virtual ~IServiceProviderRoot() = default;
    };
} // namespace sb::details
