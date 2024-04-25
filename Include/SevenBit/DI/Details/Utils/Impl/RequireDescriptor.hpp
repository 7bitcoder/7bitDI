#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/RequireDescriptor.hpp"

namespace sb::di::details
{
    INLINE void RequireDescriptor::transient(const ServiceDescriptor &descriptor)
    {
        if (!descriptor.getLifeTime().isTransient())
        {
            throw InjectorException{"Expected transient descriptor"};
        }
    }

    INLINE void RequireDescriptor::nonTransient(const ServiceDescriptor &descriptor)
    {
        if (descriptor.getLifeTime().isTransient())
        {
            throw InjectorException{"Expected non transient descriptors"};
        }
    }

    INLINE void RequireDescriptor::alias(const ServiceDescriptor &descriptor)
    {
        if (!descriptor.isAlias())
        {
            throw InjectorException{"Expected alias descriptor"};
        }
    }

    INLINE void RequireDescriptor::nonAlias(const ServiceDescriptor &descriptor)
    {
        if (descriptor.isAlias())
        {
            throw InjectorException{"Expected non alias descriptor"};
        }
    }
} // namespace sb::di::details
