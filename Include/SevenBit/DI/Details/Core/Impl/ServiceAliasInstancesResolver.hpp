#pragma once

#include <memory>
#include <utility>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceAliasInstancesResolver.hpp"

namespace sb::di::details
{
    INLINE ServiceAliasInstancesResolver::ServiceAliasInstancesResolver(ServiceInstanceCreator &creator,
                                                                        const ServiceDescriptorList &descriptors)
        : _creator(creator), _descriptors(descriptors)
    {
    }

    INLINE ServiceInstanceList ServiceAliasInstancesResolver::createOne(const ServiceInstance *original) const
    {
        return original ? ServiceInstanceList{create(_descriptors.last(), *original)} : ServiceInstanceList{};
    }

    INLINE ServiceInstance ServiceAliasInstancesResolver::map(ServiceInstance &&original) const
    {
        original.addCastOffset(_descriptors.last().getCastOffset());
        return std::move(original);
    }

    INLINE ServiceInstance ServiceAliasInstancesResolver::create(const ServiceDescriptor &descriptor,
                                                                 const ServiceInstance &original) const
    {
        return _creator.createInstanceAlias(descriptor, original);
    }

    INLINE void ServiceAliasInstancesResolver::createAll(const ServiceDescriptor &descriptor,
                                                         const OneOrList<ServiceInstance> &originals,
                                                         ServiceInstanceList &instances) const
    {
        instances.reserve(instances.size() + originals.size());
        originals.forEach([&](const ServiceInstance &instance) { instances.add(create(descriptor, instance)); });
    }

    INLINE void ServiceAliasInstancesResolver::createRest(const ServiceDescriptor &descriptor,
                                                          const OneOrList<ServiceInstance> &originals,
                                                          ServiceInstance &&first, ServiceInstanceList &instances) const
    {
        if (const auto size = originals.size(); size > 1)
        {
            instances.reserve(instances.size() + size);
            originals.forEach([&](const ServiceInstance &instance, const std::size_t index) {
                if (index < size - 1) // skip last
                {
                    instances.add(create(descriptor, instance));
                }
            });
        }
        instances.add(std::move(first));
    }
} // namespace sb::di::details
