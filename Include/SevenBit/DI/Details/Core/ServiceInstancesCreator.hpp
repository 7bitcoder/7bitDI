#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstanceList.hpp"
#include "SevenBit/DI/Details/Core/ServiceInstancesCreatorCtx.hpp"
#include "SevenBit/DI/Details/Helpers/CircularDependencyGuard.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"

namespace sb::di::details::core
{
    class EXPORT ServiceInstancesCreator
    {
        ServiceInstancesCreatorCtx _ctx;

      public:
        explicit ServiceInstancesCreator(const ServiceInstancesCreatorCtx &ctx);

        ServiceInstancesCreator(ServiceInstancesCreator &&) = default;

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

        IServiceInstance::Ptr createInstance(const ServiceDescriptor &descriptor, bool inPlaceRequest);
    };
} // namespace sb::di::details::core

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Core/Impl/ServiceInstancesCreator.hpp"
#endif
