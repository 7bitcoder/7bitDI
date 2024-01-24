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

        IServiceInstance::Ptr createInstance();

        containers::ServiceInstanceList createOneInstance();

        containers::ServiceInstanceList createAllInstances();

        containers::ServiceInstanceList &createRestInstances(containers::ServiceInstanceList &instances);

        IServiceInstance::Ptr createInstanceInPlace();

        containers::ServiceInstanceList createOneInstanceInPlace();

        containers::ServiceInstanceList createAllInstancesInPlace();

        containers::ServiceInstanceList &createRestInstancesInPlace(containers::ServiceInstanceList &instances);

      private:
        IServiceInstance::Ptr createInstance(bool inPlaceRequest);

        containers::ServiceInstanceList createOneInstance(bool inPlaceRequest);

        containers::ServiceInstanceList createAllInstances(bool inPlaceRequest);

        containers::ServiceInstanceList &createRestInstances(containers::ServiceInstanceList &instances,
                                                             bool inPlaceRequest);
    };
} // namespace sb::di::details::core

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Core/Impl/ServiceInstancesResolver.hpp"
#endif
