#pragma once

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/ServiceDescriptor.hpp"

namespace sb
{
    INLINE ServiceDescriptor::ServiceDescriptor(TypeId serviceTypeId, ServiceLifeTime lifetime,
                                                IServiceFactory::Ptr implementationFactory)
        : _serviceTypeId(serviceTypeId), _lifetime(lifetime), _implementationFactory(std::move(implementationFactory))
    {
        if (!_implementationFactory)
        {
            // todo throw "sth"
        }
    }

    INLINE ServiceDescriptor::ServiceDescriptor(const ServiceDescriptor &other)
        : ServiceDescriptor(other._serviceTypeId, other._lifetime,
                            other._implementationFactory ? other._implementationFactory->clone() : nullptr)
    {
    }

    INLINE ServiceDescriptor &ServiceDescriptor::operator=(const ServiceDescriptor &other)
    {
        _serviceTypeId = other._serviceTypeId;
        _lifetime = other._lifetime;
        _implementationFactory = other._implementationFactory ? other._implementationFactory->clone() : nullptr;
        return *this;
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
} // namespace sb
