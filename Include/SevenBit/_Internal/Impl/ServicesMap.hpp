#pragma once

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/_Internal/ServicesMap.hpp"

namespace sb::internal
{
    INLINE ServiceList &ServicesMap::add(TypeId serviceTypeId, IServiceInstance::Ptr service)
    {
        return _serviceListMap[serviceTypeId].add(std::move(service));
    }

    INLINE ServiceList &ServicesMap::operator[](TypeId serviceTypeId) { return _serviceListMap[serviceTypeId]; }

    INLINE ServiceList *ServicesMap::getList(TypeId serviceTypeId)
    {
        auto it = _serviceListMap.find(serviceTypeId);
        return it != _serviceListMap.end() ? &it->second : nullptr;
    }

    INLINE const ServiceList *ServicesMap::getList(TypeId serviceTypeId) const
    {
        auto it = _serviceListMap.find(serviceTypeId);
        return it != _serviceListMap.end() ? &it->second : nullptr;
    }

} // namespace sb::internal
