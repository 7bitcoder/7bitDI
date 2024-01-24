#pragma once

#include <memory>
#include <utility>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceInstanceProviderRoot.hpp"
#include "SevenBit/DI/Details/Core/ServiceInstancesCreator.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

namespace sb::di::details::core
{
    INLINE ServiceInstancesCreator::ServiceInstancesCreator(const ServiceInstancesCreatorCtx &ctx) : _ctx(ctx) {}

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
        return createInstance(_ctx.getDescriptors().last(), inPlaceRequest);
    }

    INLINE containers::ServiceInstanceList ServiceInstancesCreator::createOneInstance(const bool inPlaceRequest)
    {
        auto &desctiptors = _ctx.getDescriptors();
        containers::ServiceInstanceList instances{createInstance(desctiptors.last(), inPlaceRequest)};
        if (desctiptors.size() == 1)
        {
            instances.seal();
        }
        return instances;
    }

    INLINE containers::ServiceInstanceList ServiceInstancesCreator::createAllInstances(const bool inPlaceRequest)
    {
        containers::ServiceInstanceList instances{createInstance(_ctx.getDescriptors().last(), inPlaceRequest)};
        return std::move(createRestInstances(instances, true));
    }

    INLINE containers::ServiceInstanceList &ServiceInstancesCreator::createRestInstances(
        containers::ServiceInstanceList &instances, const bool inPlaceRequest)
    {
        if (auto &desctiptors = _ctx.getDescriptors(); desctiptors.size() > 1)
        {
            instances.reserve(desctiptors.size());
            auto realFirst = createInstance(desctiptors.first(), inPlaceRequest);
            const auto end = --desctiptors.end();
            for (auto it = ++desctiptors.begin(); it != end; ++it) // skip first and last
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
        auto _ = _ctx.getGuard().spawnGuard(descriptor.getImplementationTypeId());
        return utils::Require::validInstanceAndGet(
            descriptor.getImplementationFactory().createInstance(_ctx.getServiceProvider(), inPlaceRequest));
    }
} // namespace sb::di::details::core
