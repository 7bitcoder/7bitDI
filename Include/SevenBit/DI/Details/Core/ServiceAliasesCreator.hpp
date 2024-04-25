#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstanceList.hpp"

namespace sb::di::details
{
    class EXPORT ServiceAliasesCreator
    {
      public:
        ServiceInstance tryCreate(const ServiceDescriptor &descriptor, const ServiceInstance *original) const;

        template <class TResolver>
        ServiceInstanceList tryCreateAll(const ServiceDescriptorList &descriptors, TResolver originalsResolver,
                                         const std::size_t skipLast = 0) const
        {
            ServiceInstanceList instances;
            const auto size = descriptors.size();
            instances.reserve(size);
            descriptors.forEach([&](const ServiceDescriptor &aliasDescriptor, const std::size_t index) {
                const auto lastDescriptorSkip = index + 1 == size ? skipLast : 0;
                tryCreateAll(instances, aliasDescriptor, originalsResolver(aliasDescriptor), lastDescriptorSkip);
            });
            return instances;
        }

        ServiceInstance tryMap(const ServiceDescriptor &descriptor, ServiceInstance &&original) const;

        template <class TResolver>
        ServiceInstanceList tryMapAll(const ServiceDescriptorList &descriptors, TResolver originalsResolver) const
        {
            ServiceInstanceList instances;
            descriptors.forEach([&](const ServiceDescriptor &aliasDescriptor) {
                tryMapAll(instances, aliasDescriptor, originalsResolver(aliasDescriptor));
            });
            return instances;
        }

      private:
        void tryCreateAll(ServiceInstanceList &instances, const ServiceDescriptor &descriptor,
                          const OneOrList<ServiceInstance> *originals, std::size_t skipLast = 0) const;

        void tryMapAll(ServiceInstanceList &instances, const ServiceDescriptor &descriptor,
                       OneOrList<ServiceInstance> &&originals) const;

        [[nodiscard]] ServiceInstance create(const ServiceDescriptor &descriptor,
                                             const ServiceInstance &original) const;
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Core/Impl/ServiceAliasesCreator.hpp"
#endif
