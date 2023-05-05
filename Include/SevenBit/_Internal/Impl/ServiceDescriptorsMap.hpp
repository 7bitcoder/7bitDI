#pragma once

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/_Internal/ServiceDescriptorsMap.hpp"

namespace sb::internal
{
    INLINE void ServiceDescriptorsMap::add(ServiceDescriptor descriptor)
    {
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
} // namespace sb::internal
