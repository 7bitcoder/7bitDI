#pragma once

#include <memory>
#include <utility>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceInstanceCreator.hpp"
#include <SevenBit/DI/Details/Services/AliasService.hpp>

namespace sb::di::details::core
{
    INLINE void ServiceInstanceCreator::setServiceProvider(ServiceProvider &serviceProvider)
    {
        _serviceProvider = &serviceProvider;
    }

    INLINE IServiceInstance::Ptr ServiceInstanceCreator::createInstance(const ServiceDescriptor &descriptor,
                                                                        const bool inPlaceRequest)
    {
        auto &provider = *utils::Require::notNullAndGet(_serviceProvider);
        auto &factory = descriptor.getImplementationFactory();
        auto _ = _guard(descriptor.getImplementationTypeId());
        return utils::Require::validInstanceAndGet(factory.createInstance(provider, inPlaceRequest));
    }

    INLINE IServiceInstance::Ptr ServiceInstanceCreator::createInstanceAlias(const IServiceInstance *instance,
                                                                             TypeId serviceTypeId)
    {
        utils::Require::notNull(instance);
        // auto _ = _guard(serviceTypeId);
        return std::make_unique<services::AliasService>(instance->get(), serviceTypeId);
    }
} // namespace sb::di::details::core
