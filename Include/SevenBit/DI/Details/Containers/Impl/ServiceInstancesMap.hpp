#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceInstancesMap.hpp"

namespace sb::di::details::containers
{
    INLINE ServiceInstancesMap::ServiceInstancesMap(const bool strongDestructionOrder)
        : _strongDestructionOrder(strongDestructionOrder)
    {
    }

    INLINE ServiceInstanceList &ServiceInstancesMap::insert(const TypeId serviceTypeId, IServiceInstance::Ptr instance)
    {
        return insert(serviceTypeId, ServiceInstanceList{std::move(instance)});
    }

    INLINE ServiceInstanceList &ServiceInstancesMap::insert(const TypeId serviceTypeId, ServiceInstanceList instances)
    {
        auto [it, inserted] = _serviceListMap.emplace(serviceTypeId, std::move(instances));
        if (inserted && _strongDestructionOrder)
        {
            _constructionOrder.push_back(serviceTypeId);
        }
        return it->second;
    }

    INLINE bool ServiceInstancesMap::contains(const TypeId serviceTypeId) const
    {
        return _serviceListMap.find(serviceTypeId) != _serviceListMap.end();
    }

    INLINE ServiceInstanceList *ServiceInstancesMap::findServices(const TypeId serviceTypeId)
    {
        const auto it = _serviceListMap.find(serviceTypeId);
        return it != _serviceListMap.end() ? &it->second : nullptr;
    }

    INLINE bool ServiceInstancesMap::empty() const { return _serviceListMap.empty(); }

    INLINE void ServiceInstancesMap::clear()
    {
        if (_strongDestructionOrder)
        {
            for (auto it = _constructionOrder.rbegin(); it != _constructionOrder.rend(); ++it)
            {
                if (const auto list = findServices(*it))
                {
                    list->clear();
                }
            }
        }
        _constructionOrder.clear();
        _serviceListMap.clear();
    }

    INLINE ServiceInstancesMap::~ServiceInstancesMap() { clear(); }
} // namespace sb::di::details::containers
