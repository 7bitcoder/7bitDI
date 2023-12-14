#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceInstanceProviderData.hpp"

namespace sb::di::details::core
{
    INLINE const containers::ServiceDescriptorsMap &ServiceInstanceProviderData::getDescriptorsMap() const
    {
        return _descriptorsMap;
    }

    INLINE containers::ServiceInstancesMap &ServiceInstanceProviderData::getSingletons() { return _singletons; }

    INLINE const ServiceProviderOptions &ServiceInstanceProviderData::getOptions() const { return _options; }

} // namespace sb::di::details::core
