#pragma once

#include <SevenBit/DI/LibraryConfig.hpp>

#include <SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp>
#include <SevenBit/DI/Details/Containers/ServiceInstanceList.hpp>

namespace sb::di::details
{
    class EXPORT ServiceAliasesCreator
    {
      public:
        ServiceInstance tryCreate(const ServiceDescriptor &descriptor, const ServiceInstance *original) const;

        template <class TResolver>
        ServiceInstanceList tryCreateAll(const ServiceDescriptorList &descriptors, TResolver originalResolver) const
        {
            ServiceInstanceList instances;
            descriptors.forEach([&](const ServiceDescriptor &aliasDescriptor) {
                tryCreateAll(instances, aliasDescriptor, originalResolver(aliasDescriptor));
            });
            return instances;
        }

        template <class TResolver>
        void tryCreateRest(const ServiceDescriptorList &descriptors, ServiceInstanceList &instances,
                           TResolver originalResolver) const
        {
            auto first = std::move(instances.first());
            instances.clear();
            const auto size = descriptors.size();
            descriptors.forEach([&](const ServiceDescriptor &aliasDescriptor, const std::size_t index) {
                index < size - 1
                    ? tryCreateAll(instances, aliasDescriptor, originalResolver(aliasDescriptor))
                    : tryCreateRest(instances, aliasDescriptor, originalResolver(aliasDescriptor), std::move(first));
            });
        }

        ServiceInstance tryMap(const ServiceDescriptor &descriptor, ServiceInstance &&original) const;

        template <class TResolver>
        ServiceInstanceList tryMapAll(const ServiceDescriptorList &descriptors, TResolver originalResolver) const
        {
            ServiceInstanceList instances;
            descriptors.forEach([&](const ServiceDescriptor &aliasDescriptor) {
                tryMapAll(instances, aliasDescriptor, originalResolver(aliasDescriptor));
            });
            return instances;
        }

      private:
        void tryCreateAll(ServiceInstanceList &instances, const ServiceDescriptor &descriptor,
                          const OneOrList<ServiceInstance> *originals) const;

        void tryCreateRest(ServiceInstanceList &instances, const ServiceDescriptor &descriptor,
                           const OneOrList<ServiceInstance> *originals, ServiceInstance &&first) const;

        void tryMapAll(ServiceInstanceList &instances, const ServiceDescriptor &descriptor,
                       OneOrList<ServiceInstance> &&originals) const;

        ServiceInstance create(const ServiceDescriptor &descriptor, const ServiceInstance &original) const;
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include <SevenBit/DI/Details/Core/Impl/ServiceAliasesCreator.hpp>
#endif
