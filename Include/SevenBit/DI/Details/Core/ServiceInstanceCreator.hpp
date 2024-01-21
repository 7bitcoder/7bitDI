#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstanceList.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details::core
{
    class EXPORT ServiceInstanceCreator
    {
        ServiceProvider &_provider;
        containers::ServiceDescriptorList &_descriptors;

      public:
        using Ptr = std::unique_ptr<ServiceInstanceProvider>;

        explicit ServiceInstanceCreator(ServiceProvider &provider, containers::ServiceDescriptorList &descriptors);

        const IServiceInstance *tryCreateAndRegister();

        const OneOrList<IServiceInstance::Ptr> *tryCreateAndRegisterAll(
            const containers::ServiceDescriptorList &descriptors);

        IServiceInstance::Ptr tryCreate(const ServiceDescriptor &descriptor, bool inPlaceRequest);
        std::optional<OneOrList<IServiceInstance::Ptr>> tryCreateAll(
            const containers::ServiceDescriptorList &descriptors);

        OneOrList<IServiceInstance::Ptr> &createRestInstances(const containers::ServiceDescriptorList &descriptors,
                                                              containers::ServiceInstanceList &instances,
                                                              bool inPlaceRequest);

        IServiceInstance::Ptr createInstance(const ServiceDescriptor &descriptor, bool inPlaceRequest);

        containers::ServiceInstancesMap *tryGetInstancesMap(const ServiceLifeTime &lifeTime);
    };
} // namespace sb::di::details::core

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Core/Impl/ServiceInstanceCreator.hpp"
#endif
