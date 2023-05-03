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
        checkIfRegistered(descriptor);
        checkLifeTime(descriptor);
        _serviceDescriptors.emplace_back(std::move(descriptor));
    }

    INLINE size_t ServiceDescriptorList::remove(TypeId typeId)
    {
        return std::erase_if(_serviceDescriptors, [&](ServiceDescriptor &descriptor) {
            return descriptor.getImplementationTypeId() == typeId;
        });
    }

    INLINE bool ServiceDescriptorList::contains(TypeId typeId)
    {
        return std::find_if(begin(), end(), [&](const ServiceDescriptor &descriptor) {
                   return descriptor.getImplementationTypeId() == typeId;
               }) != end();
    }

    INLINE size_t ServiceDescriptorList::size() const { return _serviceDescriptors.size(); }

    INLINE const ServiceDescriptor &ServiceDescriptorList::last() const { return _serviceDescriptors.back(); }

    INLINE ServiceDescriptor &ServiceDescriptorList::last() { return _serviceDescriptors.back(); }

    INLINE void ServiceDescriptorList::checkIfRegistered(ServiceDescriptor &descriptor)
    {
        if (contains(descriptor.getImplementationTypeId()))
        {
            throw ServiceAlreadyRegisteredException{descriptor.getImplementationTypeId()};
        }
    }

    INLINE void ServiceDescriptorList::checkLifeTime(ServiceDescriptor &descriptor)
    {
        if (!_serviceDescriptors.empty() && last().getLifeTime() != descriptor.getLifeTime())
        {
            throw ServiceLifeTimeMissmatchException{descriptor.getImplementationTypeId(),
                                                    descriptor.getServiceTypeId()};
        }
    }

} // namespace sb::internal