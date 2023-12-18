#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorsMap.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstancesMap.hpp"
#include "SevenBit/DI/Details/Helpers/ScopedGuard.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"

namespace sb::di::details::core
{
    struct IServiceInstanceProviderRoot
    {
        using Ptr = std::unique_ptr<IServiceInstanceProviderRoot>;
        using SPtr = std::shared_ptr<IServiceInstanceProviderRoot>;

        [[nodiscard]] virtual const containers::ServiceDescriptorsMap &getDescriptorsMap() const = 0;

        virtual containers::ServiceInstancesMap &getSingletons() = 0;

        virtual helpers::ScopedGuard spawhGuard(TypeId typeId) = 0;

        virtual IServiceInstance::Ptr createInstance(const ServiceDescriptor &descriptor, bool inPlaceRequest) = 0;

        virtual ~IServiceInstanceProviderRoot() = default;
    };
} // namespace sb::di::details::core
