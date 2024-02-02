#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/TypeId.hpp"

namespace sb::di
{
    struct IServiceInstance
    {
        using Ptr = std::unique_ptr<IServiceInstance>;
        using SPtr = std::shared_ptr<IServiceInstance>;

        /**
         * @brief Returns service pointer as void *
         */
        [[nodiscard]] virtual void *get() const = 0;

        /**
         * @brief Returns service pointer as void *,
         * @details Method is used to ensure that service can be moved out
         * @throws sb::di::CannotMoveOutServiceException
         *
         * Example:
         * @code{.cpp}
         * void* service = instance->getForMoveOut();
         * @endcode
         */
        [[nodiscard]] virtual void *getForMoveOut() = 0;

        /**
         * @brief Releases service ownership as void *
         * @details If instance is owner of service it will release this ownership just like
         * std::unique_ptr<T>::release(), otherwise it will throw exception
         * @throws sb::di::CannotReleaseServiceException
         * @warning Using this method might cause memory leaks, client is responsible for managing this pointner
         * lifetime, the best approach is to immediately wrap this pointer with proper std::unique_ptr<T>
         *
         * Example:
         * @code{.cpp}
         * std::unique_ptr<T> service{static_cast<T *>(instance->release())};
         * @endcode
         */
        [[nodiscard]] virtual void *release() = 0;

        /**
         * @brief Get the TypeId of service
         * @details This method can be used to check if casting is safe
         *
         * Example:
         * @code{.cpp}
         * if(instance->getTypeId() == typeid(T)) {
         *      T* service = instance->getAs<T>();
         * }
         * @endcode
         */
        [[nodiscard]] virtual TypeId getTypeId() const = 0;

        /**
         * @brief Checks if service instance is valid
         * @details If service instance is invalid, get and move methods might lead to undefined behaviour
         */
        [[nodiscard]] virtual bool isValid() const = 0;

        virtual ~IServiceInstance() = default;
    };
} // namespace sb::di
