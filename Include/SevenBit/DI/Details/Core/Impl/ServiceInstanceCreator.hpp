#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceInstanceCreator.hpp"
#include "SevenBit/DI/Details/Services/AliasService.hpp"
#include "SevenBit/DI/Details/Utils/Require.hpp"

namespace sb::di::details::core
{
    INLINE void ServiceInstanceCreator::setServiceProvider(ServiceProvider &serviceProvider)
    {
        _serviceProvider = &serviceProvider;
    }

    INLINE ServiceInstance ServiceInstanceCreator::createInstance(const ServiceDescriptor &descriptor,
                                                                  const bool inPlaceRequest)
    {
        auto &provider = *utils::Require::notNullAndGet(_serviceProvider);
        auto &factory = descriptor.getImplementationFactory();
        auto _ = _guard(descriptor.getImplementationTypeId());
        return ServiceInstance{factory.createInstance(provider, inPlaceRequest), descriptor.getCastOffset()};
    }

    INLINE ServiceInstance ServiceInstanceCreator::createInstanceAlias(const ServiceDescriptor &descriptor,
                                                                       const ServiceInstance *instance)
    {
        utils::Require::notNull(instance);
        auto aliasInstance =
            std::make_unique<services::AliasService>(instance->getAs<void>(), descriptor.getImplementationTypeId());
        return ServiceInstance{std::move(aliasInstance), descriptor.getCastOffset()};
    }
} // namespace sb::di::details::core
