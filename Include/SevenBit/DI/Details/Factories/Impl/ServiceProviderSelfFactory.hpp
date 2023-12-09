#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Services/ExternalService.hpp"
#include "SevenBit/DI/Details/Factories/ServiceProviderSelfFactory.hpp"

namespace sb::di::details::factories
{
    INLINE [[nodiscard]] TypeId ServiceProviderSelfFactory::getServiceTypeId() const { return typeid(ServiceProvider); }

    INLINE IServiceInstance::Ptr ServiceProviderSelfFactory::createInstance(ServiceProvider &serviceProvider,
                                                                            const bool inPlaceRequest) const
    {
        return std::make_unique<services::ExternalService<ServiceProvider>>(&serviceProvider);
    }
} // namespace sb::di::details::factories
