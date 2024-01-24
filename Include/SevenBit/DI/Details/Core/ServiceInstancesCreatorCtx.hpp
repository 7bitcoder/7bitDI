#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/CircularDependencyGuard.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"

namespace sb::di::details::core
{
    class EXPORT ServiceInstancesCreatorCtx
    {
        ServiceProvider *_serviceProvider = nullptr;
        helpers::CircularDependencyGuard &_guard;
        const containers::ServiceDescriptorList &_descriptors;

      public:
        ServiceInstancesCreatorCtx(ServiceProvider *serviceProvider, helpers::CircularDependencyGuard &guard,
                                   const containers::ServiceDescriptorList &descriptors);

        [[nodiscard]] ServiceProvider &getServiceProvider() const;

        [[nodiscard]] helpers::CircularDependencyGuard &getGuard() const;

        [[nodiscard]] const containers::ServiceDescriptorList &getDescriptors() const;
    };
} // namespace sb::di::details::core

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Core/Impl/ServiceInstancesCreatorCtx.hpp"
#endif
