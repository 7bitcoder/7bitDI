#pragma once

#include <memory>

#include "SevenBit/_Internal/IServiceInstance.hpp"
#include "SevenBit/_Internal/ServiceLifeTime.hpp"

namespace sb
{
    class ServiceProvider;

    struct IServiceFactory
    {
        using Ptr = std::unique_ptr<IServiceFactory>;

        virtual TypeId getServiceTypeId() const = 0;

        virtual IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider) const = 0;

        virtual IServiceFactory::Ptr clone() = 0;

        virtual ~IServiceFactory() {}
    };

} // namespace sb