#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/IServiceFactory.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di
{
    class EXPORT ServiceDescriptor
    {
        TypeId _serviceTypeId;
        TypeId _implementationTypeId;
        ServiceLifeTime _lifeTime;
        IServiceFactory::SPtr _implementationFactory;
        ptrdiff_t _castOffset;

      public:
        using Ptr = std::unique_ptr<ServiceDescriptor>;

        /**
         * @brief Construct a new service descriptor object
         * @details implementationFactory cannot be null, otherwise constructor will throw exception
         * @throws sb::di::NullPointerException
         */
        ServiceDescriptor(TypeId serviceTypeId, TypeId implementationTypeId, ServiceLifeTime lifeTime,
                          IServiceFactory::Ptr implementationFactory, ptrdiff_t castOffset = 0);

        ServiceDescriptor(const ServiceDescriptor &other) = default;
        ServiceDescriptor(ServiceDescriptor &&other) = default;

        ServiceDescriptor &operator=(const ServiceDescriptor &other) = default;
        ServiceDescriptor &operator=(ServiceDescriptor &&other) = default;

        /**
         * @brief Get the service TypeId
         */
        [[nodiscard]] TypeId getServiceTypeId() const;

        /**
         * @brief Get the service implementation TypeId
         */
        [[nodiscard]] TypeId getImplementationTypeId() const;

        /**
         * @brief Get the lifetime object
         */
        [[nodiscard]] ServiceLifeTime getLifeTime() const;

        /**
         * @brief Get the service implementation factory
         */
        [[nodiscard]] const IServiceFactory &getImplementationFactory() const;

        [[nodiscard]] ptrdiff_t getCastOffset() const;

        /**
         * @brief Check if service is alias
         */
        [[nodiscard]] bool isAlias() const;

        bool operator!=(const ServiceDescriptor &descriptor) const;
        bool operator==(const ServiceDescriptor &descriptor) const;
    };
} // namespace sb::di

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Impl/ServiceDescriptor.hpp"
#endif
