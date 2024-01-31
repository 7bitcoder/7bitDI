#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceInstanceList.hpp"
#include "SevenBit/DI/Details/Utils/Require.hpp"
#include "SevenBit/DI/ServiceInstance.hpp"

namespace sb::di::details::containers
{
    INLINE ServiceInstanceList::ServiceInstanceList(const size_t size) : _oneOrList(size) {}

    INLINE ServiceInstanceList::ServiceInstanceList(ServiceInstance::Ptr instance)
        : _oneOrList(details::utils::Require::validInstanceAndGet(std::move(instance)))
    {
    }

    INLINE void ServiceInstanceList::add(ServiceInstance::Ptr &&service)
    {
        _oneOrList.add(details::utils::Require::validInstanceAndGet(std::move(service)));
    }

    INLINE OneOrList<ServiceInstance::Ptr> &ServiceInstanceList::getInnerList() { return _oneOrList; }
    INLINE const OneOrList<ServiceInstance::Ptr> &ServiceInstanceList::getInnerList() const { return _oneOrList; }

    INLINE ServiceInstance::Ptr &ServiceInstanceList::first() { return _oneOrList.first(); }
    INLINE const ServiceInstance::Ptr &ServiceInstanceList::first() const { return _oneOrList.first(); }

    INLINE ServiceInstance::Ptr &ServiceInstanceList::last() { return _oneOrList.last(); }
    INLINE const ServiceInstance::Ptr &ServiceInstanceList::last() const { return _oneOrList.last(); }

    INLINE size_t ServiceInstanceList::size() const { return _oneOrList.size(); }

    INLINE bool ServiceInstanceList::empty() const { return _oneOrList.empty(); }

    INLINE void ServiceInstanceList::reserve(const size_t newCapacity) { return _oneOrList.reserve(newCapacity); }

    INLINE void ServiceInstanceList::shrink() { return _oneOrList.shrink(); }

    INLINE void ServiceInstanceList::seal()
    {
        _oneOrList.shrink();
        _sealed = true;
    }

    INLINE bool ServiceInstanceList::isSealed() const { return _sealed; }

    INLINE void ServiceInstanceList::clear()
    {
        if (const auto single = _oneOrList.tryGetAsSingle())
        {
            single->reset();
        }
        else
        {
            _oneOrList.getAsList().clear();
        }
    }

} // namespace sb::di::details::containers
