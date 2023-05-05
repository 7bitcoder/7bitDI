#pragma once

#include "SevenBit/Exceptions.hpp"
#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/_Internal/ServicesMap.hpp"

namespace sb::internal
{
    INLINE ServicesMap::ServicesMap(bool strongDestructionOrder) : _strongDestructionOrder(strongDestructionOrder) {}

    INLINE ServiceList &ServicesMap::add(TypeId serviceTypeId, IServiceInstance::Ptr service)
    {
        auto &list = _serviceListMap[serviceTypeId].add(std::move(service));
        if (_strongDestructionOrder)
        {
            _constructionOrder.push_back(&list.last());
        }
        return list;
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
    INLINE void ServicesMap::clear()
    {
        if (_strongDestructionOrder)
        {
            for (auto it = _constructionOrder.rbegin(); it != _constructionOrder.rend(); ++it)
            {
                (*it)->reset();
            }
        }
        _constructionOrder.clear();
        _serviceListMap.clear();
    }

    INLINE ServicesMap::~ServicesMap() { clear(); }
} // namespace sb::internal
