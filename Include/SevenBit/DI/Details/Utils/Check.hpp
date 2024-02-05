#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/ServiceInstance.hpp"

namespace sb::di::details::utils
{
    struct EXPORT Check
    {
        static bool instanceValidity(const ServiceInstance *instance);

        static bool instanceValidity(const ServiceInstance &instance);
    };
} // namespace sb::di::details::utils

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Utils/Impl/Check.hpp"
#endif
