#pragma once

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
        using OneOrList::OneOrList;

        ServiceDescriptorList(const ServiceDescriptorList &) = delete;
        ServiceDescriptorList(ServiceDescriptorList &&) = default;

        ServiceDescriptorList &operator=(const ServiceDescriptorList &) = delete;
        ServiceDescriptorList &operator=(ServiceDescriptorList &&) = default;

        void add(ServiceDescriptor &&descriptor);

        [[nodiscard]] ServiceLifeTime getLifeTime() const;

        [[nodiscard]] TypeId getServiceTypeId() const;

        [[nodiscard]] const std::string *getServiceKey() const;

        [[nodiscard]] bool isAlias() const;

      private:
        void checkBaseType(const ServiceDescriptor &descriptor) const;
        void checkKey(const ServiceDescriptor &descriptor) const;
        void checkAlias(const ServiceDescriptor &descriptor) const;
        void checkLifeTime(const ServiceDescriptor &descriptor) const;
    };

} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Containers/Impl/ServiceDescriptorList.hpp"
#endif
