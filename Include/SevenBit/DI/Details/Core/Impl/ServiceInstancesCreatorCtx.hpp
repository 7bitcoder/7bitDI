#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceInstancesCreatorCtx.hpp"
#include "SevenBit/DI/Details/Utils/Require.hpp"

namespace sb::di::details::core
{
    INLINE ServiceInstancesCreatorCtx::ServiceInstancesCreatorCtx(ServiceProvider *serviceProvider,
                                                                  helpers::CircularDependencyGuard &guard,
                                                                  const containers::ServiceDescriptorList &descriptors)
        : _serviceProvider(serviceProvider), _guard(guard), _descriptors(descriptors)
    {
    }

    INLINE ServiceProvider &ServiceInstancesCreatorCtx::getServiceProvider() const
    {
        return *utils::Require::notNullAndGet(_serviceProvider);
    }

    INLINE helpers::CircularDependencyGuard &ServiceInstancesCreatorCtx::getGuard() const { return _guard; }

    INLINE const containers::ServiceDescriptorList &ServiceInstancesCreatorCtx::getDescriptors() const
    {
        return _descriptors;
    }
} // namespace sb::di::details::core
