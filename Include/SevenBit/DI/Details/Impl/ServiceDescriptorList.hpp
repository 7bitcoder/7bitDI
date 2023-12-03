#pragma once

#include <algorithm>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Exceptions.hpp"

namespace sb::di::details
{

    INLINE ServiceDescriptorList::ServiceDescriptorList(ServiceDescriptor &&descriptor)
        : _oneOrList(std::move(descriptor))
    {
    }

    INLINE void ServiceDescriptorList::add(ServiceDescriptor &&descriptor)
    {
        checkBaseType(descriptor);
        checkLifeTime(descriptor);
        _oneOrList.add(std::move(descriptor));
    }

    INLINE const ServiceDescriptor &ServiceDescriptorList::first() const { return _oneOrList.first(); }

    INLINE const ServiceDescriptor &ServiceDescriptorList::last() const { return _oneOrList.last(); }

    INLINE bool ServiceDescriptorList::empty() const { return _oneOrList.empty(); }

    INLINE size_t ServiceDescriptorList::size() const { return _oneOrList.size(); }

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

    INLINE void ServiceDescriptorList::seal() { _oneOrList.shrink(); }

} // namespace sb::di::details
