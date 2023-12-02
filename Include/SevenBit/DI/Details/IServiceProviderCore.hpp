#pragma once

#include <memory>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "ServiceDescriptorsMap.hpp"
#include "ServiceInstancesMap.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"

namespace sb::di::details
{
    struct IServiceProviderCore
    {
        using Ptr = std::unique_ptr<IServiceProviderCore>;
        using SPtr = std::shared_ptr<IServiceProviderCore>;

        virtual const ServiceProviderOptions &getOptions() const = 0;

        virtual const ServiceDescriptorsMap &getDescriptorsMap() const = 0;

        virtual ServiceInstancesMap &getSingletons() = 0;

        virtual ~IServiceProviderCore() = default;
    };
} // namespace sb::di::details
