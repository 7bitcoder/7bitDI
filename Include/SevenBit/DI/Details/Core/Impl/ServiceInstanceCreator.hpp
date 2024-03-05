#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceInstanceCreator.hpp"
#include "SevenBit/DI/Details/Services/AliasService.hpp"
#include "SevenBit/DI/Details/Utils/ExtRequire.hpp"
#include "SevenBit/DI/Details/Utils/Require.hpp"

namespace sb::di::details
{
    INLINE void ServiceInstanceCreator::setServiceProvider(ServiceProvider &serviceProvider)
    {
        _serviceProvider = &serviceProvider;
    }

    INLINE ServiceInstance ServiceInstanceCreator::createInstance(const ServiceDescriptor &descriptor,
                                                                  const bool inPlaceRequest)
    {
        ExtRequire::nonAliasDescriptor(descriptor);
        auto &provider = *Require::notNullAndGet(_serviceProvider);
        auto &factory = descriptor.getImplementationFactory();
        auto _ = _circularDependencyGuard(descriptor.getImplementationTypeId());

        auto implementation = factory.createInstance(provider, inPlaceRequest);
        return createInstance(std::move(implementation), descriptor.getCastOffset());
    }

    INLINE ServiceInstance ServiceInstanceCreator::createInstanceAlias(const ServiceDescriptor &descriptor,
                                                                       const ServiceInstance &instance)
    {
        ExtRequire::aliasDescriptor(descriptor);
        ExtRequire::validInstance(instance);
        auto implementationType = descriptor.getImplementationTypeId();

        auto implementation = std::make_unique<AliasService>(instance.getAs<void>(), implementationType);
        return createInstance(std::move(implementation), descriptor.getCastOffset());
    }

    INLINE ServiceInstance ServiceInstanceCreator::createInstance(IServiceInstance::Ptr &&implementation,
                                                                  const ptrdiff_t castOffset)
    {
        return ExtRequire::validInstanceAndGet(ServiceInstance{std::move(implementation), castOffset});
    }
} // namespace sb::di::details
