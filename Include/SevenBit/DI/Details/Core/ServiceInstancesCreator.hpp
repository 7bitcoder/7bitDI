#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstanceList.hpp"
#include "SevenBit/DI/Details/Helpers/CircularDependencyGuard.hpp"

namespace sb::di::details
{
    class EXPORT ServiceInstancesCreator
    {
        ServiceProvider *_serviceProvider = nullptr;
        CircularDependencyGuard _circularDependencyGuard;

      public:
        void setServiceProvider(ServiceProvider &serviceProvider) { _serviceProvider = &serviceProvider; }

        [[nodiscard]] ServiceInstance create(const ServiceDescriptor &descriptor);
        [[nodiscard]] ServiceInstanceList createAll(const ServiceDescriptorList &descriptors);
        ServiceInstanceList &createRest(const ServiceDescriptorList &descriptors, ServiceInstanceList &instances);

        [[nodiscard]] ServiceInstance createInPlace(const ServiceDescriptor &descriptor);
        [[nodiscard]] ServiceInstanceList createAllInPlace(const ServiceDescriptorList &descriptors);
        ServiceInstanceList &createRestInPlace(const ServiceDescriptorList &descriptors,
                                               ServiceInstanceList &instances);

      private:
        [[nodiscard]] ServiceInstance create(const ServiceDescriptor &descriptor, bool inPlaceRequest);
        [[nodiscard]] ServiceInstanceList createAll(const ServiceDescriptorList &descriptors, bool inPlaceRequest);
        ServiceInstanceList &createRest(const ServiceDescriptorList &descriptors, ServiceInstanceList &instances,
                                        bool inPlaceRequest);
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Core/Impl/ServiceInstancesCreator.hpp"
#endif
