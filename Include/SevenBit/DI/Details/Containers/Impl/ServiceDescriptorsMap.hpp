#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorsMap.hpp"
#include "SevenBit/DI/Exceptions.hpp"

namespace sb::di::details
{
    INLINE ServiceDescriptorsMap::ServiceDescriptorsMap(const bool checkDescriptorUniqueness)
    {
        _registeredServicesCheck = checkDescriptorUniqueness ? std::make_unique<std::unordered_set<TypeId>>() : nullptr;
    }

    INLINE void ServiceDescriptorsMap::add(ServiceDescriptor descriptor)
    {
        return _registeredServicesCheck && !descriptor.isAlias() ? addDescriptorWithCheck(std::move(descriptor))
                                                                 : addDescriptor(std::move(descriptor));
    }

    INLINE void ServiceDescriptorsMap::seal() { _registeredServicesCheck.reset(); }

    INLINE const ServiceDescriptorList *ServiceDescriptorsMap::findDescriptors(const TypeId typeId) const
    {
        if (const auto it = _serviceCreatorsMap.find(typeId); it != _serviceCreatorsMap.end())
        {
            return &it->second;
        }
        return nullptr;
    }

    INLINE void ServiceDescriptorsMap::addDescriptorWithCheck(ServiceDescriptor &&descriptor)
    {
        const auto implementationTypeId = descriptor.getImplementationTypeId();
        if (_registeredServicesCheck->count(implementationTypeId))
        {
            throw ServiceAlreadyRegisteredException{implementationTypeId};
        }
        addDescriptor(std::move(descriptor));
        _registeredServicesCheck->insert(implementationTypeId);
    }

    INLINE void ServiceDescriptorsMap::addDescriptor(ServiceDescriptor &&descriptor)
    {
        auto serviceTypeId = descriptor.getServiceTypeId();
        if (const auto it = _serviceCreatorsMap.find(serviceTypeId); it != _serviceCreatorsMap.end())
        {
            it->second.add(std::move(descriptor));
        }
        else
        {
            _serviceCreatorsMap.emplace(serviceTypeId, std::move(descriptor));
        }
    }

} // namespace sb::di::details
