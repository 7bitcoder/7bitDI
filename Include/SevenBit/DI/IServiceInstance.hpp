#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/TypeId.hpp"

namespace sb::di
{
    /**
     * @brief Interface for all service instances
     */
    struct IServiceInstance
    {
        using Ptr = std::unique_ptr<IServiceInstance>;

        /**
         * @brief Returns service pointner as void *
         */
        virtual void *get() const = 0;

        /**
         * @brief Returns service pointner as T *
         * @details The client is responsible for ensuring that the T type is correct
         * @code {.cpp}
         * T* service = instance->getAs<T>();
         * @endcode
         */
        template <class T> T *getAs() const { return static_cast<T *>(get()); };

        /**
         * @brief Moves out service pointner from instance as void *
         * @details If instance is owner of service it will release this ownership just like
         * std::unique_ptr<T>::release(), otherwise it will throw exception
         * @throws CannotMoveOutServiceException cannot move out service
         * @warning Using this method might couse memory leaks, client is responsible for managing this pointner
         * lifetime, the best approach is to imediatly wrap this poinrner with proper std::unique_ptr<T>
         * @code {.cpp}
         * std::unique_ptr<T> service{static_cast<T *>(instance->moveOut())};
         * @endcode
         */
        [[nodiscard]] virtual void *moveOut() = 0;

        /**
         * @brief Moves out service pointner from instance as T *
         * @details This method bahaves exactly the same as moveOut method except that its casting type, the client is
         * responsible for ensuring that the T type is correct
         * @throws CannotMoveOutServiceException cannot move out service
         * @warning Using this method might couse memory leaks, clietn is responsible for managing this pointner
         * lifetime, the best approach is to imediatly wrap this poinrner with proper std::unique_ptr<T>
         * @code {.cpp}
         * std::unique_ptr<T> service{instance->moveOutAs<T>()};
         * @endcode
         */
        template <class T> [[nodiscard]] T *moveOutAs() { return static_cast<T *>(moveOut()); };

        /**
         * @brief Get the TypeId of service
         * @details  This method can be used to check if casting is safe
         * @code {.cpp}
         * if(instance->getTypeId() == typeid(T)) {
         *      T* service = instance->getAs<T>();
         * }
         * @endcode
         */
        virtual TypeId getTypeId() const = 0;

        /**
         * @brief Checks if service instance is valid
         * @details If service instance is invalid, get and move methods might lead to undefined behaviour
         */
        virtual bool isValid() const = 0;

        /**
         * @brief Wrapper around isValid method
         */
        operator bool() const { return isValid(); }

        virtual ~IServiceInstance() = default;
    };
} // namespace sb::di