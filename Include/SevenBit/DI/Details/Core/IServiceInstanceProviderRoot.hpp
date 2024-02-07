#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorsMap.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstancesMap.hpp"
#include "SevenBit/DI/Details/Core/ServiceInstanceCreator.hpp"

namespace sb::di::details::core
{
    struct IServiceInstanceProviderRoot
    {
        [[nodiscard]] virtual const containers::ServiceDescriptorsMap &getDescriptorsMap() const = 0;

        virtual containers::ServiceInstancesMap &getSingletons() = 0;

        virtual ServiceInstanceCreator &getRootInstanceCreator() = 0;

        virtual ~IServiceInstanceProviderRoot() = default;
    };
} // namespace sb::di::details::core
