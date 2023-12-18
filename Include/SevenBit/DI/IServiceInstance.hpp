#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di
{
    struct IServiceInstance
    {
        using Ptr = std::unique_ptr<IServiceInstance>;

        /**
         * @brief Returns service pointer as void *
         */
        [[nodiscard]] virtual void *get() const = 0;

        /**
         * @brief Returns service pointer as void *,
         * @details Method is used to ensure that service can be moved out
         * @throw sb::di::CannotMoveOutServiceException
         * @example
         * @code{.cpp}
         * void* service = instance->getForMoveOut();
         * @endcode
         */
        [[nodiscard]] virtual void *getForMoveOut() = 0;

        /**
         * @brief Releases service ownership as void *
         * @details If instance is owner of service it will release this ownership just like
         * std::unique_ptr<T>::release(), otherwise it will throw exception
         * @throw sb::di::CannotReleaseServiceException
         * @warning Using this method might cause memory leaks, client is responsible for managing this pointner
         * lifetime, the best approach is to immediately wrap this pointer with proper std::unique_ptr<T>
         * @example
         * @code{.cpp}
         * std::unique_ptr<T> service{static_cast<T *>(instance->release())};
         * @endcode
         */
        [[nodiscard]] virtual void *release() = 0;

        /**
         * @brief Get the TypeId of service
         * @details This method can be used to check if casting is safe
         * @example
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

        /**
         * @brief Wrapper around isValid method
         */
        explicit operator bool() const { return isValid(); }

        /**
         * @brief Returns service pointer as T *
         * @details The client is responsible for ensuring that the T type is correct
         * @example
         * @code{.cpp}
         * T* service = instance->getAs<T>();
         * @endcode
         */
        template <class T> [[nodiscard]] T *getAs() const { return static_cast<T *>(get()); }

        /**
         * @brief Releases service ownership as pointer T *
         * @details The client is responsible for ensuring that the T type is correct
         * @example
         * @code{.cpp}
         * T* service = instance->releaseAs<T>();
         * @endcode
         */
        template <class T> T *releaseAs() { return static_cast<T *>(release()); }

        /**
         * @brief Moves out service as unique_ptr<T>
         * @details The client is responsible for ensuring that the T type is correct
         * @example
         * @code{.cpp}
         * std::unique_ptr<T> service = instance->moveOutAsUniquePtr<T>();
         * @endcode
         */
        template <class T> std::unique_ptr<T> moveOutAsUniquePtr() { return std::unique_ptr<T>{releaseAs<T>()}; }

        /**
         * @brief Moves out service as T
         * @details The client is responsible for ensuring that the T type is correct
         * @example
         * @code{.cpp}
         * T service = instance->moveOutAs<T>();
         * @endcode
         */
        template <class T> T &&moveOutAs() { return std::move(*static_cast<T *>(getForMoveOut())); }

        virtual ~IServiceInstance() = default;
    };
} // namespace sb::di
