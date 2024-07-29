#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Exceptions.hpp"

namespace sb::di::details
{
    INLINE void ServiceDescriptorList::add(ServiceDescriptor &&descriptor)
    {
        if (!empty())
        {
            checkBaseType(descriptor);
            checkKey(descriptor);
            checkAlias(descriptor);
            if (!isAlias())
            {
                checkLifeTime(descriptor);
            }
        }
        OneOrList::add(std::move(descriptor));
    }

    INLINE ServiceLifeTime ServiceDescriptorList::getLifeTime() const { return first().getLifeTime(); }

    INLINE TypeId ServiceDescriptorList::getServiceTypeId() const { return first().getServiceTypeId(); }

    INLINE const std::string *ServiceDescriptorList::getServiceKey() const { return first().getServiceKey(); }

    INLINE bool ServiceDescriptorList::isAlias() const { return first().isAlias(); }

    INLINE void ServiceDescriptorList::checkBaseType(const ServiceDescriptor &descriptor) const
    {
        if (getServiceTypeId() != descriptor.getServiceTypeId()) // should not happen
        {
            throw ServiceRegisterException{descriptor.getImplementationTypeId(), "Service base type does not match"};
        }
    }

    INLINE void ServiceDescriptorList::checkKey(const ServiceDescriptor &descriptor) const
    {
        if (static_cast<bool>(getServiceKey()) != static_cast<bool>(descriptor.getServiceKey()) ||
            (getServiceKey() && *getServiceKey() != *descriptor.getServiceKey())) // should not happen
        {
            throw ServiceRegisterException{descriptor.getImplementationTypeId(), "Service key does not match"};
        }
    }

    INLINE void ServiceDescriptorList::checkAlias(const ServiceDescriptor &descriptor) const
    {
        if (isAlias() != descriptor.isAlias())
        {
            auto reason = details::String::fmt(
                "Service was expected to be registered as {}, like other services registered with this base type '{}'",
                (isAlias() ? "alias" : "not alias"), getServiceTypeId().name());
            throw ServiceRegisterException{descriptor.getImplementationTypeId(), reason};
        }
    }

    INLINE void ServiceDescriptorList::checkLifeTime(const ServiceDescriptor &descriptor) const
    {
        if (getLifeTime() != descriptor.getLifeTime())
        {
            auto reason = details::String::fmt(
                "Service was expected to be registered as {}, like other services registered with this base type '{}'",
                getLifeTime().toString(), getServiceTypeId().name());
            throw ServiceRegisterException{descriptor.getImplementationTypeId(), reason};
        }
    }
} // namespace sb::di::details
