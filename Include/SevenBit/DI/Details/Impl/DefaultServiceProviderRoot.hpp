#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/DefaultServiceProviderRoot.hpp"
#include "SevenBit/DI/Details/ServicesMap.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"

namespace sb::di::details
{
    INLINE const ServiceDescriptorsMap &DefaultServiceProviderRoot::getDescriptorsMap() { return _descriptorsMap; }

    INLINE ServicesMap &DefaultServiceProviderRoot::getSingletons() { return _singletons; }

    INLINE void DefaultServiceProviderRoot::prebuildSingeletons()
    {
        for (auto &[_, list] : getDescriptorsMap())
        {
            for (auto &descriptor : list)
            {
                if (descriptor.getLifeTime().isSingleton())
                {
                    DefaultServiceProvider::getInstance(descriptor.getServiceTypeId());
                }
            }
        }
    }

    INLINE DefaultServiceProviderRoot::~DefaultServiceProviderRoot()
    {
        clear(); // clear scoped first
        // then member singeletons will be cleared
    }
} // namespace sb::di::details
