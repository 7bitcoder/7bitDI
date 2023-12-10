#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/IServiceFactory.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di
{
    /**
     * @brief Describes Service
     */
    class EXPORT ServiceDescriptor
    {
        ServiceLifeTime _lifetime;

        TypeId _serviceTypeId;

        IServiceFactory::SPtr _implementationFactory;

      public:
        using Ptr = std::unique_ptr<ServiceDescriptor>;

        /**
         * @brief Construct a new service descriptor object
         * @details implementationFactory cannot be null, otherwise construcor will throw exception
         * @throws NullPointnerException if implementationFactory is null
         */
        ServiceDescriptor(TypeId serviceTypeId, ServiceLifeTime lifetime, IServiceFactory::Ptr implementationFactory);

        /**
         * @brief Construct a new service descriptor copy
         */
        ServiceDescriptor(const ServiceDescriptor &other) = default;

        /**
         * @brief Construct a new service descriptor by move
         */
        ServiceDescriptor(ServiceDescriptor &&) = default;

        /**
         * @brief Assigns service descriptor
         */
        ServiceDescriptor &operator=(const ServiceDescriptor &other) = default;

        /**
         * @brief Assigns service descriptor by move
         */
        ServiceDescriptor &operator=(ServiceDescriptor &&other) = default;

        /**
         * @brief Get the lifetime object
         */
        [[nodiscard]] const ServiceLifeTime &getLifeTime() const;

        /**
         * @brief Get the service TypeId
         */
        [[nodiscard]] TypeId getServiceTypeId() const;

        /**
         * @brief Get the service implementation TypeId
         */
        [[nodiscard]] TypeId getImplementationTypeId() const;

        /**
         * @brief Get the service implementation factory
         */
        [[nodiscard]] const IServiceFactory &getImplementationFactory() const;
    };
} // namespace sb::di

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Impl/ServiceDescriptor.hpp"
#endif
