#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/ExtRequire.hpp"
#include "SevenBit/DI/Details/Utils/Require.hpp"

namespace sb::di::details
{
    INLINE ServiceInstance *ExtRequire::validInstanceAndGet(ServiceInstance *instance)
    {
        validInstance(instance);
        return instance;
    }

    INLINE ServiceInstance &&ExtRequire::validInstanceAndGet(ServiceInstance &&instance)
    {
        validInstance(instance);
        return std::move(instance);
    }

    INLINE void ExtRequire::validInstance(const ServiceInstance *instance)
    {
        Require::notNull(instance);
        validInstance(*instance);
    }

    INLINE void ExtRequire::validInstance(const ServiceInstance &instance)
    {
        if (!instance.isValid())
        {
            if (instance.tryGetImplementation())
            {
                throw InvalidServiceException{instance.getImplementation().getTypeId()};
            }
            throw InvalidServiceException{};
        }
    }

    INLINE void ExtRequire::transientDescriptors(const ServiceDescriptorList &descriptors)
    {
        if (!descriptors.getLifeTime().isTransient())
        {
            throw InjectorException{"Expected transient descriptors"};
        }
    }

    INLINE void ExtRequire::nonTransientDescriptors(const ServiceDescriptorList &descriptors)
    {
        if (descriptors.getLifeTime().isTransient())
        {
            throw InjectorException{"Expected non transient descriptors"};
        }
    }

    INLINE void ExtRequire::aliasDescriptor(const ServiceDescriptor &descriptor)
    {
        if (!descriptor.isAlias())
        {
            throw InjectorException{"Expected alias descriptor"};
        }
    }

    INLINE void ExtRequire::nonAliasDescriptor(const ServiceDescriptor &descriptor)
    {
        if (descriptor.isAlias())
        {
            throw InjectorException{"Expected non alias descriptor"};
        }
    }
} // namespace sb::di::details
