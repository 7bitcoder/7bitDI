#pragma once

#include <algorithm>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/OptimalList.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details
{
    class EXPORT ServiceDescriptorList
    {
      private:
        OptimalList<ServiceDescriptor> _descriptors;

      public:
        explicit ServiceDescriptorList(ServiceDescriptor &&descriptor);

        [[nodiscard]] auto begin() const { return _descriptors.getAsList().begin(); }
        [[nodiscard]] auto end() const { return _descriptors.getAsList().end(); }

        [[nodiscard]] auto rBegin() const { return _descriptors.getAsList().rbegin(); }
        [[nodiscard]] auto rEnd() const { return _descriptors.getAsList().rend(); }

        void add(ServiceDescriptor &&descriptor);

        ServiceDescriptor &last();

        [[nodiscard]] const ServiceDescriptor &last() const;

        [[nodiscard]] const ServiceDescriptor &operator[](size_t index) const;

        [[nodiscard]] const ServiceLifeTime &getLifeTime() const;

        [[nodiscard]] TypeId getServiceTypeId() const;

        [[nodiscard]] size_t size() const;

        void seal();

      private:
        void checkBaseType(ServiceDescriptor &descriptor) const;
        void checkLifeTime(ServiceDescriptor &descriptor) const;
    };

} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Impl/ServiceDescriptorList.hpp"
#endif
