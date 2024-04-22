#pragma once

#include <memory>
#include <utility>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceInstancesCreator.hpp"
#include "SevenBit/DI/Details/Utils/Require.hpp"
#include "SevenBit/DI/Details/Utils/RequireDescriptor.hpp"
#include "SevenBit/DI/Details/Utils/RequireInstance.hpp"

namespace sb::di::details
{
    INLINE ServiceInstance ServiceInstancesCreator::create(const ServiceDescriptor &descriptor)
    {
        return create(descriptor, false);
    }

    INLINE ServiceInstanceList ServiceInstancesCreator::createAll(const ServiceDescriptorList &descriptors)
    {
        return createAll(descriptors, false);
    }

    INLINE void ServiceInstancesCreator::createRest(const ServiceDescriptorList &descriptors,
                                                    ServiceInstanceList &instances)
    {
        return createRest(descriptors, instances, false);
    }

    INLINE ServiceInstance ServiceInstancesCreator::createInPlace(const ServiceDescriptor &descriptor)
    {
        return create(descriptor, true);
    }

    INLINE ServiceInstanceList ServiceInstancesCreator::createAllInPlace(const ServiceDescriptorList &descriptors)
    {
        return createAll(descriptors, true);
    }

    INLINE void ServiceInstancesCreator::createRestInPlace(const ServiceDescriptorList &descriptors,
                                                           ServiceInstanceList &instances)
    {
        return createRest(descriptors, instances, true);
    }

    INLINE ServiceInstanceList ServiceInstancesCreator::createAll(const ServiceDescriptorList &descriptors,
                                                                  const bool inPlaceRequest)
    {
        ServiceInstanceList instances;
        instances.reserve(descriptors.size());
        descriptors.getInnerList().forEach(
            [&](const ServiceDescriptor &descriptor) { instances.add(create(descriptor, inPlaceRequest)); });
        return instances;
    }

    INLINE void ServiceInstancesCreator::createRest(const ServiceDescriptorList &descriptors,
                                                    ServiceInstanceList &instances, const bool inPlaceRequest)
    {
        if (const auto size = descriptors.size(); size > 1)
        {
            instances.reserve(size);
            auto first = create(descriptors.first(), inPlaceRequest);
            descriptors.getInnerList().forEach([&](const ServiceDescriptor &descriptor, const std::size_t index) {
                if (index && index < size - 1) // skip first and last
                {
                    instances.add(create(descriptor, inPlaceRequest));
                }
            });
            instances.add(std::move(first));
            std::swap(instances.first(), instances.last());
        }
    }

    INLINE ServiceInstance ServiceInstancesCreator::create(const ServiceDescriptor &descriptor,
                                                           const bool inPlaceRequest)
    {
        RequireDescriptor::nonAlias(descriptor);
        auto &provider = *Require::notNullAndGet(_serviceProvider);
        const auto &factory = *Require::notNullAndGet(descriptor.getImplementationFactory());
        auto _ = _circularDependencyGuard(descriptor.getImplementationTypeId());

        auto implementation = factory.createInstance(provider, inPlaceRequest);
        return RequireInstance::validAndGet(ServiceInstance{std::move(implementation), descriptor.getCastOffset()});
    }
} // namespace sb::di::details
