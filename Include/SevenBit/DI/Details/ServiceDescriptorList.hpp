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
    class EXPORT ServiceDescriptorList : public OptimalList<ServiceDescriptor>
    {
      public:
        ServiceDescriptorList() = default;
        ServiceDescriptorList() = default;

        ServiceDescriptorList(const ServiceDescriptorList &) = delete;
        ServiceDescriptorList(ServiceDescriptorList &&) = default;

        ServiceDescriptorList &operator=(ServiceDescriptorList &&) = default;
        ServiceDescriptorList &operator=(const ServiceDescriptorList &) = delete;

        const ServiceLifeTime &getLifeTime() const;

        TypeId getServiceTypeId() const;

        bool contains(TypeId typeId);

      private:
        void checkBaseType(ServiceDescriptor &descriptor);
        void checkLifeTime(ServiceDescriptor &descriptor);
    };

} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Impl/ServiceDescriptorList.hpp"
#endif
