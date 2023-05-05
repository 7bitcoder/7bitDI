#pragma once

#include "SevenBit/_Internal/ServiceDescriptorsMap.hpp"
#include "SevenBit/_Internal/ServicesMap.hpp"

namespace sb::internal
{
    struct IServiceProviderRoot
    {
        virtual const ServiceDescriptorsMap &getDescriptorsMap() = 0;

        virtual ServicesMap &getSingletons() = 0;

        virtual ~IServiceProviderRoot() = default;
    };
} // namespace sb::internal
