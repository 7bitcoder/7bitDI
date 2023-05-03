#pragma once

#include <cstddef>
#include <memory>

#include "SevenBit/LibraryConfig.hpp"

namespace sb::internal::utils
{
    template <class... T> inline constexpr bool notSupportedType = false;

    template <size_t N> struct StringLiteral
    {
        constexpr StringLiteral(const char (&str)[N])
        {
            for (size_t i = 0; i < N; ++i)
            {
                value[i] = str[i];
            }
        }

        char value[N] = {'\0'};
    };

    template <typename T, typename Enable = void> struct IsUniquePtr : std::false_type
    {
        using Type = T;
    };

    template <typename T> struct IsUniquePtr<std::unique_ptr<T>> : std::true_type
    {
        using Type = T;
    };

    template <typename T> inline constexpr bool IsUniquePtrV = IsUniquePtr<T>::value;

    template <class TService, class TImplementation> static void inheritCheck()
    {
        static_assert(std::is_base_of_v<TService, TImplementation>, "Type TImplementation must inherit from TService");
    }
} // namespace sb::internal::utils