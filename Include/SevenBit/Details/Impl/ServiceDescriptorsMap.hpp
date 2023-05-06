#pragma once

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/Details/ServiceDescriptorsMap.hpp"

namespace sb::details
{
    INLINE void ServiceDescriptorsMap::add(ServiceDescriptor descriptor)
    {
        checkIfAlreadyRegistered(descriptor);
        _serviceCreatorsMap[descriptor.getServiceTypeId()].add(std::move(descriptor));
    }

    INLINE void ServiceDescriptorsMap::seal()
    {
        for (auto &list : *this)
        {
            list.second.seal();
        }
    }

    INLINE const ServiceDescriptorList *ServiceDescriptorsMap::getDescriptorsList(TypeId typeId) const
    {
        if (auto it = _serviceCreatorsMap.find(typeId); it != end())
        {
            return &it->second;
        }
        return nullptr;
    }

    INLINE void ServiceDescriptorsMap::checkIfAlreadyRegistered(ServiceDescriptor &descriptor)
    {
        for (auto &[_, list] : _serviceCreatorsMap)
        {
            if (list.contains(descriptor.getImplementationTypeId()))
            {
                throw ServiceAlreadyRegisteredException{descriptor.getImplementationTypeId()};
            }
        }
    }
} // namespace sb::details
