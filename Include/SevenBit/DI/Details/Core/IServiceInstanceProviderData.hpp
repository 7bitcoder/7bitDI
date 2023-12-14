#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorsMap.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstancesMap.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"

namespace sb::di::details::core
{
    struct IServiceInstanceProviderData
    {
        using Ptr = std::unique_ptr<IServiceInstanceProviderData>;
        using SPtr = std::shared_ptr<IServiceInstanceProviderData>;

        [[nodiscard]] virtual const ServiceProviderOptions &getOptions() const = 0;

        [[nodiscard]] virtual const containers::ServiceDescriptorsMap &getDescriptorsMap() const = 0;

        virtual containers::ServiceInstancesMap &getSingletons() = 0;

        virtual ~IServiceInstanceProviderData() = default;
    };
} // namespace sb::di::details::core
