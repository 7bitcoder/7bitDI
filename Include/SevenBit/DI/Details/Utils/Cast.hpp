#pragma once

#include <limits>
#include <unordered_set>

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details::utils
{
    struct Cast
    {
        template <class T, class I> static constexpr intptr_t getCastOffset()
        {
            auto implementation = reinterpret_cast<I *>(std::numeric_limits<size_t>::max() / 2);
            auto service = static_cast<T *>(implementation);
            return reinterpret_cast<intptr_t>(service) - reinterpret_cast<intptr_t>(implementation);
        };

        static void *applyCastOffset(void *ptr, const intptr_t offset)
        {
            const auto casted = reinterpret_cast<intptr_t>(ptr) + offset;
            return reinterpret_cast<void *>(casted);
        };
    };
} // namespace sb::di::details::utils
