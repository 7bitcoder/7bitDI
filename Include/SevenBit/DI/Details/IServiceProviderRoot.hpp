#pragma once

#include "SevenBit/DI/Details/ServiceDescriptorsMap.hpp"
#include "SevenBit/DI/Details/ServicesMap.hpp"

namespace sb::di::details
{
    struct IServiceProviderRoot
    {
        virtual const ServiceDescriptorsMap &getDescriptorsMap() = 0;

        virtual ServicesMap &getSingletons() = 0;

        virtual ~IServiceProviderRoot() = default;
    };
} // namespace sb::di::details
