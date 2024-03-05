#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/ExtCheck.hpp"

namespace sb::di::details
{
    INLINE bool ExtCheck::instanceValidity(const ServiceInstance *instance) { return instance && instance->isValid(); }

    INLINE bool ExtCheck::instanceValidity(const ServiceInstance &instance) { return instance.isValid(); }
} // namespace sb::di::details
