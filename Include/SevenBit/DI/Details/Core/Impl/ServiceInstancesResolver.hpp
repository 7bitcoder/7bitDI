#pragma once

#include <algorithm>
#include <memory>
#include <utility>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceInstancesResolver.hpp"

namespace sb::di::details::core
{
    INLINE ServiceInstancesResolver::ServiceInstancesResolver(ServiceInstanceCreator &creator,
                                                              const containers::ServiceDescriptorList &descriptors)
        : _creator(creator), _descriptors(descriptors)
    {
    }

    INLINE ServiceInstance ServiceInstancesResolver::createInstance() const { return createInstance(false); }

    INLINE containers::ServiceInstanceList ServiceInstancesResolver::createOneInstance() const
    {
        return createOneInstance(false);
    }

    INLINE containers::ServiceInstanceList ServiceInstancesResolver::createAllInstances() const
    {
        return createAllInstances(false);
    }

    INLINE containers::ServiceInstanceList &ServiceInstancesResolver::createRestInstances(
        containers::ServiceInstanceList &instances) const
    {
        return createRestInstances(instances, false);
    }

    INLINE ServiceInstance ServiceInstancesResolver::createInstanceInPlace() const { return createInstance(true); }

    INLINE containers::ServiceInstanceList ServiceInstancesResolver::createOneInstanceInPlace() const
    {
        return createOneInstance(true);
    }

    INLINE containers::ServiceInstanceList ServiceInstancesResolver::createAllInstancesInPlace() const
    {
        return createAllInstances(true);
    }

    INLINE containers::ServiceInstanceList &ServiceInstancesResolver::createRestInstancesInPlace(
        containers::ServiceInstanceList &instances) const
    {
        return createRestInstances(instances, true);
    }

    INLINE ServiceInstance ServiceInstancesResolver::createAlias(const ServiceInstance &original) const
    {
        return createAlias(&original);
    }

    INLINE containers::ServiceInstanceList ServiceInstancesResolver::createOneAlias(
        const ServiceInstance &original) const
    {
        return containers::ServiceInstanceList{createAlias(&original)};
    }

    INLINE containers::ServiceInstanceList ServiceInstancesResolver::createAllAliases(
        const OneOrList<ServiceInstance> &originals) const
    {
        containers::ServiceInstanceList aliases{createAlias(originals.last())};
        return std::move(createRestAliases(originals, aliases));
    }

    INLINE containers::ServiceInstanceList &ServiceInstancesResolver::createRestAliases(
        const OneOrList<ServiceInstance> &originals, containers::ServiceInstanceList &instances) const
    {
        if (const auto size = originals.size(); size > 1)
        {
            instances.reserve(size);
            auto realFirst = createAlias(originals.first());
            originals.forEach([&](const ServiceInstance &instance, const size_t index) {
                if (index && index < size - 1) // skip first and last
                {
                    instances.add(createAlias(instance));
                }
            });
            instances.add(std::move(realFirst));
            std::swap(instances.first(), instances.last());
        }
        instances.seal();
        return instances;
    }

    INLINE ServiceInstance ServiceInstancesResolver::createInstance(const bool inPlaceRequest) const
    {
        return _creator.createInstance(_descriptors.last(), inPlaceRequest);
    }

    INLINE containers::ServiceInstanceList ServiceInstancesResolver::createOneInstance(const bool inPlaceRequest) const
    {
        containers::ServiceInstanceList instances{_creator.createInstance(_descriptors.last(), inPlaceRequest)};
        if (_descriptors.size() == 1)
        {
            instances.seal();
        }
        return instances;
    }

    INLINE containers::ServiceInstanceList ServiceInstancesResolver::createAllInstances(const bool inPlaceRequest) const
    {
        containers::ServiceInstanceList instances{_creator.createInstance(_descriptors.last(), inPlaceRequest)};
        return std::move(createRestInstances(instances, inPlaceRequest));
    }

    INLINE containers::ServiceInstanceList &ServiceInstancesResolver::createRestInstances(
        containers::ServiceInstanceList &instances, const bool inPlaceRequest) const
    {
        if (const auto size = _descriptors.size(); size > 1)
        {
            instances.reserve(size);
            auto realFirst = _creator.createInstance(_descriptors.first(), inPlaceRequest);
            _descriptors.getInnerList().forEach([&](const ServiceDescriptor &descriptor, const size_t index) {
                if (index && index < size - 1) // skip first and last
                {
                    instances.add(_creator.createInstance(descriptor, inPlaceRequest));
                }
            });
            instances.add(std::move(realFirst));
            std::swap(instances.first(), instances.last());
        }
        instances.seal();
        return instances;
    }

    INLINE ServiceInstance ServiceInstancesResolver::createAlias(const ServiceInstance *original) const
    {
        return _creator.createInstanceAlias(_descriptors.last(), original);
    }

} // namespace sb::di::details::core
