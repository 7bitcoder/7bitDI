#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/DefaultServiceProviderCore.hpp"

namespace sb::di::details
{
    INLINE const ServiceDescriptorsMap &DefaultServiceProviderCore::getDescriptorsMap() { return _descriptorsMap; }

    INLINE ServicesMap &DefaultServiceProviderCore::getSingletons() { return _singletons; }

    INLINE const ServiceProviderOptions &DefaultServiceProviderCore::getOptions() { return _options; }

} // namespace sb::di::details
