#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/OneOrList.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details::containers
{
    class EXPORT ServiceDescriptorList
    {
        OneOrList<ServiceDescriptor> _oneOrList;

      public:
        explicit ServiceDescriptorList(ServiceDescriptor &&descriptor);

        ServiceDescriptorList(const ServiceDescriptorList &) = delete;
        ServiceDescriptorList(ServiceDescriptorList &&) = default;

        ServiceDescriptorList &operator=(const ServiceDescriptorList &) = delete;
        ServiceDescriptorList &operator=(ServiceDescriptorList &&) = default;

        OneOrList<ServiceDescriptor> &getInnerList();
        [[nodiscard]] const OneOrList<ServiceDescriptor> &getInnerList() const;

        void add(ServiceDescriptor &&descriptor);

        [[nodiscard]] auto begin() const { return _oneOrList.getAsList().begin(); }
        [[nodiscard]] auto end() const { return _oneOrList.getAsList().end(); }

        [[nodiscard]] const ServiceDescriptor &first() const;

        [[nodiscard]] const ServiceDescriptor &last() const;

        [[nodiscard]] bool empty() const;

        [[nodiscard]] size_t size() const;

        [[nodiscard]] ServiceLifeTime getLifeTime() const;

        [[nodiscard]] TypeId getServiceTypeId() const;

        [[nodiscard]] bool isAlias() const;

        void seal();

      private:
        void checkBaseType(const ServiceDescriptor &descriptor) const;
        void checkAlias(const ServiceDescriptor &descriptor) const;
        void checkLifeTime(const ServiceDescriptor &descriptor) const;
    };

} // namespace sb::di::details::containers

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Containers/Impl/ServiceDescriptorList.hpp"
#endif
