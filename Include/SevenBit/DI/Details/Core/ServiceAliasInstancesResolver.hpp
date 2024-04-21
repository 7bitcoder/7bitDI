#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstanceList.hpp"
#include "SevenBit/DI/Details/Core/ServiceInstanceCreator.hpp"

namespace sb::di::details
{
    class EXPORT ServiceAliasInstancesResolver
    {
        ServiceInstanceCreator &_creator;
        const ServiceDescriptorList &_descriptors;

      public:
        ServiceAliasInstancesResolver(ServiceInstanceCreator &creator, const ServiceDescriptorList &descriptors);

        [[nodiscard]] ServiceInstanceList createOne(const ServiceInstance *original) const;

        template <class TFactory> ServiceInstanceList createAll(TFactory originalFactory) const
        {
            ServiceInstanceList instances;
            _descriptors.getInnerList().forEach([&](const ServiceDescriptor &aliasDescriptor) {
                if (const OneOrList<ServiceInstance> *originals = originalFactory(aliasDescriptor))
                {
                    createAll(aliasDescriptor, *originals, instances);
                }
            });
            instances.seal();
            return instances;
        }

        template <class TFactory>
        ServiceInstanceList &createRest(ServiceInstanceList &instances, TFactory originalFactory) const
        {
            auto first = std::move(instances.first());
            instances = ServiceInstanceList{};
            const auto size = _descriptors.size();
            _descriptors.getInnerList().forEach([&](const ServiceDescriptor &aliasDescriptor, const std::size_t index) {
                if (const OneOrList<ServiceInstance> *originals = originalFactory(aliasDescriptor))
                {
                    if (index < size - 1) // not last
                    {
                        createAll(aliasDescriptor, *originals, instances);
                    }
                    else
                    {
                        createRest(aliasDescriptor, *originals, std::move(first), instances);
                    }
                }
            });
            instances.seal();
            return instances;
        }

        [[nodiscard]] ServiceInstance map(ServiceInstance &&original) const;

        template <class TFactory> OneOrList<ServiceInstance> mapAll(TFactory originalFactory) const
        {
            OneOrList<ServiceInstance> instances;
            _descriptors.getInnerList().forEach([&](const ServiceDescriptor &aliasDescriptor) {
                if (OneOrList<ServiceInstance> originals = originalFactory(aliasDescriptor))
                {
                    if (aliasDescriptor.getCastOffset())
                    {
                        originals.forEach([&](ServiceInstance &instance) {
                            instance.addCastOffset(aliasDescriptor.getCastOffset());
                        });
                    }
                    instances.add(std::move(originals));
                }
            });
            return instances;
        }

      private:
        [[nodiscard]] ServiceInstance create(const ServiceDescriptor &descriptor,
                                             const ServiceInstance &original) const;

        void createAll(const ServiceDescriptor &descriptor, const OneOrList<ServiceInstance> &originals,
                       ServiceInstanceList &instances) const;

        void createRest(const ServiceDescriptor &descriptor, const OneOrList<ServiceInstance> &originals,
                        ServiceInstance &&first, ServiceInstanceList &instances) const;
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Core/Impl/ServiceAliasInstancesResolver.hpp"
#endif
