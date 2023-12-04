#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"

namespace sb::di::details::utils
{
    struct EXPORT Check
    {
        template <class TService, class TImplementation> static void inherit()
        {
            static_assert(std::is_same_v<TService, TImplementation> || std::is_base_of_v<TService, TImplementation>,
                          "Type TImplementation must inherit from TService");
        }

        template <class T>
        static std::unique_ptr<T> &&notNullAndMove(std::unique_ptr<T> &&ptr, std::string_view failMessage = "")
        {
            notNull(ptr.get(), failMessage);
            return std::move(ptr);
        }

        template <class T>
        static std::shared_ptr<T> &&notNullAndMove(std::shared_ptr<T> &&ptr, std::string_view failMessage = "")
        {
            notNull(ptr.get(), failMessage);
            return std::move(ptr);
        }

        template <class T>
        static std::unique_ptr<T> notNullAndGet(std::unique_ptr<T> ptr, std::string_view failMessage = "")
        {
            notNull(ptr.get(), failMessage);
            return std::move(ptr);
        }

        template <class T>
        static std::shared_ptr<T> notNullAndGet(std::shared_ptr<T> ptr, std::string_view failMessage = "")
        {
            notNull(ptr.get(), failMessage);
            return std::move(ptr);
        }

        template <class T> static T *notNullAndGet(T *ptr, std::string_view failMessage = "")
        {
            notNull(ptr, failMessage);
            return ptr;
        }

        template <class T> static void notNull(const std::unique_ptr<T> &ptr, std::string_view failMessage = "")
        {
            notNull(ptr.get(), failMessage);
        }

        template <class T> static void notNull(const std::shared_ptr<T> &ptr, std::string_view failMessage = "")
        {
            notNull(ptr.get(), failMessage);
        }

        template <class T> static void notNull(const T *ptr, std::string_view failMessage = "")
        {
            if (!ptr)
            {
                auto message = !failMessage.empty()
                                   ? std::string{failMessage}
                                   : std::string{"Object of type: '"} + typeid(T).name() + "' cannot be null";
                throw NullPointnerException(message);
            }
        }

        static IServiceInstance::Ptr &&serviceAndMove(IServiceInstance::Ptr &&service);

        static IServiceInstance::Ptr serviceAndGet(IServiceInstance::Ptr service);

        static IServiceInstance *serviceAndGet(IServiceInstance *service);

        static void service(const IServiceInstance *service);

        static void service(const IServiceInstance::Ptr &service);
    };
} // namespace sb::di::details::utils

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Utils/Impl/Check.hpp"
#endif
