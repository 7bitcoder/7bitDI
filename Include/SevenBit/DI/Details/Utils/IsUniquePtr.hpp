#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details::utils
{
    template <class T> struct IsUniquePtr : std::false_type
    {
        using Type = T;
    };

    template <class T> struct IsUniquePtr<std::unique_ptr<T>> : std::true_type
    {
        using Type = T;
    };
} // namespace sb::di::details::utils
