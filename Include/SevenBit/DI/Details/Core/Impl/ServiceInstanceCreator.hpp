#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceInstanceCreator.hpp"
#include "SevenBit/DI/Details/Services/AliasService.hpp"
#include "SevenBit/DI/Details/Utils/Require.hpp"
#include "SevenBit/DI/Details/Utils/RequireDescriptor.hpp"
#include "SevenBit/DI/Details/Utils/RequireInstance.hpp"

namespace sb::di::details
{
    INLINE void ServiceInstanceCreator::setServiceProvider(ServiceProvider &serviceProvider)
    {
        _serviceProvider = &serviceProvider;
    }

    INLINE ServiceInstance ServiceInstanceCreator::createInstance(const ServiceDescriptor &descriptor,
                                                                  const bool inPlaceRequest)
    {
        RequireDescriptor::nonAlias(descriptor);
        auto &provider = *Require::notNullAndGet(_serviceProvider);
        const auto &factory = *Require::notNullAndGet(descriptor.getImplementationFactory());
        auto _ = _circularDependencyGuard(descriptor.getImplementationTypeId());

        auto implementation = factory.createInstance(provider, inPlaceRequest);
        return createInstance(std::move(implementation), descriptor.getCastOffset());
    }

    INLINE ServiceInstance ServiceInstanceCreator::createInstanceAlias(const ServiceDescriptor &descriptor,
                                                                       const ServiceInstance &instance)
    {
        RequireDescriptor::alias(descriptor);
        RequireInstance::valid(instance);
        auto implementationType = descriptor.getImplementationTypeId();

        auto implementation = std::make_unique<AliasService>(instance.getAs<void>(), implementationType);
        return createInstance(std::move(implementation), descriptor.getCastOffset());
    }

    INLINE ServiceInstance ServiceInstanceCreator::createInstance(IServiceInstance::Ptr &&implementation,
                                                                  const ptrdiff_t castOffset)
    {
        return RequireInstance::validAndGet(ServiceInstance{std::move(implementation), castOffset});
    }
} // namespace sb::di::details
