#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstanceList.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details::core
{
	class ServiceInstanceProviderRoot;

    class EXPORT ServiceInstancesCreator
    {
        ServiceInstanceProviderRoot &_root;
        ServiceProvider &_serviceProvider;
        const containers::ServiceDescriptorList &_descriptors;
        bool _isThisRoot;

      public:
        explicit ServiceInstancesCreator(ServiceInstanceProviderRoot &root, ServiceProvider &provider, bool isThisRoot,
                                         const containers::ServiceDescriptorList &descriptors);

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
