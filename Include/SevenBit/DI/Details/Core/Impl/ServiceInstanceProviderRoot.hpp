#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceInstanceProviderRoot.hpp"

namespace sb::di::details
{
    INLINE ServiceInstanceProviderRoot::ServiceInstanceProviderRoot(ServiceProviderOptions options)
        : ServiceInstanceProvider(*this, options), _descriptorsMap(options.checkServiceGlobalUniqueness),
          _singletons(options.strongDestructionOrder)
    {
    }

    INLINE void ServiceInstanceProviderRoot::init(ServiceProvider &serviceProvider)
    {
        ServiceInstanceProvider::init(serviceProvider);
        _descriptorsMap.seal();
        if (getOptions().threadSafe)
        {
            _mutex = std::make_unique<std::recursive_mutex>();
        }
        if (getOptions().prebuildSingletons)
        {
            prebuildSingletons();
        }
    }

    INLINE void ServiceInstanceProviderRoot::prebuildSingletons()
    {
        for (auto &[id, descriptors] : getDescriptorsMap())
        {
            if (!descriptors.isAlias() && descriptors.getLifeTime().isSingleton())
            {
                tryGetInstances(id);
            }
        }
    }
} // namespace sb::di::details
