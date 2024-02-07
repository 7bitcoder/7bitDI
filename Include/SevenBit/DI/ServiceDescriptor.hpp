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
         * @param serviceTypeId - service type identifier
         * @param implementationTypeId - service implementation type identifier, type must inhetit from service type
         * @param lifeTime - service life time: Singleton, Scoped or Transient
         * @param implementationFactory - service implementation factory, can be nullptr in that case describer is
         * treated as alias
         * @param castOffset - cast offset is a difference in bytes between implementation pointer and casted service
         * pointer, should be non zero for multiinheritance scenarios
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

        /**
         * @brief Get the service cast offset, non zero for multiinheritance
         * @details
         */
        [[nodiscard]] ptrdiff_t getCastOffset() const;

        /**
         * @brief Check if service is alias (implementation factory is nullptr)
         */
        [[nodiscard]] bool isAlias() const;

        bool operator!=(const ServiceDescriptor &descriptor) const;
        bool operator==(const ServiceDescriptor &descriptor) const;
    };
} // namespace sb::di

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Impl/ServiceDescriptor.hpp"
#endif
