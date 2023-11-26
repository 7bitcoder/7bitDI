#pragma once

#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/DefaultServiceProvider.hpp"
#include "SevenBit/DI/Details/IServiceProviderRoot.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"
#include "SevenBit/DI/ServiceProviderOptions.hpp"

namespace sb::di
{
    INLINE ServiceProvider::ServiceProvider(IServiceInstanceProvider::Ptr instanceProvider)
        : _instanceProvider(std::move(instanceProvider))
    {
        if (!_instanceProvider)
        {
            throw NullPointnerException("InstanceProvider cannot be null");
        }
    }

    INLINE ServiceProvider ServiceProvider::createScope() { return {_instanceProvider->createScope()}; }

    INLINE IServiceInstanceProvider &ServiceProvider::getInstanceProvider() { return *_instanceProvider; }

    INLINE const IServiceInstanceProvider &ServiceProvider::getInstanceProvider() const { return *_instanceProvider; }
    
} // namespace sb::di
