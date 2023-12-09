#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorsMap.hpp"

namespace sb::di::details::containers
{
    INLINE ServiceDescriptorsMap::ServiceDescriptorsMap(const bool checkDescriptorUniqueness)
        : _checkDescriptorUniqueness(checkDescriptorUniqueness)
    {
    }

    INLINE void ServiceDescriptorsMap::add(ServiceDescriptor descriptor)
    {
        if (_checkDescriptorUniqueness)
        {
            const auto implementationTypeId = descriptor.getImplementationTypeId();
            checkIfAlreadyRegistered(implementationTypeId);
            addDescriptor(std::move(descriptor));
            registerService(implementationTypeId);
        }
        else
        {
            addDescriptor(std::move(descriptor));
        }
    }

    INLINE void ServiceDescriptorsMap::seal() { _registeredServices.clear(); }

    INLINE const ServiceDescriptorList *ServiceDescriptorsMap::findDescriptors(const TypeId typeId) const
    {
        if (const auto it = _serviceCreatorsMap.find(typeId); it != end())
        {
            return &it->second;
        }
        return nullptr;
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

    INLINE void ServiceDescriptorsMap::checkIfAlreadyRegistered(const TypeId implementationTypeId)
    {
        if (const auto it = _registeredServices.find(implementationTypeId); it != _registeredServices.end())
        {
            throw ServiceAlreadyRegisteredException{implementationTypeId};
        }
    }

    INLINE void ServiceDescriptorsMap::registerService(const TypeId implementationTypeId)
    {
        _registeredServices.insert(implementationTypeId);
    }

} // namespace sb::di::details::containers
