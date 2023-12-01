#pragma once

#include <algorithm>
#include <vector>

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/ServiceDescriptorList.hpp"

namespace sb::di::details
{

    INLINE ServiceDescriptorList::ServiceDescriptorList(ServiceDescriptor &&descriptor)
        : _descriptors(std::move(descriptor))
    {
    }

    INLINE const ServiceDescriptor &ServiceDescriptorList::operator[](size_t index) const
    {
        return _descriptors[index];
    }

    INLINE const ServiceDescriptor &ServiceDescriptorList::last() const { return _descriptors.last(); }

    INLINE ServiceDescriptor &ServiceDescriptorList::last() { return _descriptors.last(); }

    INLINE const ServiceLifeTime &ServiceDescriptorList::getLifeTime() const { return last().getLifeTime(); }

    INLINE TypeId ServiceDescriptorList::getServiceTypeId() const { return last().getServiceTypeId(); }

    INLINE void ServiceDescriptorList::add(ServiceDescriptor &&descriptor)
    {
        checkBaseType(descriptor);
        checkLifeTime(descriptor);
        _descriptors.add(std::move(descriptor));
    }

    INLINE size_t ServiceDescriptorList::size() const { return _descriptors.size(); }

    INLINE void ServiceDescriptorList::checkBaseType(ServiceDescriptor &descriptor) const
    {
        if (!_descriptors.empty() && descriptor.getServiceTypeId() != getServiceTypeId())
        {
            throw ServiceBaseTypeMissmatchException{descriptor.getImplementationTypeId(), getServiceTypeId()};
        }
    }

    INLINE void ServiceDescriptorList::checkLifeTime(ServiceDescriptor &descriptor) const
    {
        if (!_descriptors.empty() && descriptor.getLifeTime() != getLifeTime())
        {
            throw ServiceLifeTimeMissmatchException{descriptor.getImplementationTypeId(), getServiceTypeId()};
        }
    }

    INLINE void ServiceDescriptorList::seal() { _descriptors.shrink(); }

} // namespace sb::di::details
