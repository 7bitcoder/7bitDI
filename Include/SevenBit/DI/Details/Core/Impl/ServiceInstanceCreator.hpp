#pragma once

#include <memory>
#include <utility>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceInstanceCreator.hpp"

namespace sb::di::details::core
{
    INLINE void ServiceInstanceCreator::setServiceProvider(ServiceProvider *serviceProvider)
    {
        _serviceProvider = serviceProvider;
    }

    INLINE IServiceInstance::Ptr ServiceInstanceCreator::createInstance(const ServiceDescriptor &descriptor,
                                                                        const bool inPlaceRequest)
    {
        auto &provider = *utils::Require::notNullAndGet(_serviceProvider);
        auto &factory = descriptor.getImplementationFactory();
        auto _ = _guard.spawnGuard(descriptor.getImplementationTypeId());
        return utils::Require::validInstanceAndGet(factory.createInstance(provider, inPlaceRequest));
    }
} // namespace sb::di::details::core
