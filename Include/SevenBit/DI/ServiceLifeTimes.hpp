#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/ServiceLifeTime.hpp"

namespace sb::di
{
    struct ServiceLifeTimes
    {
        constexpr static ServiceLifeTime Singleton = ServiceLifeTime::singleton();
        constexpr static ServiceLifeTime Scoped = ServiceLifeTime::scoped();
        constexpr static ServiceLifeTime Transient = ServiceLifeTime::transient();
        constexpr static ServiceLifeTime Alias = ServiceLifeTime::alias();
    };
} // namespace sb::di
