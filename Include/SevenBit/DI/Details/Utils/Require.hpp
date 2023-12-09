#pragma once

#include <algorithm>
#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"

namespace sb::di::details::utils
{
    struct EXPORT Require
    {
        template <class T>
        static std::unique_ptr<T> notNullAndGet(std::unique_ptr<T> &&ptr, std::string_view failMessage = "")
        {
            notNull(ptr.get(), failMessage);
            return std::move(ptr);
        }

        template <class T>
        static std::shared_ptr<T> notNullAndGet(std::shared_ptr<T> &&ptr, std::string_view failMessage = "")
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

        template <class T> static void notNull(const T *ptr, const std::string_view failMessage = "")
        {
            if (!ptr)
            {
                const auto message = !failMessage.empty()
                                         ? std::string{failMessage}
                                         : std::string{"Object of type: '"} + typeid(T).name() + "' cannot be null";
                throw NullPointerException(message);
            }
        }

        static IServiceInstance::Ptr validInstanceAndGet(IServiceInstance::Ptr &&instance);

        static IServiceInstance &validInstanceAndGet(IServiceInstance *instance);

        static void validInstance(const IServiceInstance::Ptr &instance);

        static void validInstance(const IServiceInstance *instance);
    };
} // namespace sb::di::details::utils

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Utils/Impl/Require.hpp"
#endif
