#pragma once

#include <SevenBit/DI/LibraryConfig.hpp>

#include <SevenBit/DI/Details/Containers/ServiceDescriptorsMap.hpp>
#include <SevenBit/DI/Exceptions.hpp>

namespace sb::di::details
{
    INLINE ServiceDescriptorsMap::ServiceDescriptorsMap(const bool checkDescriptorUniqueness)
    {
        _registeredServices = checkDescriptorUniqueness ? std::make_unique<std::unordered_set<ServiceId>>() : nullptr;
    }

    INLINE void ServiceDescriptorsMap::add(ServiceDescriptor descriptor)
    {
        if (_registeredServices && !descriptor.isAlias())
        {
            addDescriptorWithCheck(std::move(descriptor));
        }
        else
        {
            addDescriptor(std::move(descriptor));
        }
    }

    INLINE void ServiceDescriptorsMap::seal() { _registeredServices.reset(); }

    INLINE const ServiceDescriptorList *ServiceDescriptorsMap::findDescriptors(const ServiceId &id) const
    {
        if (const auto it = _descriptorsMap.find(id); it != _descriptorsMap.end())
        {
            return &it->second;
        }
        return nullptr;
    }

    INLINE void ServiceDescriptorsMap::addDescriptorWithCheck(ServiceDescriptor &&descriptor)
    {
        const ServiceId id{descriptor.getImplementationTypeId(), descriptor.getServiceKey()};
        if (_registeredServices->count(id))
        {
            throw ServiceAlreadyRegisteredException{descriptor.getImplementationTypeId()};
        }
        addDescriptor(std::move(descriptor));
        _registeredServices->insert(id);
    }

    INLINE void ServiceDescriptorsMap::addDescriptor(ServiceDescriptor &&descriptor)
    {
        ServiceId id{descriptor.getServiceTypeId(), descriptor.getServiceKey()};
        if (const auto it = _descriptorsMap.find(id); it != _descriptorsMap.end())
        {
            it->second.add(std::move(descriptor));
        }
        else
        {
            _descriptorsMap.emplace(id, std::move(descriptor));
        }
    }

} // namespace sb::di::details
