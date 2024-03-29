#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details
{
    struct Check
    {
        template <class T> constexpr static bool notNull(const std::unique_ptr<T> &ptr) { return notNull(ptr.get()); }

        template <class T> constexpr static bool notNull(const std::shared_ptr<T> &ptr) { return notNull(ptr.get()); }

        template <class T> constexpr static bool notNull(const T *ptr) { return ptr != nullptr; }

        template <class TEnum> constexpr static bool enumValidity(TEnum value)
        {
            auto index = static_cast<std::underlying_type_t<TEnum>>(value);
            auto count = static_cast<std::underlying_type_t<TEnum>>(TEnum::Count);
            return std::is_enum_v<TEnum> && index >= 0 && index < count;
        }
    };
} // namespace sb::di::details
