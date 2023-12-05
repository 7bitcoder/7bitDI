#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"

namespace sb::di::details::utils
{
    struct EXPORT Check
    {
        static bool instanceValidity(const IServiceInstance::Ptr &instance);

        static bool instanceValidity(const IServiceInstance *instance);
    };
} // namespace sb::di::details::utils

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Utils/Impl/Check.hpp"
#endif
