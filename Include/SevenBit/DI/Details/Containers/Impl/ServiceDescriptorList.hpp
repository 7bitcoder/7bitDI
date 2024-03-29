#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Exceptions.hpp"

namespace sb::di::details::containers
{

    INLINE ServiceDescriptorList::ServiceDescriptorList(ServiceDescriptor &&descriptor)
        : _oneOrList(std::move(descriptor))
    {
    }

    INLINE void ServiceDescriptorList::add(ServiceDescriptor &&descriptor)
    {
        if (!empty())
        {
            checkBaseType(descriptor);
            checkAlias(descriptor);
            checkLifeTime(descriptor);
        }
        _oneOrList.add(std::move(descriptor));
    }

    INLINE OneOrList<ServiceDescriptor> &ServiceDescriptorList::getInnerList() { return _oneOrList; }
    INLINE const OneOrList<ServiceDescriptor> &ServiceDescriptorList::getInnerList() const { return _oneOrList; }

    INLINE const ServiceDescriptor &ServiceDescriptorList::first() const { return _oneOrList.first(); }

    INLINE const ServiceDescriptor &ServiceDescriptorList::last() const { return _oneOrList.last(); }

    INLINE bool ServiceDescriptorList::empty() const { return _oneOrList.empty(); }

    INLINE size_t ServiceDescriptorList::size() const { return _oneOrList.size(); }

    INLINE ServiceLifeTime ServiceDescriptorList::getLifeTime() const { return first().getLifeTime(); }

    INLINE TypeId ServiceDescriptorList::getServiceTypeId() const { return first().getServiceTypeId(); }

    INLINE bool ServiceDescriptorList::isAlias() const { return first().isAlias(); }

    INLINE void ServiceDescriptorList::checkBaseType(const ServiceDescriptor &descriptor) const
    {
        if (descriptor.getServiceTypeId() != getServiceTypeId())
        {
            throw ServiceBaseTypeMismatchException{descriptor.getImplementationTypeId(), getServiceTypeId()};
        }
    }

    INLINE void ServiceDescriptorList::checkAlias(const ServiceDescriptor &descriptor) const
    {
        if (descriptor.isAlias() != isAlias())
        {
            throw ServiceAliasMismatchException{descriptor.getImplementationTypeId(), getServiceTypeId(), isAlias()};
        }
    }

    INLINE void ServiceDescriptorList::checkLifeTime(const ServiceDescriptor &descriptor) const
    {
        if (!isAlias() && !descriptor.isAlias() && descriptor.getLifeTime() != getLifeTime())
        {
            throw ServiceLifeTimeMismatchException{descriptor.getImplementationTypeId(), getServiceTypeId()};
        }
    }

    INLINE void ServiceDescriptorList::seal() { _oneOrList.shrink(); }

} // namespace sb::di::details::containers
