#pragma once

#include <memory>

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/IServiceInstance.hpp"
#include "SevenBit/IServiceProvider.hpp"
#include "SevenBit/ServiceLifeTime.hpp"

namespace sb
{
    struct IServiceFactory
    {
        using Ptr = std::unique_ptr<IServiceFactory>;

        virtual TypeId getServiceTypeId() const = 0;

        virtual IServiceInstance::Ptr createInstance(IServiceProvider &serviceProvider) const = 0;

        virtual IServiceFactory::Ptr clone() = 0;

        virtual ~IServiceFactory() {}
    };

} // namespace sb