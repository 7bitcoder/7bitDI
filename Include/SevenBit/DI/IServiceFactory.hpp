#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/IServiceProvider.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"

namespace sb::di
{
    struct IServiceFactory
    {
        using Ptr = std::unique_ptr<IServiceFactory>;

        virtual TypeId getServiceTypeId() const = 0;

        virtual IServiceInstance::Ptr createInstance(IServiceProvider &serviceProvider) const = 0;

        virtual IServiceFactory::Ptr clone() = 0;

        virtual ~IServiceFactory() {}
    };

} // namespace sb::di