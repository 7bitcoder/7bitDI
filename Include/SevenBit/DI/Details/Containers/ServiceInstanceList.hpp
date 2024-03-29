#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/OneOrList.hpp"
#include "SevenBit/DI/ServiceInstance.hpp"

namespace sb::di::details
{
    class EXPORT ServiceInstanceList
    {
        OneOrList<ServiceInstance> _oneOrList;
        bool _sealed = false;

      public:
        explicit ServiceInstanceList(ServiceInstance instance);
        explicit ServiceInstanceList(std::size_t size);

        ServiceInstanceList(const ServiceInstanceList &) = delete;
        ServiceInstanceList(ServiceInstanceList &&) = default;

        ServiceInstanceList &operator=(const ServiceInstanceList &) = delete;
        ServiceInstanceList &operator=(ServiceInstanceList &&) = default;

        void add(ServiceInstance &&instance);

        [[nodiscard]] auto begin() const { return _oneOrList.getAsList().begin(); }
        [[nodiscard]] auto end() const { return _oneOrList.getAsList().end(); }

        OneOrList<ServiceInstance> &getInnerList();
        [[nodiscard]] const OneOrList<ServiceInstance> &getInnerList() const;

        ServiceInstance &first();
        [[nodiscard]] const ServiceInstance &first() const;

        ServiceInstance &last();
        [[nodiscard]] const ServiceInstance &last() const;

        [[nodiscard]] std::size_t size() const;

        [[nodiscard]] bool empty() const;

        void reserve(std::size_t newCapacity);

        void shrink();

        void seal();

        [[nodiscard]] bool isSealed() const;

        void clear();

        ~ServiceInstanceList() = default;
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Containers/Impl/ServiceInstanceList.hpp"
#endif
