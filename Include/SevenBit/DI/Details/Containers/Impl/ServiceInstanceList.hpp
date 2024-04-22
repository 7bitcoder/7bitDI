#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceInstanceList.hpp"
#include "SevenBit/DI/Details/Utils/RequireInstance.hpp"

namespace sb::di::details
{
    INLINE ServiceInstanceList::ServiceInstanceList(const std::size_t size) : _oneOrList(size) {}

    INLINE ServiceInstanceList::ServiceInstanceList(ServiceInstance instance) : _oneOrList(std::move(instance)) {}

    INLINE void ServiceInstanceList::add(ServiceInstance &&instance) { _oneOrList.add(std::move(instance)); }

    INLINE void ServiceInstanceList::add(ServiceInstanceList &&instances) { add(std::move(instances.getInnerList())); }

    INLINE void ServiceInstanceList::add(OneOrList<ServiceInstance> &&instances)
    {
        _oneOrList.add(std::move(instances));
    }

    INLINE OneOrList<ServiceInstance> &ServiceInstanceList::getInnerList() { return _oneOrList; }
    INLINE const OneOrList<ServiceInstance> &ServiceInstanceList::getInnerList() const { return _oneOrList; }

    INLINE ServiceInstance &ServiceInstanceList::first() { return _oneOrList.first(); }
    INLINE const ServiceInstance &ServiceInstanceList::first() const { return _oneOrList.first(); }

    INLINE ServiceInstance &ServiceInstanceList::last() { return _oneOrList.last(); }
    INLINE const ServiceInstance &ServiceInstanceList::last() const { return _oneOrList.last(); }

    INLINE std::size_t ServiceInstanceList::size() const { return _oneOrList.size(); }

    INLINE bool ServiceInstanceList::empty() const { return _oneOrList.empty(); }

    INLINE void ServiceInstanceList::reserve(const std::size_t newCapacity) { return _oneOrList.reserve(newCapacity); }

    INLINE void ServiceInstanceList::shrink() { return _oneOrList.shrink(); }

    INLINE void ServiceInstanceList::seal() { _sealed = true; }

    INLINE void ServiceInstanceList::clear() { _oneOrList.clear(); }

} // namespace sb::di::details
