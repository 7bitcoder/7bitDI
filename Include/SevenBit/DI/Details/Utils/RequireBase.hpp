#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/CheckBase.hpp"
#include "SevenBit/DI/Exceptions.hpp"

namespace sb::di::details::utils
{
    struct RequireBase
    {
        template <class T>
        static std::unique_ptr<T> &&notNullAndGet(std::unique_ptr<T> &&ptr, std::string_view failMessage = "")
        {
            notNull(ptr.get(), failMessage);
            return std::move(ptr);
        }

        template <class T>
        static std::shared_ptr<T> &&notNullAndGet(std::shared_ptr<T> &&ptr, std::string_view failMessage = "")
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

        template <class TEnum> constexpr static TEnum validEnumAndGet(TEnum value)
        {
            validEnum(value);
            return value;
        }

        template <class TEnum> constexpr static void validEnum(TEnum value)
        {
            if (!CheckBase::enumValidity(value))
            {
                throw InjectorException{"enum value: " + std::to_string(value) + " is invalid, shoud be in range [0" +
                                        std::to_string(TEnum::Count) + ")"};
            }
        }
    };
} // namespace sb::di::details::utils
