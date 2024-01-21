#pragma once

#include <memory>
#include <utility>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceInstancesCreator.hpp"
#include "SevenBit/DI/Details/Core/ServiceInstanceProviderRoot.hpp"

namespace sb::di::details::core
{
    INLINE ServiceInstancesCreator::ServiceInstancesCreator(ServiceInstanceProviderRoot &root,
                                                              ServiceProvider &provider, const bool isThisRoot,
                                                              const containers::ServiceDescriptorList &descriptors)
        : _root(root), _serviceProvider(provider), _descriptors(descriptors), _isThisRoot(isThisRoot)
    {
    }

    INLINE IServiceInstance::Ptr ServiceInstancesCreator::createInstance() { return createInstance(false); }

    INLINE containers::ServiceInstanceList ServiceInstancesCreator::createOneInstance()
    {
        return createOneInstance(false);
    }

    INLINE containers::ServiceInstanceList ServiceInstancesCreator::createAllInstances()
    {
        return createAllInstances(false);
    }

    INLINE containers::ServiceInstanceList &ServiceInstancesCreator::createRestInstances(
        containers::ServiceInstanceList &instances)
    {
        return createRestInstances(instances, false);
    }

    INLINE IServiceInstance::Ptr ServiceInstancesCreator::createInstanceInPlace() { return createInstance(true); }

    INLINE containers::ServiceInstanceList ServiceInstancesCreator::createOneInstanceInPlace()
    {
        return createOneInstance(true);
    }

    INLINE containers::ServiceInstanceList ServiceInstancesCreator::createAllInstancesInPlace()
    {
        return createAllInstances(true);
    }

    INLINE containers::ServiceInstanceList &ServiceInstancesCreator::createRestInstancesInPlace(
        containers::ServiceInstanceList &instances)
    {
        return createRestInstances(instances, true);
    }

    INLINE IServiceInstance::Ptr ServiceInstancesCreator::createInstance(const bool inPlaceRequest)
    {
        return createInstance(_descriptors.last(), inPlaceRequest);
    }

    INLINE containers::ServiceInstanceList ServiceInstancesCreator::createOneInstance(const bool inPlaceRequest)
    {
        containers::ServiceInstanceList instances{createInstance(_descriptors.last(), inPlaceRequest)};
        if (_descriptors.size() == 1)
        {
            instances.seal();
        }
        return instances;
    }

    INLINE containers::ServiceInstanceList ServiceInstancesCreator::createAllInstances(const bool inPlaceRequest)
    {
        containers::ServiceInstanceList instances{createInstance(_descriptors.last(), inPlaceRequest)};
        return std::move(createRestInstances(instances, true));
    }

    INLINE containers::ServiceInstanceList &ServiceInstancesCreator::createRestInstances(
        containers::ServiceInstanceList &instances, const bool inPlaceRequest)
    {
        if (_descriptors.size() > 1)
        {
            instances.reserve(_descriptors.size());
            auto realFirst = createInstance(_descriptors.first(), inPlaceRequest);
            const auto end = --_descriptors.end();
            for (auto it = ++_descriptors.begin(); it != end; ++it) // skip first and last
            {
                instances.add(createInstance(*it, inPlaceRequest));
            }
            instances.add(std::move(realFirst));
            instances.first().swap(instances.last());
        }
        instances.seal();
        return instances;
    }

    INLINE IServiceInstance::Ptr ServiceInstancesCreator::createInstance(const ServiceDescriptor &descriptor,
                                                                          const bool inPlaceRequest)
    {
        if (descriptor.getLifeTime().isSingleton() && !_isThisRoot)
        {
            return _root.createInstance(descriptor, inPlaceRequest);
        }
        auto _ = _root.spawnGuard(descriptor.getImplementationTypeId());
        return utils::Require::validInstanceAndGet(
            descriptor.getImplementationFactory().createInstance(_serviceProvider, inPlaceRequest));
    }
} // namespace sb::di::details::core
