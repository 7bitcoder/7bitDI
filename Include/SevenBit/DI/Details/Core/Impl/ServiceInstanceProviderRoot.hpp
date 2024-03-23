#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceInstanceProviderRoot.hpp"

namespace sb::di::details
{
    INLINE ServiceInstanceProviderRoot::ServiceInstanceProviderRoot(ServiceProviderOptions options)
        : ServiceInstanceProvider(*this, options), _descriptorsMap(options.checkServiceGlobalUniqueness),
          _singletons(options.strongDestructionOrder)
    {
        _descriptorsMap.seal();
    }

    INLINE void ServiceInstanceProviderRoot::init(ServiceProvider &serviceProvider)
    {
        ServiceInstanceProvider::init(serviceProvider);
        if (getOptions().prebuildSingletons)
        {
            prebuildSingletons();
        }
    }

    INLINE const ServiceDescriptorsMap &ServiceInstanceProviderRoot::getDescriptorsMap() const
    {
        return _descriptorsMap;
    }

    INLINE ServiceInstancesMap &ServiceInstanceProviderRoot::getSingletons() { return _singletons; }

    INLINE ServiceInstanceCreator &ServiceInstanceProviderRoot::getRootInstanceCreator()
    {
        return getInstanceCreator();
    }

    INLINE void ServiceInstanceProviderRoot::prebuildSingletons()
    {
        for (auto &[_, descriptors] : getDescriptorsMap())
        {
            if (!descriptors.isAlias() && descriptors.getLifeTime().isSingleton())
            {
                tryRegisterAndGet({descriptors.getServiceTypeId(), descriptors.getServiceKey()}, descriptors,
                                  tryCreateAllNonTransient(descriptors));
            }
        }
    }
} // namespace sb::di::details
