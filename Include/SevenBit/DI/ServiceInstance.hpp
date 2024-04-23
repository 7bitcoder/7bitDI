#pragma once

#include <memory>

#include <SevenBit/DI/LibraryConfig.hpp>

#include <SevenBit/DI/Details/Utils/Cast.hpp>
#include <SevenBit/DI/IServiceInstance.hpp>

namespace sb::di
{
    class ServiceInstance
    {
        IServiceInstance::Ptr _implementation;
        ptrdiff_t _castOffset = 0;

      public:
        using Ptr = std::unique_ptr<ServiceInstance>;

        /**
         * @brief Creates default invalid service instance
         */
        ServiceInstance() = default;

        /**
         * @brief Created service instance with specified implementation and cast offset
         */
        explicit ServiceInstance(IServiceInstance::Ptr implementation, const ptrdiff_t castOffset = 0)
            : _implementation(std::move(implementation)), _castOffset(castOffset)
        {
        }

        ServiceInstance(const ServiceInstance &other) = delete;
        ServiceInstance(ServiceInstance &&other) = default;

        ServiceInstance &operator=(const ServiceInstance &other) = delete;
        ServiceInstance &operator=(ServiceInstance &&other) = default;

        /**
         * @brief Try get service instance implementation
         */
        IServiceInstance *tryGetImplementation() { return _implementation.get(); }

        /**
         * @brief Try get service instance implementation
         */
        [[nodiscard]] const IServiceInstance *tryGetImplementation() const { return _implementation.get(); }

        /**
         * @brief Get service instance implementation
         * @details If service instance implementation is nullptr, method throws exception
         * @throws sb::di::NullPointerException
         */
        IServiceInstance &getImplementation() { return *_implementation; }

        /**
         * @brief Get service instance implementation
         * @details If service instance implementation is nullptr, method throws exception
         * @throws sb::di::NullPointerException
         */
        [[nodiscard]] const IServiceInstance &getImplementation() const { return *_implementation; }

        /**
         * @details cast offset is added to service pointer when get/release methods are used
         */
        void addCastOffset(const ptrdiff_t castOffset) { _castOffset += castOffset; }

        /**
         * @details cast offset is added to service pointer when get/release methods are used
         */
        void setCastOffset(const ptrdiff_t castOffset) { _castOffset = castOffset; }

        /**
         * @details cast offset is added to service pointer when get/release methods are used
         */
        [[nodiscard]] ptrdiff_t getCastOffset() const { return _castOffset; }

        /**
         * @brief Checks if service instance is valid (implementation is not nullptr and implementation is valid)
         * @details If service instance is invalid, get and move methods might lead to undefined behaviour
         */
        [[nodiscard]] bool isValid() const { return _implementation && _implementation->isValid(); }

        /**
         * @brief Wrapper around isValid method
         */
        explicit operator bool() const { return isValid(); }

        /**
         * @brief Returns service pointer as T *
         * @details The client is responsible for ensuring that the T type is correct
         *
         * Example:
         * @code{.cpp}
         * T* service = instance.getAs<T>();
         * @endcode
         */
        template <class T> [[nodiscard]] T *getAs() const
        {
            return static_cast<T *>(applyOffset(getImplementation().get()));
        }

        /**
         * @brief Releases service ownership as pointer T *
         * @details The client is responsible for ensuring that the T type is correct
         * @warning Using this method might cause memory leaks, client is responsible for managing this pointner
         * lifetime, the best approach is to immediately wrap this pointer with proper std::unique_ptr<T>
         *
         * Example:
         * @code{.cpp}
         * T* service = instance.releaseAs<T>();
         * @endcode
         */
        template <class T> T *releaseAs() { return static_cast<T *>(applyOffset(getImplementation().release())); }

        /**
         * @brief Moves out service as unique_ptr<T>
         * @details The client is responsible for ensuring that the T type is correct
         *
         * Example:
         * @code{.cpp}
         * std::unique_ptr<T> service = instance.moveOutAsUniquePtr<T>();
         * @endcode
         */
        template <class T> std::unique_ptr<T> moveOutAsUniquePtr() { return std::unique_ptr<T>{releaseAs<T>()}; }

        /**
         * @brief Moves out service as T
         * @details The client is responsible for ensuring that the T type is correct
         *
         * Example:
         * @code{.cpp}
         * T service = instance.moveOutAs<T>();
         * @endcode
         */
        template <class T> T &&moveOutAs()
        {
            return std::move(*static_cast<T *>(applyOffset(getImplementation().getForMoveOut())));
        }

        /**
         * @brief Copies service as T
         * @details The client is responsible for ensuring that the T type is correct
         *
         * Example:
         * @code{.cpp}
         * T service = instance.copyAs<T>();
         * @endcode
         */
        template <class T> T copyAs() { return *static_cast<T *>(applyOffset(getImplementation().getForMoveOut())); }

        /**
         * @brief Clears service instance (implemnentation is reset)
         * @details after clearing instance becomes invalid
         */
        void clear()
        {
            _implementation.reset();
            _castOffset = 0;
        }

      private:
        void *applyOffset(void *ptr) const { return details::Cast::applyCastOffset(ptr, _castOffset); }
    };
} // namespace sb::di
