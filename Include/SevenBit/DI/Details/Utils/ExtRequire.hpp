#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/ServiceDescriptor.hpp"
#include "SevenBit/DI/ServiceInstance.hpp"

namespace sb::di::details
{
    struct EXPORT ExtRequire
    {
        static ServiceInstance *validInstanceAndGet(ServiceInstance *instance);
        static ServiceInstance &&validInstanceAndGet(ServiceInstance &&instance);

        static void validInstance(const ServiceInstance *instance);
        static void validInstance(const ServiceInstance &instance);

        static void transientDescriptor(const ServiceDescriptor &descriptor);
        static void nonTransientDescriptor(const ServiceDescriptor &descriptor);

        static void aliasDescriptor(const ServiceDescriptor &descriptor);
        static void nonAliasDescriptor(const ServiceDescriptor &descriptor);
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Utils/Impl/ExtRequire.hpp"
#endif
