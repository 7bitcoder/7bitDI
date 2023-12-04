#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceInstancesMap.hpp"
#include "SevenBit/DI/Exceptions.hpp"

namespace sb::di::details::containers
{
    INLINE ServiceInstancesMap::ServiceInstancesMap(bool strongDestructionOrder)
        : _strongDestructionOrder(strongDestructionOrder)
    {
    }

    INLINE ServiceInstanceList &ServiceInstancesMap::insert(TypeId serviceTypeId, IServiceInstance::Ptr service)
    {
        ServiceInstanceList &list = add(serviceTypeId, std::move(service));
        if (_strongDestructionOrder)
        {
            _constructionOrder.push_back(&list.last());
        }
        return list;
    }

    INLINE bool ServiceInstancesMap::contains(TypeId serviceTypeId) const
    {
        return _serviceListMap.find(serviceTypeId) != _serviceListMap.end();
    }

    INLINE ServiceInstanceList *ServiceInstancesMap::findServices(TypeId serviceTypeId)
    {
        auto it = _serviceListMap.find(serviceTypeId);
        return it != _serviceListMap.end() ? &it->second : nullptr;
    }

    INLINE bool ServiceInstancesMap::empty() const { return _serviceListMap.empty(); }

    INLINE void ServiceInstancesMap::clear()
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

    INLINE ServiceInstanceList &ServiceInstancesMap::add(TypeId serviceTypeId, IServiceInstance::Ptr &&service)
    {
        if (auto it = _serviceListMap.find(serviceTypeId); it != _serviceListMap.end())
        {
            it->second.add(std::move(service));
            return it->second;
        }
        return _serviceListMap.emplace(serviceTypeId, std::move(service)).first->second;
    }

    INLINE ServiceInstancesMap::~ServiceInstancesMap() { clear(); }
} // namespace sb::di::details::containers
