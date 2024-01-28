#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/CircularDependencyGuard.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"

namespace sb::di::details::core
{
    class EXPORT ServiceInstanceCreator
    {
        ServiceProvider *_serviceProvider = nullptr;
        helpers::CircularDependencyGuard _guard;

      public:
        void setServiceProvider(ServiceProvider &serviceProvider);

        IServiceInstance::Ptr createInstance(const ServiceDescriptor &descriptor, bool inPlaceRequest);

        IServiceInstance::Ptr createInstanceAlias(TypeId serviceTypeId, const IServiceInstance *instance);
    };
} // namespace sb::di::details::core

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Core/Impl/ServiceInstanceCreator.hpp"
#endif
