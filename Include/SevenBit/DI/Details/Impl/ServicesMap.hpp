#pragma once

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/ServicesMap.hpp"

namespace sb::di::details
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

    INLINE bool ServicesMap::empty() { return _serviceListMap.empty(); }

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
} // namespace sb::di::details
