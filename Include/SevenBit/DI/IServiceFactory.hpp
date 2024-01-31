#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/ServiceInstance.hpp"

namespace sb::di
{
    class ServiceProvider;

    struct IServiceFactory
    {
        using Ptr = std::unique_ptr<IServiceFactory>;
        using SPtr = std::shared_ptr<IServiceFactory>;

        /**
         * @brief Get the TypeId of the service instances that the factory will create
         */
        [[nodiscard]] virtual TypeId getServiceTypeId() const = 0;

        /**
         * @brief Create a service instance object
         */
        virtual ServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider, bool inPlaceRequest) const = 0;

        virtual ~IServiceFactory() = default;
    };

} // namespace sb::di
