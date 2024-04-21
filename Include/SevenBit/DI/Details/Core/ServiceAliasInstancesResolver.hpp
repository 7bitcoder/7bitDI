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

        [[nodiscard]] std::optional<ServiceInstanceList> createOne(const ServiceInstance *original) const;

        template <class TFactory> std::optional<ServiceInstanceList> createAll(TFactory originalFactory) const
        {
            std::optional<ServiceInstanceList> result;
            _descriptors.getInnerList().forEach([&](const ServiceDescriptor &aliasDescriptor) {
                if (const OneOrList<ServiceInstance> *originals = originalFactory(aliasDescriptor))
                {
                    createAll(aliasDescriptor, *originals, result);
                }
            });
            if (result)
            {
                result->seal();
            }
            return result;
        }

        template <class TFactory>
        ServiceInstanceList &createRest(ServiceInstanceList &instances, TFactory originalFactory) const
        {
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
                        createRest(aliasDescriptor, *originals, instances);
                    }
                }
            });
            instances.seal();
            return instances;
        }

        [[nodiscard]] ServiceInstance map(ServiceInstance &&original) const;

        template <class TFactory> std::optional<OneOrList<ServiceInstance>> mapAll(TFactory originalFactory) const
        {
            std::optional<OneOrList<ServiceInstance>> result;
            _descriptors.getInnerList().forEach([&](const ServiceDescriptor &aliasDescriptor) {
                if (std::optional<OneOrList<ServiceInstance>> originals = originalFactory(aliasDescriptor))
                {
                    if (aliasDescriptor.getCastOffset())
                    {
                        originals->forEach([&](ServiceInstance &instance) {
                            instance.addCastOffset(aliasDescriptor.getCastOffset());
                        });
                    }
                    if (!result)
                    {
                        result = std::move(originals);
                    }
                    else
                    {
                        result->addRange(std::move(*originals));
                    }
                }
            });
            return result;
        }

      private:
        [[nodiscard]] ServiceInstance create(const ServiceDescriptor &descriptor,
                                             const ServiceInstance &original) const;

        void createAll(const ServiceDescriptor &descriptor, const OneOrList<ServiceInstance> &originals,
                       std::optional<ServiceInstanceList> &instances) const;

        void createAll(const ServiceDescriptor &descriptor, const OneOrList<ServiceInstance> &originals,
                       ServiceInstanceList &instances) const;

        void createRest(const ServiceDescriptor &descriptor, const OneOrList<ServiceInstance> &originals,
                        ServiceInstanceList &instances) const;
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Core/Impl/ServiceAliasInstancesResolver.hpp"
#endif
