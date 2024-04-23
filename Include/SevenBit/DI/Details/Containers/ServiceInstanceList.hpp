#pragma once

#include <SevenBit/DI/LibraryConfig.hpp>

#include <SevenBit/DI/OneOrList.hpp>
#include <SevenBit/DI/ServiceInstance.hpp>

namespace sb::di::details
{
    class ServiceInstanceList : public OneOrList<ServiceInstance>
    {
        bool _sealed = false;

      public:
        using OneOrList::OneOrList;

        ServiceInstanceList(const ServiceInstanceList &) = delete;
        ServiceInstanceList(ServiceInstanceList &&) = default;

        ServiceInstanceList &operator=(const ServiceInstanceList &) = delete;
        ServiceInstanceList &operator=(ServiceInstanceList &&) = default;

        void seal() { _sealed = true; }
        [[nodiscard]] bool isSealed() const { return _sealed; }
    };
} // namespace sb::di::details
