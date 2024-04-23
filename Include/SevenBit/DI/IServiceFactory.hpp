#pragma once

#include <memory>

#include <SevenBit/DI/LibraryConfig.hpp>

#include <SevenBit/DI/IServiceInstance.hpp>

namespace sb::di
{
    class ServiceProvider;

    struct IServiceFactory
    {
        using Ptr = std::unique_ptr<IServiceFactory>;
        using SPtr = std::shared_ptr<IServiceFactory>;

        /**
         * @brief Create a service instance object
         */
        virtual IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider, bool inPlaceRequest) const = 0;

        virtual ~IServiceFactory() = default;
    };

} // namespace sb::di
