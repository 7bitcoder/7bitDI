#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstanceList.hpp"
#include "SevenBit/DI/Details/Core/ServiceInstanceCreator.hpp"

namespace sb::di::details::core
{
    class EXPORT ServiceInstancesResolver
    {
        ServiceInstanceCreator &_creator;
        const containers::ServiceDescriptorList &_descriptors;

      public:
        ServiceInstancesResolver(ServiceInstanceCreator &creator, const containers::ServiceDescriptorList &descriptors);

        [[nodiscard]] IServiceInstance::Ptr createInstance() const;

        [[nodiscard]] containers::ServiceInstanceList createOneInstance() const;

        [[nodiscard]] containers::ServiceInstanceList createAllInstances() const;

        containers::ServiceInstanceList &createRestInstances(containers::ServiceInstanceList &instances) const;

        [[nodiscard]] IServiceInstance::Ptr createInstanceInPlace() const;

        [[nodiscard]] containers::ServiceInstanceList createOneInstanceInPlace() const;

        [[nodiscard]] containers::ServiceInstanceList createAllInstancesInPlace() const;

        containers::ServiceInstanceList &createRestInstancesInPlace(containers::ServiceInstanceList &instances) const;

        [[nodiscard]] IServiceInstance::Ptr createAlias(const IServiceInstance &instance) const;

        [[nodiscard]] containers::ServiceInstanceList createOneAliase(
            const OneOrList<IServiceInstance::Ptr> &instances) const;

        [[nodiscard]] containers::ServiceInstanceList createAllAliases(
            const OneOrList<IServiceInstance::Ptr> &instances) const;

        [[nodiscard]] containers::ServiceInstanceList &createRestAliases(
            const OneOrList<IServiceInstance::Ptr> &instances, containers::ServiceInstanceList &toFill) const;

      private:
        [[nodiscard]] IServiceInstance::Ptr createInstance(bool inPlaceRequest) const;

        [[nodiscard]] containers::ServiceInstanceList createOneInstance(bool inPlaceRequest) const;

        [[nodiscard]] containers::ServiceInstanceList createAllInstances(bool inPlaceRequest) const;

        containers::ServiceInstanceList &createRestInstances(containers::ServiceInstanceList &instances,
                                                             bool inPlaceRequest) const;
    };
} // namespace sb::di::details::core

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Core/Impl/ServiceInstancesResolver.hpp"
#endif
