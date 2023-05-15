#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Exceptions.hpp"
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
      private:
        ServiceLifeTime _lifetime;

        TypeId _serviceTypeId;

        IServiceFactory::Ptr _implementationFactory;

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
         * @details Note that implementation factory must be clonable
         */
        ServiceDescriptor(const ServiceDescriptor &other);

        /**
         * @brief Construct a new service descriptor by move
         */
        ServiceDescriptor(ServiceDescriptor &&) = default;

        /**
         * @brief Assigns service descriptor
         * @details Note that implementation factory must be clonable
         */
        ServiceDescriptor &operator=(const ServiceDescriptor &other);

        /**
         * @brief Assigns service descriptor by move
         */
        ServiceDescriptor &operator=(ServiceDescriptor &&other) = default;

        /**
         * @brief Get the lifetime object
         */
        const ServiceLifeTime &getLifeTime() const;

        /**
         * @brief Get the service TypeId
         */
        TypeId getServiceTypeId() const;

        /**
         * @brief Get the service implementation TypeId
         */
        TypeId getImplementationTypeId() const;

        /**
         * @brief Get the service implementation factory
         */
        const IServiceFactory &getImplementationFactory() const;
    };
} // namespace sb::di

#ifdef SEVEN_BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Impl/ServiceDescriptor.hpp"
#endif