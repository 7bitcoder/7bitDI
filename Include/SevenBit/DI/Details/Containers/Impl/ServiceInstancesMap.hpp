#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceInstancesMap.hpp"

namespace sb::di::details
{
    INLINE ServiceInstancesMap::ServiceInstancesMap(const bool strongDestructionOrder)
        : _strongDestructionOrder(strongDestructionOrder)
    {
    }

    INLINE ServiceInstanceList &ServiceInstancesMap::insert(ServiceId &&serviceId, ServiceInstance &&instance)
    {
        return insert(std::move(serviceId), ServiceInstanceList{std::move(instance)});
    }

    INLINE ServiceInstanceList &ServiceInstancesMap::insert(ServiceId &&serviceId, ServiceInstanceList &&instances)
    {
        auto [it, inserted] = _serviceListMap.emplace(std::move(serviceId), std::move(instances));
        if (inserted && _strongDestructionOrder)
        {
            _constructionOrder.push_back(it->first);
        }
        return it->second;
    }

    INLINE bool ServiceInstancesMap::contains(const ServiceId &serviceId) const
    {
        return _serviceListMap.count(serviceId);
    }

    INLINE ServiceInstanceList *ServiceInstancesMap::findInstances(const ServiceId &serviceId)
    {
        const auto it = _serviceListMap.find(serviceId);
        return it != _serviceListMap.end() ? &it->second : nullptr;
    }

    INLINE bool ServiceInstancesMap::empty() const { return _serviceListMap.empty(); }

    INLINE void ServiceInstancesMap::clear()
    {
        if (_strongDestructionOrder)
        {
            for (auto it = _constructionOrder.rbegin(); it != _constructionOrder.rend(); ++it)
            {
                if (const auto list = findInstances(*it))
                {
                    list->clear();
                }
            }
        }
        _constructionOrder.clear();
        _serviceListMap.clear();
    }

    INLINE ServiceInstancesMap::~ServiceInstancesMap() { clear(); }
} // namespace sb::di::details
