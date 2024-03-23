#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorsMap.hpp"
#include "SevenBit/DI/Exceptions.hpp"

namespace sb::di::details
{
    INLINE ServiceDescriptorsMap::ServiceDescriptorsMap(const bool checkDescriptorUniqueness)
    {
        _registeredServicesCheck =
            checkDescriptorUniqueness ? std::make_unique<std::unordered_set<ServiceId>>() : nullptr;
    }

    INLINE void ServiceDescriptorsMap::add(ServiceDescriptor descriptor)
    {
        return _registeredServicesCheck && !descriptor.isAlias() ? addDescriptorWithCheck(std::move(descriptor))
                                                                 : addDescriptor(std::move(descriptor));
    }

    INLINE void ServiceDescriptorsMap::seal() { _registeredServicesCheck.reset(); }

    INLINE const ServiceDescriptorList *ServiceDescriptorsMap::findDescriptors(const ServiceId &serviceId) const
    {
        if (const auto it = _serviceCreatorsMap.find(serviceId); it != _serviceCreatorsMap.end())
        {
            return &it->second;
        }
        return nullptr;
    }

    INLINE void ServiceDescriptorsMap::addDescriptorWithCheck(ServiceDescriptor &&descriptor)
    {
        ServiceId id{descriptor.getImplementationTypeId(), descriptor.getServiceKey()};
        if (_registeredServicesCheck->count(id))
        {
            throw ServiceAlreadyRegisteredException{descriptor.getImplementationTypeId()};
        }
        addDescriptor(std::move(descriptor));
        _registeredServicesCheck->insert(std::move(id));
    }

    INLINE void ServiceDescriptorsMap::addDescriptor(ServiceDescriptor &&descriptor)
    {
        ServiceId id{descriptor.getServiceTypeId(), descriptor.getServiceKey()};
        if (const auto it = _serviceCreatorsMap.find(id); it != _serviceCreatorsMap.end())
        {
            it->second.add(std::move(descriptor));
        }
        else
        {
            _serviceCreatorsMap.emplace(std::move(id), std::move(descriptor));
        }
    }

} // namespace sb::di::details
