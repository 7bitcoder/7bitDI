#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/Require.hpp"
#include "SevenBit/DI/Details/Utils/RequireBase.hpp"

namespace sb::di::details::utils
{
    INLINE ServiceInstance *Require::validInstanceAndGet(ServiceInstance *instance)
    {
        validInstance(instance);
        return instance;
    }

    INLINE ServiceInstance &&Require::validInstanceAndGet(ServiceInstance &&instance)
    {
        validInstance(instance);
        return std::move(instance);
    }

    INLINE void Require::validInstance(const ServiceInstance *instance)
    {
        RequireBase::notNull(instance);
        validInstance(*instance);
    }

    INLINE void Require::validInstance(const ServiceInstance &instance)
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

    INLINE void Require::transientDescriptors(const containers::ServiceDescriptorList &descriptors)
    {
        if (!descriptors.getLifeTime().isTransient())
        {
            throw InjectorException{"Expected transient descriptors"};
        }
    }

    INLINE void Require::nonTransientDescriptors(const containers::ServiceDescriptorList &descriptors)
    {
        if (descriptors.getLifeTime().isTransient())
        {
            throw InjectorException{"Expected non transient descriptors"};
        }
    }

    INLINE void Require::aliasDescriptor(const ServiceDescriptor &descriptor)
    {
        if (!descriptor.isAlias())
        {
            throw InjectorException{"Expected alias descriptor"};
        }
    }

    INLINE void Require::nonAliasDescriptor(const ServiceDescriptor &descriptor)
    {
        if (descriptor.isAlias())
        {
            throw InjectorException{"Expected non alias descriptor"};
        }
    }
} // namespace sb::di::details::utils
