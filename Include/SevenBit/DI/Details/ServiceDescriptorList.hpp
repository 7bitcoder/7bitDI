#pragma once

#include <algorithm>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/OneOrList.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details
{
    class EXPORT ServiceDescriptorList : public OneOrList<ServiceDescriptor>
    {
      public:
        explicit ServiceDescriptorList(ServiceDescriptor &&descriptor);

        void add(ServiceDescriptor &&descriptor);

        [[nodiscard]] const ServiceLifeTime &getLifeTime() const;

        [[nodiscard]] TypeId getServiceTypeId() const;

        void seal();

      private:
        void checkBaseType(ServiceDescriptor &descriptor) const;
        void checkLifeTime(ServiceDescriptor &descriptor) const;
    };

} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Impl/ServiceDescriptorList.hpp"
#endif
