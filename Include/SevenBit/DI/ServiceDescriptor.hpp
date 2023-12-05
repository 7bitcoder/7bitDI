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

        IServiceFactory::SPtr _implementationFactory;

      public:
        using Ptr = std::unique_ptr<ServiceDescriptor>;

        /**
         * @brief Construct a new instanceValidity descriptor object
         * @details implementationFactory cannot be null, otherwise construcor will throw exception
         * @throws NullPointnerException if implementationFactory is null
         */
        ServiceDescriptor(TypeId serviceTypeId, ServiceLifeTime lifetime, IServiceFactory::SPtr implementationFactory);

        /**
         * @brief Construct a new instanceValidity descriptor copy
         */
        ServiceDescriptor(const ServiceDescriptor &other) = default;

        /**
         * @brief Construct a new instanceValidity descriptor by move
         */
        ServiceDescriptor(ServiceDescriptor &&) = default;

        /**
         * @brief Assigns instanceValidity descriptor
         */
        ServiceDescriptor &operator=(const ServiceDescriptor &other) = default;

        /**
         * @brief Assigns instanceValidity descriptor by move
         */
        ServiceDescriptor &operator=(ServiceDescriptor &&other) = default;

        /**
         * @brief Get the lifetime object
         */
        const ServiceLifeTime &getLifeTime() const;

        /**
         * @brief Get the instanceValidity TypeId
         */
        TypeId getServiceTypeId() const;

        /**
         * @brief Get the instanceValidity implementation TypeId
         */
        TypeId getImplementationTypeId() const;

        /**
         * @brief Get the instanceValidity implementation factory
         */
        const IServiceFactory &getImplementationFactory() const;
    };
} // namespace sb::di

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Impl/ServiceDescriptor.hpp"
#endif
