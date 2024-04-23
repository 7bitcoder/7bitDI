#pragma once

#include <SevenBit/DI/LibraryConfig.hpp>

#include <SevenBit/DI/ServiceDescriptor.hpp>
#include <SevenBit/DI/ServiceInstance.hpp>

namespace sb::di::details
{
    struct EXPORT RequireDescriptor
    {
        static void transient(const ServiceDescriptor &descriptor);
        static void nonTransient(const ServiceDescriptor &descriptor);

        static void alias(const ServiceDescriptor &descriptor);
        static void nonAlias(const ServiceDescriptor &descriptor);
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include <SevenBit/DI/Details/Utils/Impl/RequireDescriptor.hpp>
#endif
