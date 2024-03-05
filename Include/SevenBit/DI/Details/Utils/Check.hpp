#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details
{
    struct EXPORT Check
    {
        template <class T> constexpr static bool notNull(const std::unique_ptr<T> &ptr) { return notNull(ptr.get()); }

        template <class T> constexpr static bool notNull(const std::shared_ptr<T> &ptr) { return notNull(ptr.get()); }

        template <class T> constexpr static bool notNull(const T *ptr) { return ptr != nullptr; }

        template <class TEnum> constexpr static bool enumValidity(TEnum value)
        {
            return std::is_enum_v<TEnum> && value >= 0 && value < TEnum::Count;
        }
    };
} // namespace sb::di::details
