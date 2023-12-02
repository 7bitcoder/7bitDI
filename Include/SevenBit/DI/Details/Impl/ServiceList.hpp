#pragma once

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/ServiceList.hpp"
#include "SevenBit/DI/Details/Utils.hpp"

namespace sb::di::details
{
    INLINE ServiceList::ServiceList(size_t size) : OneOrList<IServiceInstance::Ptr>(size) {}

    INLINE ServiceList::ServiceList(IServiceInstance::Ptr instance)
        : OneOrList<IServiceInstance::Ptr>(utils::Assert::serviceAndGet(std::move(instance)))
    {
    }

    INLINE ServiceList &ServiceList::add(IServiceInstance::Ptr &&service)
    {
        OneOrList<IServiceInstance::Ptr>::add(utils::Assert::serviceAndMove(std::move(service)));
        return *this;
    }

    INLINE void ServiceList::seal()
    {
        shrink();
        _sealed = true;
    }

    INLINE bool ServiceList::isSealed() const { return _sealed; }

} // namespace sb::di::details
