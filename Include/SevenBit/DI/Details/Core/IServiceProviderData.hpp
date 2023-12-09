#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorsMap.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstancesMap.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"

namespace sb::di::details::core
{
    struct IServiceProviderData
    {
        using Ptr = std::unique_ptr<IServiceProviderData>;
        using SPtr = std::shared_ptr<IServiceProviderData>;

        [[nodiscard]] virtual const ServiceProviderOptions &getOptions() const = 0;

        [[nodiscard]] virtual const containers::ServiceDescriptorsMap &getDescriptorsMap() const = 0;

        virtual containers::ServiceInstancesMap &getSingletons() = 0;

        virtual ~IServiceProviderData() = default;
    };
} // namespace sb::di::details::core
