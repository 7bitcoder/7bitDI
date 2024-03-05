#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/CircularDependencyGuard.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"
#include "SevenBit/DI/ServiceInstance.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

namespace sb::di::details
{
    class EXPORT ServiceInstanceCreator
    {
        ServiceProvider *_serviceProvider = nullptr;
        CircularDependencyGuard _circularDependencyGuard;

      public:
        void setServiceProvider(ServiceProvider &serviceProvider);

        ServiceInstance createInstance(const ServiceDescriptor &descriptor, bool inPlaceRequest);

        ServiceInstance createInstanceAlias(const ServiceDescriptor &descriptor, const ServiceInstance &instance);

      private:
        ServiceInstance createInstance(IServiceInstance::Ptr &&implementation, ptrdiff_t castOffset);
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Core/Impl/ServiceInstanceCreator.hpp"
#endif
