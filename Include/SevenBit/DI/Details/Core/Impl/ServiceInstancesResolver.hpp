#pragma once

#include <memory>
#include <utility>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceInstancesResolver.hpp"
#include "SevenBit/DI/Details/Services/AliasService.hpp"

namespace sb::di::details::core
{
    INLINE ServiceInstancesResolver::ServiceInstancesResolver(ServiceInstanceCreator &creator,
                                                              const containers::ServiceDescriptorList &descriptors)
        : _creator(creator), _descriptors(descriptors)
    {
    }

    INLINE IServiceInstance::Ptr ServiceInstancesResolver::createInstance() const { return createInstance(false); }

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

    INLINE IServiceInstance::Ptr ServiceInstancesResolver::createInstanceInPlace() const
    {
        return createInstance(true);
    }

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

    INLINE IServiceInstance::Ptr ServiceInstancesResolver::createAlias(const IServiceInstance &instance) const
    {
        return std::make_unique<services::AliasService>(instance.get(), _descriptors.last().getImplementationTypeId());
    }

    INLINE containers::ServiceInstanceList ServiceInstancesResolver::createOneAliase(
        const OneOrList<IServiceInstance::Ptr> &instances) const
    {
        return containers::ServiceInstanceList{createAlias(*instances.last())};
    }

    INLINE containers::ServiceInstanceList ServiceInstancesResolver::createAllAliases(
        const OneOrList<IServiceInstance::Ptr> &instances) const
    {
        containers::ServiceInstanceList aliases{createAlias(*instances.first())};
        return std::move(createRestAliases(instances, aliases));
    }

    INLINE containers::ServiceInstanceList &ServiceInstancesResolver::createRestAliases(
        const OneOrList<IServiceInstance::Ptr> &instances, containers::ServiceInstanceList &toFill) const
    {
        if (instances.size() > 1)
        {
            auto &list = instances.getAsList();
            toFill.reserve(list.size());
            const auto end = list.end();
            for (auto it = ++list.begin(); it != end; ++it) // skip first and last
            {
                toFill.add(createAlias(**it));
            }
        }
        toFill.seal();
        return toFill;
    }

    INLINE IServiceInstance::Ptr ServiceInstancesResolver::createInstance(const bool inPlaceRequest) const
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
        if (_descriptors.size() > 1)
        {
            instances.reserve(_descriptors.size());
            auto realFirst = _creator.createInstance(_descriptors.first(), inPlaceRequest);
            const auto end = --_descriptors.end();
            for (auto it = ++_descriptors.begin(); it != end; ++it) // skip first and last
            {
                instances.add(_creator.createInstance(*it, inPlaceRequest));
            }
            instances.add(std::move(realFirst));
            instances.first().swap(instances.last());
        }
        instances.seal();
        return instances;
    }
} // namespace sb::di::details::core
