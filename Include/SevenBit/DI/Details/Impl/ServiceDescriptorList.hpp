#pragma once

#include <algorithm>
#include <vector>

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/ServiceDescriptorList.hpp"

namespace sb::di::details
{

    INLINE ServiceDescriptorList::ServiceDescriptorList(ServiceDescriptor &&descriptor)
        : OneOrList<ServiceDescriptor>(std::move(descriptor))
    {
    }

    INLINE void ServiceDescriptorList::add(ServiceDescriptor &&descriptor)
    {
        checkBaseType(descriptor);
        checkLifeTime(descriptor);
        OneOrList<ServiceDescriptor>::add(std::move(descriptor));
    }

    INLINE const ServiceLifeTime &ServiceDescriptorList::getLifeTime() const { return first().getLifeTime(); }

    INLINE TypeId ServiceDescriptorList::getServiceTypeId() const { return first().getServiceTypeId(); }

    INLINE void ServiceDescriptorList::checkBaseType(ServiceDescriptor &descriptor) const
    {
        if (!empty() && descriptor.getServiceTypeId() != getServiceTypeId())
        {
            throw ServiceBaseTypeMissmatchException{descriptor.getImplementationTypeId(), getServiceTypeId()};
        }
    }

    INLINE void ServiceDescriptorList::checkLifeTime(ServiceDescriptor &descriptor) const
    {
        if (!empty() && descriptor.getLifeTime() != getLifeTime())
        {
            throw ServiceLifeTimeMissmatchException{descriptor.getImplementationTypeId(), getServiceTypeId()};
        }
    }

    INLINE void ServiceDescriptorList::seal() { shrink(); }

} // namespace sb::di::details
