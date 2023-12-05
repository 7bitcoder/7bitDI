#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/DefaultServiceProviderData.hpp"

namespace sb::di::details::core
{
    INLINE const containers::ServiceDescriptorsMap &DefaultServiceProviderData::getDescriptorsMap() const
    {
        return _descriptorsMap;
    }

    INLINE containers::ServiceInstancesMap &DefaultServiceProviderData::getSingletons() { return _singletons; }

    INLINE const ServiceProviderOptions &DefaultServiceProviderData::getOptions() const { return _options; }

} // namespace sb::di::details::core
