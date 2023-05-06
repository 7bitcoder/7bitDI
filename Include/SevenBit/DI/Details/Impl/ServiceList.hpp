#pragma once

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/ServiceList.hpp"

namespace sb::di::details
{
    INLINE ServiceList &ServiceList::add(IServiceInstance::Ptr service)
    {
        if (!service || !service->isValid())
        {
            throw NullPointnerException{"service cannot be null"};
        }
        _services.push_back(std::move(service));
        return *this;
    }

    INLINE IServiceInstance::Ptr &ServiceList::first() { return _services.front(); }

    INLINE IServiceInstance::Ptr &ServiceList::last() { return _services.back(); }

    INLINE IServiceInstance::Ptr &ServiceList::at(size_t index) { return _services.at(index); }

    INLINE std::vector<const IServiceInstance *> ServiceList::getAllServices() const
    {
        std::vector<const IServiceInstance *> result;
        result.reserve(_services.size());
        for (auto it = rBegin(); it != rEnd(); ++it)
        {
            auto &instance = *it;
            result.push_back(instance.get());
        }
        return result;
    }

    INLINE bool ServiceList::empty() const { return _services.empty(); }

    INLINE void ServiceList::reserve(size_t size) { _services.reserve(size); }

    INLINE void ServiceList::seal()
    {
        _services.shrink_to_fit();
        _sealed = true;
    }

    INLINE bool ServiceList::isSealed() const { return _sealed; }
} // namespace sb::di::details