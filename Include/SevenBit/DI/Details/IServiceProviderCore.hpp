#pragma once

#include <memory>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "ServiceDescriptorsMap.hpp"
#include "ServicesMap.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"

namespace sb::di::details
{
    struct IServiceProviderCore
    {
        using Ptr = std::unique_ptr<IServiceProviderCore>;
        using SPtr = std::shared_ptr<IServiceProviderCore>;

        virtual const ServiceProviderOptions &getOptions() = 0;

        virtual const ServiceDescriptorsMap &getDescriptorsMap() = 0;

        virtual ServicesMap &getSingletons() = 0;

        virtual ~IServiceProviderCore() = default;
    };
} // namespace sb::di::details
