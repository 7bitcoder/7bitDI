#pragma once

#include "SevenBit/LibraryConfig.hpp"
#include "SevenBit/_Internal/Exceptions.hpp"
#include "SevenBit/_Internal/ServiceList.hpp"

namespace sb
{
    INLINE ServiceList::ServiceList() { _serviceHolders.reserve(1); }

    INLINE void ServiceList::add(IServiceHolder::Ptr holder)
    {
        if (!holder || !holder->isValid())
        {
            throw ServiceHolderInvalidException{};
        }
        _serviceHolders.push_back(std::move(holder));
    }

    INLINE void *ServiceList::get(TypeId typeId) const
    {
        if (auto serviceHolderPtr = getHolder(typeId))
        {
            return serviceHolderPtr->getService();
        }
        return nullptr;
    }

    INLINE void *ServiceList::at(size_t index) const
    {
        if (auto serviceHolderPtr = getHolderAt(index))
        {
            return serviceHolderPtr->getService();
        }
        return nullptr;
    }

    INLINE bool ServiceList::empty() const { return _serviceHolders.empty(); }

    INLINE std::vector<void *> ServiceList::getAll() const
    {
        std::vector<void *> result;
        result.reserve(_serviceHolders.size());
        for (auto &holder : _serviceHolders)
        {
            result.push_back(holder->getService());
        }
        return result;
    }

    INLINE IServiceHolder *ServiceList::getHolder(TypeId typeId) const
    {
        for (auto &holder : _serviceHolders)
        {
            if (holder->getServiceTypeId() == typeId)
            {
                return holder.get();
            }
        }
        return nullptr;
    }

    INLINE IServiceHolder *ServiceList::getHolderAt(size_t index) const
    {
        if (index < _serviceHolders.size())
        {
            return _serviceHolders.at(index).get();
        }
        return nullptr;
    }

    INLINE void ServiceList::reserve(size_t size) { _serviceHolders.reserve(size); }

    INLINE void ServiceList::seal()
    {
        _serviceHolders.shrink_to_fit();
        _sealed = true;
    }

    INLINE bool ServiceList::isSealed() const { return _sealed; }

    INLINE bool ServiceList::contains(TypeId typeId) const { return getHolder(typeId); }

} // namespace sb