#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/DefaultServiceProviderCore.hpp"

namespace sb::di::details
{
    INLINE const ServiceDescriptorsMap &DefaultServiceProviderCore::getDescriptorsMap() const
    {
        return _descriptorsMap;
    }

    INLINE ServiceInstancesMap &DefaultServiceProviderCore::getSingletons() { return _singletons; }

    INLINE const ServiceProviderOptions &DefaultServiceProviderCore::getOptions() const { return _options; }

} // namespace sb::di::details
