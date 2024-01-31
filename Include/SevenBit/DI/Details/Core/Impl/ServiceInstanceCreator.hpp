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

    INLINE ServiceInstance::Ptr ServiceInstanceCreator::createInstance(const ServiceDescriptor &descriptor,
                                                                       const bool inPlaceRequest)
    {
        auto &provider = *utils::Require::notNullAndGet(_serviceProvider);
        auto &factory = descriptor.getImplementationFactory();
        auto _ = _guard(descriptor.getImplementationTypeId());
        auto instance = utils::Require::validInstanceAndGet(factory.createInstance(provider, inPlaceRequest));
        instance->addGetOffset(descriptor.getCastOffset());
        return std::move(instance);
    }

    INLINE ServiceInstance::Ptr ServiceInstanceCreator::createInstanceAlias(const ServiceDescriptor &descriptor,
                                                                            const ServiceInstance *instance)
    {
        utils::Require::validInstance(instance);
        auto aliasInstance =
            std::make_unique<services::AliasService>(instance->getAs<void>(), descriptor.getImplementationTypeId());
        aliasInstance->addGetOffset(descriptor.getCastOffset());
        return std::move(aliasInstance);
    }
} // namespace sb::di::details::core
