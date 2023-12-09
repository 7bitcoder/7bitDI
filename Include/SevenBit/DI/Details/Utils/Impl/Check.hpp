#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/Check.hpp"

namespace sb::di::details::utils
{
    INLINE bool Check::instanceValidity(const IServiceInstance::Ptr &instance)
    {
        return instanceValidity(instance.get());
    }

    INLINE bool Check::instanceValidity(const IServiceInstance *instance) { return instance && instance->isValid(); }
} // namespace sb::di::details::utils
