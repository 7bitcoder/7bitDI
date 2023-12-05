#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"

namespace sb::di
{
    INLINE ServiceDescriptor::ServiceDescriptor(TypeId serviceTypeId, ServiceLifeTime lifetime,
                                                IServiceFactory::SPtr implementationFactory)
        : _serviceTypeId(serviceTypeId), _lifetime(lifetime), _implementationFactory(std::move(implementationFactory))
    {
        if (!_implementationFactory)
        {
            throw NullPointerException{"Implementation factory cannot be null"};
        }
    }

    INLINE const ServiceLifeTime &ServiceDescriptor::getLifeTime() const { return _lifetime; }

    INLINE TypeId ServiceDescriptor::getServiceTypeId() const { return _serviceTypeId; }

    INLINE TypeId ServiceDescriptor::getImplementationTypeId() const
    {
        return _implementationFactory->getServiceTypeId();
    }

    INLINE const IServiceFactory &ServiceDescriptor::getImplementationFactory() const
    {
        return *_implementationFactory;
    }
} // namespace sb::di
