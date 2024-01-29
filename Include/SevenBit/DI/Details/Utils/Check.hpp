#pragma once
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/IServiceInstance.hpp"

namespace sb::di::details::utils
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

        static bool instanceValidity(const IServiceInstance::Ptr &instance);

        static bool instanceValidity(const IServiceInstance *instance);
    };
} // namespace sb::di::details::utils

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Utils/Impl/Check.hpp"
#endif
