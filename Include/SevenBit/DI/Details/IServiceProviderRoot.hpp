#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/ServiceDescriptorsMap.hpp"
#include "SevenBit/DI/Details/ServiceInstancesMap.hpp"

namespace sb::di::details
{
    struct IServiceProviderRoot
    {
        virtual const ServiceDescriptorsMap &getDescriptorsMap() = 0;

        virtual ServiceInstancesMap &getSingletons() = 0;

        virtual ~IServiceProviderRoot() = default;
    };
} // namespace sb::di::details
