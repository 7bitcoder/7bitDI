#pragma once

#include <memory>
#include <string>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/IServiceFactory.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di
{
    class ServiceDescriptor
    {
        TypeId _serviceTypeId;
        TypeId _implementationTypeId;
        std::shared_ptr<const std::string> _serviceKey;
        std::shared_ptr<const std::string> _implementationKey;
        ServiceLifeTime _lifeTime;
        std::shared_ptr<const IServiceFactory> _implementationFactory;
        ptrdiff_t _castOffset;

      public:
        using Ptr = std::unique_ptr<ServiceDescriptor>;

        /**
         * @brief Construct a new service descriptor object
         * @param serviceTypeId - service type identifier
         * @param implementationTypeId - service implementation type identifier, type must inhetit from service type
         * @param serviceKey - service key
         * @param implementationKey - service implementation key
         * @param lifeTime - service life time: Singleton, Scoped or Transient
         * @param implementationFactory - service implementation factory, can be nullptr in that case describer is
         * treated as alias
         * @param castOffset - cast offset is a difference in bytes between implementation pointer and casted service
         * pointer, should be non zero for multiinheritance scenarios
         */
        ServiceDescriptor(const TypeId serviceTypeId, const TypeId implementationTypeId,
                          std::unique_ptr<std::string> serviceKey, std::unique_ptr<std::string> implementationKey,
                          const ServiceLifeTime lifeTime, IServiceFactory::Ptr implementationFactory,
                          const ptrdiff_t castOffset = 0)
            : _serviceTypeId(serviceTypeId), _implementationTypeId(implementationTypeId),
              _serviceKey(std::move(serviceKey)), _implementationKey(std::move(implementationKey)), _lifeTime(lifeTime),
              _implementationFactory(std::move(implementationFactory)), _castOffset(castOffset)

        {
        }

        ServiceDescriptor(const ServiceDescriptor &) = default;
        ServiceDescriptor(ServiceDescriptor &&) = default;

        ServiceDescriptor &operator=(const ServiceDescriptor &) = default;
        ServiceDescriptor &operator=(ServiceDescriptor &&) = default;

        /**
         * @brief Get the service TypeId
         */
        [[nodiscard]] TypeId getServiceTypeId() const { return _serviceTypeId; }

        /**
         * @brief Get the service implementation TypeId
         */
        [[nodiscard]] TypeId getImplementationTypeId() const { return _implementationTypeId; }

        /**
         * @brief Get the service key pointer
         */
        [[nodiscard]] const std::string *getServiceKey() const { return _serviceKey.get(); }

        /**
         * @brief Get the service implementation key pointer
         */
        [[nodiscard]] const std::string *getImplementationKey() const { return _implementationKey.get(); }

        /**
         * @brief Get the lifetime object
         */
        [[nodiscard]] ServiceLifeTime getLifeTime() const { return _lifeTime; }

        /**
         * @brief Get the service implementation factory pointer
         */
        [[nodiscard]] const IServiceFactory *getImplementationFactory() const { return _implementationFactory.get(); }

        /**
         * @brief Get the service cast offset, non zero for multiinheritance
         * @details
         */
        [[nodiscard]] ptrdiff_t getCastOffset() const { return _castOffset; }

        /**
         * @brief Check if service is alias (implementation factory is nullptr)
         */
        [[nodiscard]] bool isAlias() const { return !_implementationFactory; }

        friend bool operator==(const ServiceDescriptor &l, const ServiceDescriptor &r)
        {
            return l._serviceTypeId == r._serviceTypeId && l._implementationTypeId == r._implementationTypeId &&
                   l._serviceKey == r._serviceKey && l._implementationKey == r._implementationKey &&
                   l._lifeTime == r._lifeTime && l._implementationFactory == r._implementationFactory &&
                   l._castOffset == r._castOffset;
        }

        friend bool operator!=(const ServiceDescriptor &l, const ServiceDescriptor &r) { return !(l == r); }
    };

} // namespace sb::di
