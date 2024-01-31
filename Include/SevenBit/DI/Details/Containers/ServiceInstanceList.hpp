#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/OneOrList.hpp"
#include "SevenBit/DI/ServiceInstance.hpp"

namespace sb::di::details::containers
{
    class EXPORT ServiceInstanceList
    {
        OneOrList<ServiceInstance::Ptr> _oneOrList;
        bool _sealed = false;

      public:
        explicit ServiceInstanceList(ServiceInstance::Ptr instance);
        explicit ServiceInstanceList(size_t size);

        ServiceInstanceList(const ServiceInstanceList &) = delete;
        ServiceInstanceList(ServiceInstanceList &&) = default;

        ServiceInstanceList &operator=(const ServiceInstanceList &) = delete;
        ServiceInstanceList &operator=(ServiceInstanceList &&) = default;

        void add(ServiceInstance::Ptr &&service);

        [[nodiscard]] auto begin() const { return _oneOrList.getAsList().begin(); }
        [[nodiscard]] auto end() const { return _oneOrList.getAsList().end(); }

        OneOrList<ServiceInstance::Ptr> &getInnerList();
        [[nodiscard]] const OneOrList<ServiceInstance::Ptr> &getInnerList() const;

        ServiceInstance::Ptr &first();
        [[nodiscard]] const ServiceInstance::Ptr &first() const;

        ServiceInstance::Ptr &last();
        [[nodiscard]] const ServiceInstance::Ptr &last() const;

        [[nodiscard]] size_t size() const;

        [[nodiscard]] bool empty() const;

        void reserve(size_t newCapacity);

        void shrink();

        void seal();

        [[nodiscard]] bool isSealed() const;

        void clear();

        ~ServiceInstanceList() = default;
    };
} // namespace sb::di::details::containers

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Containers/Impl/ServiceInstanceList.hpp"
#endif
