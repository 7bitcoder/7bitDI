#pragma once

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/ServiceInstanceList.hpp"
#include "SevenBit/DI/Details/Utils.hpp"

namespace sb::di::details
{
    INLINE ServiceInstanceList::ServiceInstanceList(size_t size) : OneOrList<IServiceInstance::Ptr>(size) {}

    INLINE ServiceInstanceList::ServiceInstanceList(IServiceInstance::Ptr instance)
        : OneOrList<IServiceInstance::Ptr>(utils::Assert::serviceAndGet(std::move(instance)))
    {
    }

    INLINE ServiceInstanceList &ServiceInstanceList::add(IServiceInstance::Ptr &&service)
    {
        OneOrList<IServiceInstance::Ptr>::add(utils::Assert::serviceAndMove(std::move(service)));
        return *this;
    }

    INLINE void ServiceInstanceList::seal()
    {
        shrink();
        _sealed = true;
    }

    INLINE bool ServiceInstanceList::isSealed() const { return _sealed; }

} // namespace sb::di::details
