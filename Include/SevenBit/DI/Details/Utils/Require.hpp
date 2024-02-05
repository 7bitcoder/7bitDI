#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"
#include "SevenBit/DI/ServiceInstance.hpp"

namespace sb::di::details::utils
{
    struct EXPORT Require
    {
        static ServiceInstance *validInstanceAndGet(ServiceInstance *instance);
        static ServiceInstance &&validInstanceAndGet(ServiceInstance &&instance);

        static void validInstance(const ServiceInstance *instance);
        static void validInstance(const ServiceInstance &instance);

        static void transientDescriptors(const containers::ServiceDescriptorList &descriptors);
        static void nonTransientDescriptors(const containers::ServiceDescriptorList &descriptors);

        static void aliasDescriptor(const ServiceDescriptor &descriptor);
        static void nonAliasDescriptor(const ServiceDescriptor &descriptor);
    };
} // namespace sb::di::details::utils

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Utils/Impl/Require.hpp"
#endif
