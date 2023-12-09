#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

namespace sb::di
{
    /**
     * @brief Interface for all service factories
     */
    struct IServiceFactory
    {
        using Ptr = std::unique_ptr<IServiceFactory>;
        using SPtr = std::shared_ptr<IServiceFactory>;

        /**
         * @brief Get the TypeId of the service instances that the factory will create
         */
        [[nodiscard]] virtual TypeId getServiceTypeId() const = 0;

        /**
         * @brief Create a service service object
         */

        virtual IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider, bool inPlaceRequest) const = 0;

        virtual ~IServiceFactory() = default;
    };

} // namespace sb::di
