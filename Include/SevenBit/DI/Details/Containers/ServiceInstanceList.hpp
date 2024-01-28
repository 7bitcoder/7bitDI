#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/OneOrList.hpp"

namespace sb::di::details::containers
{
    class EXPORT ServiceInstanceList
    {
        OneOrList<IServiceInstance::Ptr> _oneOrList;
        bool _sealed = false;

      public:
        explicit ServiceInstanceList(IServiceInstance::Ptr instance);
        explicit ServiceInstanceList(size_t size);

        ServiceInstanceList(const ServiceInstanceList &) = delete;
        ServiceInstanceList(ServiceInstanceList &&) = default;

        ServiceInstanceList &operator=(const ServiceInstanceList &) = delete;
        ServiceInstanceList &operator=(ServiceInstanceList &&) = default;

        void add(IServiceInstance::Ptr &&service);

        [[nodiscard]] auto begin() const { return _oneOrList.getAsList().begin(); }
        [[nodiscard]] auto end() const { return _oneOrList.getAsList().end(); }

        OneOrList<IServiceInstance::Ptr> &getInnerList();
        [[nodiscard]] const OneOrList<IServiceInstance::Ptr> &getInnerList() const;

        IServiceInstance::Ptr &first();
        [[nodiscard]] const IServiceInstance::Ptr &first() const;

        IServiceInstance::Ptr &last();
        [[nodiscard]] const IServiceInstance::Ptr &last() const;

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
