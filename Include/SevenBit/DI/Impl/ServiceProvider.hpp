#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/RequireBase.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

namespace sb::di
{
    INLINE ServiceProvider::ServiceProvider(IServiceInstanceProvider::Ptr instanceProvider)
        : _instanceProvider(std::move(instanceProvider))
    {
        details::RequireBase::notNull(_instanceProvider);
        getInstanceProvider().init(*this);
    }

    INLINE const ServiceProviderOptions &ServiceProvider::getOptions() const
    {
        return getInstanceProvider().getOptions();
    }

    INLINE ServiceProvider ServiceProvider::createScope() const
    {
        return ServiceProvider{getInstanceProvider().createScope()};
    }

    INLINE ServiceProvider::Ptr ServiceProvider::createScopeAsPtr() const
    {
        return std::make_unique<ServiceProvider>(getInstanceProvider().createScope());
    }

    INLINE const IServiceInstanceProvider &ServiceProvider::getInstanceProvider() const { return *_instanceProvider; }

    INLINE IServiceInstanceProvider &ServiceProvider::getInstanceProvider() { return *_instanceProvider; }

} // namespace sb::di
