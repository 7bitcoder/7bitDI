#pragma once

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
        OneOrList<ServiceDescriptor> _oneOrList;

      public:
        explicit ServiceDescriptorList(ServiceDescriptor &&descriptor);

        void add(ServiceDescriptor &&descriptor);

        [[nodiscard]] auto begin() const { return _oneOrList.getAsList().begin(); }
        [[nodiscard]] auto end() const { return _oneOrList.getAsList().end(); }

        [[nodiscard]] const ServiceDescriptor &first() const;

        [[nodiscard]] const ServiceDescriptor &last() const;

        [[nodiscard]] bool empty() const;

        [[nodiscard]] size_t size() const;

        [[nodiscard]] const ServiceLifeTime &getLifeTime() const;

        [[nodiscard]] TypeId getServiceTypeId() const;

        void seal();

      private:
        void checkBaseType(const ServiceDescriptor &descriptor) const;
        void checkLifeTime(const ServiceDescriptor &descriptor) const;
    };

} // namespace sb::di::details::containers

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Containers/Impl/ServiceDescriptorList.hpp"
#endif
