#pragma once

#include <memory>
#include <utility>

#include <SevenBit/DI/LibraryConfig.hpp>

#include <SevenBit/DI/Details/Core/ServiceInstancesCreator.hpp>
#include <SevenBit/DI/Details/Utils/Require.hpp>
#include <SevenBit/DI/Details/Utils/RequireDescriptor.hpp>
#include <SevenBit/DI/Details/Utils/RequireInstance.hpp>

namespace sb::di::details
{
    INLINE ServiceInstance ServiceInstancesCreator::create(const ServiceDescriptor &descriptor)
    {
        return create(descriptor, false);
    }

    INLINE ServiceInstanceList ServiceInstancesCreator::createAll(const ServiceDescriptorList &descriptors,
                                                                  const size_t skipLast)
    {
        return createAll(descriptors, false, skipLast);
    }

    INLINE ServiceInstance ServiceInstancesCreator::createInPlace(const ServiceDescriptor &descriptor)
    {
        return create(descriptor, true);
    }

    INLINE ServiceInstanceList ServiceInstancesCreator::createAllInPlace(const ServiceDescriptorList &descriptors,
                                                                         const size_t skipLast)
    {
        return createAll(descriptors, true, skipLast);
    }

    INLINE ServiceInstanceList ServiceInstancesCreator::createAll(const ServiceDescriptorList &descriptors,
                                                                  const bool inPlaceRequest, const size_t skipLast)
    {
        ServiceInstanceList instances;
        const auto take = descriptors.size() - skipLast;
        instances.reserve(take);
        descriptors.forEach([&](const ServiceDescriptor &descriptor, const size_t index) {
            if (index < take)
            {
                instances.add(create(descriptor, inPlaceRequest));
            }
        });
        return instances;
    }

    INLINE ServiceInstance ServiceInstancesCreator::create(const ServiceDescriptor &descriptor,
                                                           const bool inPlaceRequest)
    {
        RequireDescriptor::nonAlias(descriptor);
        auto &provider = *Require::notNullAndGet(_serviceProvider);
        const auto &factory = *Require::notNullAndGet(descriptor.getImplementationFactory());
        auto _ = _circularDependencyGuard(descriptor.getImplementationTypeId());

        return RequireInstance::validAndGet(
            ServiceInstance{factory.createInstance(provider, inPlaceRequest), descriptor.getCastOffset()});
    }
} // namespace sb::di::details
