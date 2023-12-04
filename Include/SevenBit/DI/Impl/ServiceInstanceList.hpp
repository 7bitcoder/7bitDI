#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/Check.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/ServiceInstanceList.hpp"

namespace sb::di
{
    INLINE ServiceInstanceList::ServiceInstanceList(size_t size) : _oneOrList(size) {}

    INLINE ServiceInstanceList::ServiceInstanceList(IServiceInstance::Ptr instance)
        : _oneOrList(details::utils::Check::serviceAndGet(std::move(instance)))
    {
    }

    INLINE void ServiceInstanceList::add(IServiceInstance::Ptr &&service)
    {
        _oneOrList.add(details::utils::Check::serviceAndMove(std::move(service)));
    }

    INLINE OneOrList<IServiceInstance::Ptr> &ServiceInstanceList::getInnerList() { return _oneOrList; }
    INLINE const OneOrList<IServiceInstance::Ptr> &ServiceInstanceList::getInnerList() const { return _oneOrList; }

    INLINE IServiceInstance::Ptr &ServiceInstanceList::first() { return _oneOrList.first(); }
    INLINE const IServiceInstance::Ptr &ServiceInstanceList::first() const { return _oneOrList.first(); }

    INLINE IServiceInstance::Ptr &ServiceInstanceList::last() { return _oneOrList.last(); }
    INLINE const IServiceInstance::Ptr &ServiceInstanceList::last() const { return _oneOrList.last(); }

    INLINE IServiceInstance::Ptr &ServiceInstanceList::operator[](size_t index) { return _oneOrList[index]; }
    INLINE const IServiceInstance::Ptr &ServiceInstanceList::operator[](size_t index) const
    {
        return _oneOrList[index];
    }

    INLINE size_t ServiceInstanceList::size() const { return _oneOrList.size(); }

    INLINE bool ServiceInstanceList::empty() const { return _oneOrList.empty(); }

    INLINE void ServiceInstanceList::reserve(size_t newCapacity) { return _oneOrList.reserve(newCapacity); }

    INLINE void ServiceInstanceList::shrink() { return _oneOrList.shrink(); }

    INLINE void ServiceInstanceList::seal()
    {
        _oneOrList.shrink();
        _sealed = true;
    }

    INLINE bool ServiceInstanceList::isSealed() const { return _sealed; }

} // namespace sb::di
