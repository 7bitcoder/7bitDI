#pragma once

#include <mutex>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorsMap.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstancesMap.hpp"
#include "SevenBit/DI/Details/Core/ServiceInstancesCreator.hpp"

namespace sb::di::details
{
    struct IServiceInstanceProviderRoot
    {
        [[nodiscard]] virtual const ServiceDescriptorsMap &getDescriptorsMap() const = 0;

        virtual ServiceInstancesMap &getSingletons() = 0;

        virtual ServiceInstancesCreator &getRootCreator() = 0;

        virtual std::recursive_mutex *tryGetSyncMutex() = 0;

        virtual ~IServiceInstanceProviderRoot() = default;
    };
} // namespace sb::di::details
