#pragma once

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/Details/ServiceProviderRoot.hpp"
#include "SevenBit/Details/ServicesMap.hpp"
#include "SevenBit/ServiceProviderOptions.hpp"

namespace sb::details
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
                    ServiceProvider::getInstance(descriptor.getServiceTypeId());
                }
            }
        }
    }

    INLINE ServiceProviderRoot::~ServiceProviderRoot()
    {
        clear(); // clear scoped first
        // then member singeletons will be cleared
    }
} // namespace sb::details
