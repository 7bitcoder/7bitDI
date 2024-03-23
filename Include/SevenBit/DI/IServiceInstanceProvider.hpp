#pragma once

#include <memory>
#include <optional>
#include <string_view>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/OneOrList.hpp"
#include "SevenBit/DI/ServiceInstance.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di
{
    class ServiceProvider;

    struct IServiceInstanceProvider
    {
        using Ptr = std::unique_ptr<IServiceInstanceProvider>;

        /**
         * @brief Initializes instance provider
         */
        virtual void init(ServiceProvider &serviceProvider) = 0;

        /**
         * @brief Create a scoped service instance provider
         * @details Scoped service instance provider creates/holds its own scoped services
         */
        [[nodiscard]] virtual Ptr createScope() const = 0;

        /**
         * @brief Returns service instance reference, might throw exception
         * @details If service was not registered or was registered as transient, method throws exception
         * @throws sb::di::ServiceNotFoundException
         */
        virtual const ServiceInstance &getInstance(TypeId serviceTypeId) = 0;

        /**
         * @brief Returns service instance reference, might throw exception
         * @details If service was not registered or was registered as transient, method throws exception
         * @throws sb::di::ServiceNotFoundException
         */
        virtual const ServiceInstance &getKeyedInstance(TypeId serviceTypeId, std::string_view serviceKey) = 0;

        /**
         * @brief Returns service instance pointer
         * @details If service was not registered or was registered as transient, method returns null
         */
        virtual const ServiceInstance *tryGetInstance(TypeId serviceTypeId) = 0;

        /**
         * @brief Returns service instance pointer
         * @details If service was not registered or was registered as transient, method returns null
         */
        virtual const ServiceInstance *tryGetKeyedInstance(TypeId serviceTypeId, std::string_view serviceKey) = 0;

        /**
         * @brief Returns service instances
         * @details If service instance was not registered or was registered as transient, method returns null
         */
        virtual const OneOrList<ServiceInstance> *tryGetInstances(TypeId serviceTypeId) = 0;

        /**
         * @brief Returns service instances
         * @details If service instance was not registered or was registered as transient, method returns null
         */
        virtual const OneOrList<ServiceInstance> *tryGetKeyedInstances(TypeId serviceTypeId,
                                                                       std::string_view serviceKey) = 0;

        /**
         * @brief Creates service instance unique pointer, might throw exception
         * @details If service was not registered or was registered as scoped/singleton, method throw exception
         * @throws sb::di::ServiceNotFoundException
         */
        virtual ServiceInstance createInstance(TypeId serviceTypeId) = 0;

        /**
         * @brief Creates service instance unique pointer, might throw exception
         * @details If service was not registered or was registered as scoped/singleton, method throw exception
         * @throws sb::di::ServiceNotFoundException
         */
        virtual ServiceInstance createKeyedInstance(TypeId serviceTypeId, std::string_view serviceKey) = 0;

        /**
         * @brief Creates service instance unique pointer, might be null
         * @details If service was not registered or was registered as scoped/singleton, method returns invalid instance
         */
        virtual ServiceInstance tryCreateInstance(TypeId serviceTypeId) = 0;

        /**
         * @brief Creates service instance unique pointer, might be null
         * @details If service was not registered or was registered as scoped/singleton, method returns invalid instance
         */
        virtual ServiceInstance tryCreateKeyedInstance(TypeId serviceTypeId, std::string_view serviceKey) = 0;

        /**
         * @brief Creates service instances
         * @details If service was not registered or was registered as scoped/singleton, method returns null option
         */
        virtual std::optional<OneOrList<ServiceInstance>> tryCreateInstances(TypeId serviceTypeId) = 0;

        /**
         * @brief Creates service instances
         * @details If service was not registered or was registered as scoped/singleton, method returns null option
         */
        virtual std::optional<OneOrList<ServiceInstance>> tryCreateKeyedInstances(TypeId serviceTypeId,
                                                                                  std::string_view serviceKey) = 0;

        /**
         * @brief Creates service instance in place, might throw exception
         * @details If service was not registered or was registered as scoped/singleton, method throws exception
         * @throws sb::di::ServiceNotFoundException
         */
        virtual ServiceInstance createInstanceInPlace(TypeId serviceTypeId) = 0;

        /**
         * @brief Creates service instance in place, might throw exception
         * @details If service was not registered or was registered as scoped/singleton, method throws exception
         * @throws sb::di::ServiceNotFoundException
         */
        virtual ServiceInstance createKeyedInstanceInPlace(TypeId serviceTypeId, std::string_view serviceKey) = 0;

        /**
         * @brief Creates service instance in place
         * @details If service was not registered or was registered as scoped/singleton, method returns invalid
         * instance
         */
        virtual ServiceInstance tryCreateInstanceInPlace(TypeId serviceTypeId) = 0;

        /**
         * @brief Creates service instance in place
         * @details If service was not registered or was registered as scoped/singleton, method returns invalid instance
         */
        virtual ServiceInstance tryCreateKeyedInstanceInPlace(TypeId serviceTypeId, std::string_view serviceKey) = 0;

        virtual ~IServiceInstanceProvider() = default;
    };
} // namespace sb::di
