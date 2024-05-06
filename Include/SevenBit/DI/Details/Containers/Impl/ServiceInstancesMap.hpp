#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceInstancesMap.hpp"

namespace sb::di::details
{
    INLINE ServiceInstancesMap::ServiceInstancesMap(const bool strongDestructionOrder)
        : _strongDestructionOrder(strongDestructionOrder)
    {
    }

    INLINE ServiceInstanceList &ServiceInstancesMap::insert(const ServiceId &id, ServiceInstance &&instance)
    {
        return insert(id, ServiceInstanceList{std::move(instance)});
    }

    INLINE ServiceInstanceList &ServiceInstancesMap::insert(const ServiceId &id, ServiceInstanceList &&instances)
    {
        auto [it, inserted] = _instancesMap.emplace(id, std::move(instances));
        if (inserted && _strongDestructionOrder)
        {
            _constructionOrder.push_back(it->first);
        }
        return it->second;
    }

    INLINE bool ServiceInstancesMap::contains(const ServiceId &id) const { return _instancesMap.count(id); }

    INLINE ServiceInstanceList *ServiceInstancesMap::findInstances(const ServiceId &id)
    {
        const auto it = _instancesMap.find(id);
        return it != _instancesMap.end() ? &it->second : nullptr;
    }

    INLINE bool ServiceInstancesMap::empty() const { return _instancesMap.empty(); }

    INLINE void ServiceInstancesMap::clear()
    {
        if (_strongDestructionOrder)
        {
            for (auto it = _constructionOrder.rbegin(); it != _constructionOrder.rend(); ++it)
            {
                if (const auto listPtr = findInstances(*it))
                {
                    listPtr->clear();
                }
            }
        }
        _constructionOrder.clear();
        _instancesMap.clear();
    }

    INLINE ServiceInstancesMap::~ServiceInstancesMap() { clear(); }
} // namespace sb::di::details
