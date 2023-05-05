#pragma once

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/ServiceProviderOptions.hpp"
#include "SevenBit/_Internal/ServiceProviderRoot.hpp"
#include "SevenBit/_Internal/ServicesMap.hpp"

namespace sb::internal
{
    INLINE const ServiceDescriptorsMap &ServiceProviderRoot::getDescriptorsMap() { return _descriptorsMap; }

    INLINE ServicesMap &ServiceProviderRoot::getSingletons() { return _singletons; }

    INLINE void ServiceProviderRoot::prebuildSingeletons()
    {
        for (auto &[_, list] : getDescriptorsMap())
        {
            for (auto &descriptor : list)
            {
                if (descriptor.getLifeTime().isSingleton())
                {
                    ServiceProvider::getService(descriptor.getServiceTypeId());
                }
            }
        }
    }

    INLINE ServiceProviderRoot::~ServiceProviderRoot()
    {
        clear(); // clear scoped first
        // then member singeletons will be cleared
    }
} // namespace sb::internal
