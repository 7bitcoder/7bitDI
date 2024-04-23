#pragma once

#include <SevenBit/DI/LibraryConfig.hpp>

#include <SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp>
#include <SevenBit/DI/Details/Containers/ServiceInstanceList.hpp>
#include <SevenBit/DI/Details/Helpers/CircularDependencyGuard.hpp>

namespace sb::di::details
{
    class EXPORT ServiceInstancesCreator
    {
        ServiceProvider *_serviceProvider = nullptr;
        CircularDependencyGuard _circularDependencyGuard;

      public:
        void setServiceProvider(ServiceProvider &serviceProvider) { _serviceProvider = &serviceProvider; }

        ServiceInstance create(const ServiceDescriptor &descriptor);
        ServiceInstanceList createAll(const ServiceDescriptorList &descriptors);
        void createRest(const ServiceDescriptorList &descriptors, ServiceInstanceList &instances);

        ServiceInstance createInPlace(const ServiceDescriptor &descriptor);
        ServiceInstanceList createAllInPlace(const ServiceDescriptorList &descriptors);
        void createRestInPlace(const ServiceDescriptorList &descriptors, ServiceInstanceList &instances);

      private:
        ServiceInstance create(const ServiceDescriptor &descriptor, bool inPlaceRequest);
        ServiceInstanceList createAll(const ServiceDescriptorList &descriptors, bool inPlaceRequest);
        void createRest(const ServiceDescriptorList &descriptors, ServiceInstanceList &instances, bool inPlaceRequest);
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include <SevenBit/DI/Details/Core/Impl/ServiceInstancesCreator.hpp>
#endif
