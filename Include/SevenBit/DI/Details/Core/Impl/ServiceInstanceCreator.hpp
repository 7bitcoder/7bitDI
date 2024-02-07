#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceInstanceCreator.hpp"
#include "SevenBit/DI/Details/Services/AliasService.hpp"
#include "SevenBit/DI/Details/Utils/Require.hpp"
#include "SevenBit/DI/Details/Utils/RequireBase.hpp"

namespace sb::di::details::core
{
    INLINE void ServiceInstanceCreator::setServiceProvider(ServiceProvider &serviceProvider)
    {
        _serviceProvider = &serviceProvider;
    }

    INLINE ServiceInstance ServiceInstanceCreator::createInstance(const ServiceDescriptor &descriptor,
                                                                  const bool inPlaceRequest)
    {
        utils::Require::nonAliasDescriptor(descriptor);
        auto &provider = *utils::RequireBase::notNullAndGet(_serviceProvider);
        auto &factory = descriptor.getImplementationFactory();
        auto _ = _guard(descriptor.getImplementationTypeId());

        auto implementation = factory.createInstance(provider, inPlaceRequest);
        return createInstance(std::move(implementation), descriptor.getCastOffset());
    }

    INLINE ServiceInstance ServiceInstanceCreator::createInstanceAlias(const ServiceDescriptor &descriptor,
                                                                       const ServiceInstance *instance)
    {
        utils::Require::aliasDescriptor(descriptor);
        utils::Require::validInstance(instance);
        auto implementationType = descriptor.getImplementationTypeId();

        auto implementation = std::make_unique<services::AliasService>(instance->getAs<void>(), implementationType);
        return createInstance(std::move(implementation), descriptor.getCastOffset());
    }

    INLINE ServiceInstance ServiceInstanceCreator::createInstance(IServiceInstance::Ptr &&implementation,
                                                                  const ptrdiff_t castOffset)
    {
        return utils::Require::validInstanceAndGet(ServiceInstance{std::move(implementation), castOffset});
    }
} // namespace sb::di::details::core
