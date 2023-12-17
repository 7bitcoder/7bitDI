#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceInstanceProviderRoot.hpp"

namespace sb::di::details::core
{
    INLINE void ServiceInstanceProviderRoot::init(ServiceProvider &serviceProvider)
    {
        ServiceInstanceProvider::init(serviceProvider);
    }

    INLINE const containers::ServiceDescriptorsMap &ServiceInstanceProviderRoot::getDescriptorsMap() const
    {
        return _descriptorsMap;
    }

    INLINE containers::ServiceInstancesMap &ServiceInstanceProviderRoot::getSingletons() { return _singletons; }

    INLINE const ServiceProviderOptions &ServiceInstanceProviderRoot::getOptions() const { return _options; }

    INLINE IServiceInstance::Ptr ServiceInstanceProviderRoot::createInstance(const ServiceDescriptor &descriptor,
                                                                             bool inPlaceRequest)
    {
        return ServiceInstanceProvider::createInstance(descriptor, inPlaceRequest);
    }

    INLINE void ServiceInstanceProviderRoot::prebuildSingletons()
    {
        for (auto &[_, descriptors] : getDescriptorsMap())
        {
            if (descriptors.getLifeTime().isSingleton())
            {
                ServiceInstanceProvider::tryCreateAndRegisterAll(descriptors);
            }
        }
    }
} // namespace sb::di::details::core
