#pragma once

#include "SevenBit/LibraryConfig.hpp"
#include "SevenBit/_Internal/Exceptions.hpp"
#include "SevenBit/_Internal/ServicesContainer.hpp"

namespace sb
{

    INLINE ServiceList *ServicesContainer::addAndGetList(IServiceHolder::Ptr holder)
    {
        if (!holder)
        {
            throw ServiceHolderInvalidException{};
        }
        auto interfaceTypeId = holder->getServiceInterfaceTypeId();
        auto &serviceList = _serviceListMap[interfaceTypeId];
        serviceList.add(std::move(holder));
        return &serviceList;
    }

    INLINE ServiceList *ServicesContainer::getList(TypeId interfaceType)
    {
        auto it = _serviceListMap.find(interfaceType);
        return it != _serviceListMap.end() ? &it->second : nullptr;
    }

    INLINE const ServiceList *ServicesContainer::getList(TypeId interfaceType) const
    {
        auto it = _serviceListMap.find(interfaceType);
        return it != _serviceListMap.end() ? &it->second : nullptr;
    }

    INLINE bool ServicesContainer::contains(TypeId interfaceType) const { return getList(interfaceType); }

} // namespace sb