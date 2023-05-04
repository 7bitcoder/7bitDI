#pragma once

#include <algorithm>
#include <vector>

#include "SevenBit/Exceptions.hpp"
#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/_Internal/ServiceDescriptorList.hpp"

namespace sb::internal
{

    INLINE const ServiceLifeTime &ServiceDescriptorList::getLifeTime() const { return last().getLifeTime(); }

    INLINE TypeId ServiceDescriptorList::getServiceTypeId() const { return last().getServiceTypeId(); }

    INLINE void ServiceDescriptorList::seal() { _serviceDescriptors.shrink_to_fit(); }

    INLINE void ServiceDescriptorList::add(ServiceDescriptor descriptor)
    {
        checkBaseType(descriptor);
        checkIfRegistered(descriptor);
        checkLifeTime(descriptor);
        _serviceDescriptors.emplace_back(std::move(descriptor));
    }

    INLINE bool ServiceDescriptorList::contains(TypeId typeId)
    {
        return std::find_if(begin(), end(), [&](const ServiceDescriptor &descriptor) {
                   return descriptor.getImplementationTypeId() == typeId;
               }) != end();
    }

    INLINE size_t ServiceDescriptorList::size() const { return _serviceDescriptors.size(); }

    INLINE bool ServiceDescriptorList::empty() const { return _serviceDescriptors.empty(); }

    INLINE const ServiceDescriptor &ServiceDescriptorList::last() const { return _serviceDescriptors.back(); }

    INLINE ServiceDescriptor &ServiceDescriptorList::last() { return _serviceDescriptors.back(); }

    INLINE void ServiceDescriptorList::checkIfRegistered(ServiceDescriptor &descriptor)
    {
        if (contains(descriptor.getImplementationTypeId()))
        {
            throw ServiceAlreadyRegisteredException{descriptor.getImplementationTypeId()};
        }
    }

    INLINE void ServiceDescriptorList::checkBaseType(ServiceDescriptor &descriptor)
    {
        if (!empty() && descriptor.getServiceTypeId() != getServiceTypeId())
        {
            throw ServiceBaseTypeMissmatchException{descriptor.getServiceTypeId(), getServiceTypeId()};
        }
    }

    INLINE void ServiceDescriptorList::checkLifeTime(ServiceDescriptor &descriptor)
    {
        if (!empty() && descriptor.getLifeTime() != getLifeTime())
        {
            throw ServiceLifeTimeMissmatchException{descriptor.getImplementationTypeId(), getServiceTypeId()};
        }
    }

} // namespace sb::internal