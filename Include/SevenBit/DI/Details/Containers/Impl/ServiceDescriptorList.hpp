#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Exceptions.hpp"

namespace sb::di::details
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
            checkKey(descriptor);
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

    INLINE std::size_t ServiceDescriptorList::size() const { return _oneOrList.size(); }

    INLINE ServiceLifeTime ServiceDescriptorList::getLifeTime() const { return first().getLifeTime(); }

    INLINE TypeId ServiceDescriptorList::getServiceTypeId() const { return first().getServiceTypeId(); }

    INLINE const std::string *ServiceDescriptorList::getServiceKey() const { return first().getServiceKey(); }

    INLINE bool ServiceDescriptorList::isAlias() const { return first().isAlias(); }

    INLINE void ServiceDescriptorList::checkBaseType(const ServiceDescriptor &descriptor) const
    {
        if (getServiceTypeId() != descriptor.getServiceTypeId())
        {
            throw InjectorException{"Service base type does not match"};
        }
    }

    INLINE void ServiceDescriptorList::checkKey(const ServiceDescriptor &descriptor) const
    {
        if (static_cast<bool>(getServiceKey()) != static_cast<bool>(descriptor.getServiceKey()) ||
            (getServiceKey() && *getServiceKey() != *descriptor.getServiceKey()))
        {
            throw InjectorException{"Service key does not match"};
        }
    }

    INLINE void ServiceDescriptorList::checkAlias(const ServiceDescriptor &descriptor) const
    {
        if (isAlias() != descriptor.isAlias())
        {
            throw ServiceAliasMismatchException{descriptor.getImplementationTypeId(), getServiceTypeId(), isAlias()};
        }
    }

    INLINE void ServiceDescriptorList::checkLifeTime(const ServiceDescriptor &descriptor) const
    {
        if (!isAlias() && !descriptor.isAlias() && getLifeTime() != descriptor.getLifeTime())
        {
            throw ServiceLifeTimeMismatchException{descriptor.getImplementationTypeId(), getServiceTypeId()};
        }
    }

    INLINE void ServiceDescriptorList::seal() { _oneOrList.shrink(); }

} // namespace sb::di::details
