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

        void add(IServiceInstance::Ptr &&service);

        OneOrList<IServiceInstance::Ptr> &getInnerList();
        [[nodiscard]] const OneOrList<IServiceInstance::Ptr> &getInnerList() const;

        IServiceInstance::Ptr &first();
        [[nodiscard]] const IServiceInstance::Ptr &first() const;

        IServiceInstance::Ptr &last();
        [[nodiscard]] const IServiceInstance::Ptr &last() const;

        IServiceInstance::Ptr &operator[](size_t index);
        const IServiceInstance::Ptr &operator[](size_t index) const;

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
