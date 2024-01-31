#pragma once

#include <limits>
#include <unordered_set>

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details::utils
{
    template <class TService, class TImplementation> struct CastOffset
    {
        static constexpr int get()
        {
            auto implementation = reinterpret_cast<TImplementation *>(std::numeric_limits<size_t>::max() / 2);
            TService *service = implementation;
            return reinterpret_cast<std::byte *>(service) - reinterpret_cast<std::byte *>(implementation);
        };
    };
} // namespace sb::di::details::utils
