#pragma once

#include <algorithm>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/OneOrList.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details::containers
{
    class EXPORT ServiceDescriptorList
    {
      private:
        OneOrList<ServiceDescriptor> _oneOrList;

      public:
        explicit ServiceDescriptorList(ServiceDescriptor &&descriptor);

        void add(ServiceDescriptor &&descriptor);

        auto begin() const { return _oneOrList.getAsList().begin(); }
        auto end() const { return _oneOrList.getAsList().end(); }

        const ServiceDescriptor &first() const;

        const ServiceDescriptor &last() const;

        bool empty() const;

        size_t size() const;

        [[nodiscard]] const ServiceLifeTime &getLifeTime() const;

        [[nodiscard]] TypeId getServiceTypeId() const;

        void seal();

      private:
        void checkBaseType(ServiceDescriptor &descriptor) const;
        void checkLifeTime(ServiceDescriptor &descriptor) const;
    };

} // namespace sb::di::details::containers

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Containers/Impl/ServiceDescriptorList.hpp"
#endif
