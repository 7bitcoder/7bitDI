#pragma once

#include <memory>
#include <utility>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceInstancesResolver.hpp"

namespace sb::di::details
{
    INLINE ServiceInstancesResolver::ServiceInstancesResolver(ServiceInstanceCreator &creator,
                                                              const ServiceDescriptorList &descriptors)
        : _creator(creator), _descriptors(descriptors)
    {
    }

    INLINE ServiceInstance ServiceInstancesResolver::create() const { return create(false); }

    INLINE ServiceInstanceList ServiceInstancesResolver::createOne() const { return createOne(false); }

    INLINE ServiceInstanceList ServiceInstancesResolver::createAll() const { return createAll(false); }

    INLINE ServiceInstanceList &ServiceInstancesResolver::createRest(ServiceInstanceList &instances) const
    {
        return createRest(instances, false);
    }

    INLINE ServiceInstance ServiceInstancesResolver::createInPlace() const { return create(true); }

    INLINE ServiceInstanceList ServiceInstancesResolver::createOneInPlace() const { return createOne(true); }

    INLINE ServiceInstanceList ServiceInstancesResolver::createAllInPlace() const { return createAll(true); }

    INLINE ServiceInstanceList &ServiceInstancesResolver::createRestInPlace(ServiceInstanceList &instances) const
    {
        return createRest(instances, true);
    }

    INLINE ServiceInstance ServiceInstancesResolver::create(const bool inPlaceRequest) const
    {
        return _creator.createInstance(_descriptors.last(), inPlaceRequest);
    }

    INLINE ServiceInstanceList ServiceInstancesResolver::createOne(const bool inPlaceRequest) const
    {
        ServiceInstanceList instances{_creator.createInstance(_descriptors.last(), inPlaceRequest)};
        if (_descriptors.size() == 1)
        {
            instances.seal();
        }
        return instances;
    }

    INLINE ServiceInstanceList ServiceInstancesResolver::createAll(const bool inPlaceRequest) const
    {
        ServiceInstanceList instances{_creator.createInstance(_descriptors.first(), inPlaceRequest)};
        if (const auto size = _descriptors.size(); size > 1)
        {
            instances.reserve(size);
            _descriptors.getInnerList().forEach([&](const ServiceDescriptor &descriptor, const std::size_t index) {
                if (index) // skip first
                {
                    instances.add(_creator.createInstance(descriptor, inPlaceRequest));
                }
            });
        }
        instances.seal();
        return instances;
    }

    INLINE ServiceInstanceList &ServiceInstancesResolver::createRest(ServiceInstanceList &instances,
                                                                     const bool inPlaceRequest) const
    {
        if (const auto size = _descriptors.size(); size > 1)
        {
            instances.reserve(size);
            auto first = _creator.createInstance(_descriptors.first(), inPlaceRequest);
            _descriptors.getInnerList().forEach([&](const ServiceDescriptor &descriptor, const std::size_t index) {
                if (index && index < size - 1) // skip first and last
                {
                    instances.add(_creator.createInstance(descriptor, inPlaceRequest));
                }
            });
            instances.add(std::move(first));
            std::swap(instances.first(), instances.last());
        }
        instances.seal();
        return instances;
    }
} // namespace sb::di::details
