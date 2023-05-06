#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/ServiceProviderRoot.hpp"
#include "SevenBit/DI/Details/ServicesMap.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"

namespace sb::di::details
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
} // namespace sb::di::details
