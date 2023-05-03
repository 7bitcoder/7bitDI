#pragma once

#include "SevenBit/Exceptions.hpp"
#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/_Internal/ServiceList.hpp"

namespace sb::internal
{
    INLINE ServiceList &ServiceList::add(IServiceInstance::Ptr service)
    {
        if (!service || !service->isValid())
        {
            throw NullPointnerException{"service is null"};
        }
        _services.push_back(std::move(service));
        return *this;
    }

    INLINE void *ServiceList::get(TypeId typeId) const
    {
        if (auto instance = getInstance(typeId))
        {
            return instance->get();
        }
        return nullptr;
    }

    INLINE void *ServiceList::at(size_t index)
    {
        if (auto instance = getInstanceAt(index))
        {
            return instance->get();
        }
        return nullptr;
    }

    INLINE void *ServiceList::first() { return at(0); }

    INLINE bool ServiceList::empty() const { return _services.empty(); }

    INLINE std::vector<void *> ServiceList::getAll() const
    {
        std::vector<void *> result;
        result.reserve(_services.size());
        for (auto it = rBegin(); it != rEnd(); ++it)
        {
            auto &instance = *it;
            result.push_back(instance->get());
        }
        return result;
    }

    INLINE IServiceInstance *ServiceList::getInstance(TypeId typeId) const
    {
        for (auto &holder : _services)
        {
            if (holder->getTypeId() == typeId)
            {
                return holder.get();
            }
        }
        return nullptr;
    }

    INLINE IServiceInstance *ServiceList::getInstanceAt(size_t index) const
    {
        if (index < _services.size())
        {
            return _services.at(index).get();
        }
        return nullptr;
    }

    INLINE void ServiceList::reserve(size_t size) { _services.reserve(size); }

    INLINE void ServiceList::seal()
    {
        _services.shrink_to_fit();
        _sealed = true;
    }

    INLINE bool ServiceList::isSealed() const { return _sealed; }

    INLINE bool ServiceList::contains(TypeId typeId) const { return getInstance(typeId); }
} // namespace sb::internal