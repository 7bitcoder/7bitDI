#pragma once

#include <SevenBit/DI/LibraryConfig.hpp>

#include <SevenBit/DI/ServiceDescriptor.hpp>
#include <SevenBit/DI/ServiceInstance.hpp>

namespace sb::di::details
{
    struct EXPORT RequireInstance
    {
        static ServiceInstance *validAndGet(ServiceInstance *instance);
        static ServiceInstance &&validAndGet(ServiceInstance &&instance);

        static void valid(const ServiceInstance *instance);
        static void valid(const ServiceInstance &instance);
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include <SevenBit/DI/Details/Utils/Impl/RequireInstance.hpp>
#endif
