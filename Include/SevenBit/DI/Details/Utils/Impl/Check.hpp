#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/Check.hpp"

namespace sb::di::details::utils
{
    INLINE bool Check::instanceValidity(const ServiceInstance *instance) { return instance && instance->isValid(); }

    INLINE bool Check::instanceValidity(const ServiceInstance &instance) { return instance.isValid(); }
} // namespace sb::di::details::utils
