#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/ServicesMap.hpp"
#include "SevenBit/DI/Details/Utils.hpp"
#include "SevenBit/DI/Exceptions.hpp"

namespace sb::di::details
{
    INLINE ServicesMap::ServicesMap(bool strongDestructionOrder) : _strongDestructionOrder(strongDestructionOrder) {}

    INLINE ServiceList &ServicesMap::insert(TypeId serviceTypeId, IServiceInstance::Ptr service)
    {
        ServiceList &list = add(serviceTypeId, std::move(service));
        if (_strongDestructionOrder)
        {
            _constructionOrder.push_back(&list.last());
        }
        return list;
    }

    INLINE bool ServicesMap::contains(TypeId serviceTypeId) const
    {
        return _serviceListMap.find(serviceTypeId) != _serviceListMap.end();
    }

    INLINE ServiceList *ServicesMap::findServices(TypeId serviceTypeId)
    {
        auto it = _serviceListMap.find(serviceTypeId);
        return it != _serviceListMap.end() ? &it->second : nullptr;
    }

    INLINE bool ServicesMap::empty() const { return _serviceListMap.empty(); }

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

    INLINE ServiceList &ServicesMap::add(TypeId serviceTypeId, IServiceInstance::Ptr &&service)
    {
        if (auto it = _serviceListMap.find(serviceTypeId); it != _serviceListMap.end())
        {
            return it->second.add(std::move(service));
        }
        return _serviceListMap.emplace(serviceTypeId, std::move(service)).first->second;
    }

    INLINE ServicesMap::~ServicesMap() { clear(); }
} // namespace sb::di::details
