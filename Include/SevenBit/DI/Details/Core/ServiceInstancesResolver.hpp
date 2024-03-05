#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstanceList.hpp"
#include "SevenBit/DI/Details/Core/ServiceInstanceCreator.hpp"

namespace sb::di::details
{
    class EXPORT ServiceInstancesResolver
    {
        ServiceInstanceCreator &_creator;
        const ServiceDescriptorList &_descriptors;

      public:
        ServiceInstancesResolver(ServiceInstanceCreator &creator, const ServiceDescriptorList &descriptors);

        [[nodiscard]] ServiceInstance createInstance() const;
        [[nodiscard]] ServiceInstanceList createOneInstance() const;
        [[nodiscard]] ServiceInstanceList createAllInstances() const;
        ServiceInstanceList &createRestInstances(ServiceInstanceList &instances) const;

        [[nodiscard]] ServiceInstance createInstanceInPlace() const;
        [[nodiscard]] ServiceInstanceList createOneInstanceInPlace() const;
        [[nodiscard]] ServiceInstanceList createAllInstancesInPlace() const;
        ServiceInstanceList &createRestInstancesInPlace(ServiceInstanceList &instances) const;

        [[nodiscard]] ServiceInstance createAlias(const ServiceInstance &original) const;
        [[nodiscard]] ServiceInstanceList createOneAlias(const ServiceInstance &original) const;
        [[nodiscard]] ServiceInstanceList createAllAliases(const OneOrList<ServiceInstance> &originals) const;
        [[nodiscard]] ServiceInstanceList &createRestAliases(const OneOrList<ServiceInstance> &originals,
                                                             ServiceInstanceList &instances) const;

      private:
        [[nodiscard]] ServiceInstance createInstance(bool inPlaceRequest) const;
        [[nodiscard]] ServiceInstanceList createOneInstance(bool inPlaceRequest) const;
        [[nodiscard]] ServiceInstanceList createAllInstances(bool inPlaceRequest) const;
        ServiceInstanceList &createRestInstances(ServiceInstanceList &instances, bool inPlaceRequest) const;
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Core/Impl/ServiceInstancesResolver.hpp"
#endif
