#pragma once

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/ServiceList.hpp"
#include "SevenBit/DI/Details/Utils.hpp"

namespace sb::di::details
{
    INLINE ServiceList::ServiceList(size_t size) : _services(size) {}

    INLINE ServiceList::ServiceList(IServiceInstance::Ptr instance)
        : _services(utils::Assert::serviceAndGet(std::move(instance)))
    {
    }

    INLINE ServiceList &ServiceList::add(IServiceInstance::Ptr &&service)
    {
        utils::Assert::service(service);
        _services.add(std::move(service));
        return *this;
    }

    INLINE IServiceInstance::Ptr &ServiceList::first() { return _services.first(); }

    INLINE IServiceInstance::Ptr &ServiceList::last() { return _services.last(); }

    INLINE std::vector<const IServiceInstance *> ServiceList::getAllServices() const
    {
        if (auto list = _services.tryGetAsList())
        {
            std::vector<const IServiceInstance *> result;
            result.reserve(_services.size());
            for (auto &instance : *list)
            {
                result.push_back(instance.get());
            }
            return result;
        }
        return {_services.first().get()};
    }

    INLINE size_t ServiceList::size() const { return _services.size(); }

    INLINE bool ServiceList::empty() const { return _services.empty(); }

    INLINE void ServiceList::reserve(size_t size) { _services.reserve(size); }

    INLINE void ServiceList::seal()
    {
        _services.shrink();
        _sealed = true;
    }

    INLINE bool ServiceList::isSealed() const { return _sealed; }

} // namespace sb::di::details
