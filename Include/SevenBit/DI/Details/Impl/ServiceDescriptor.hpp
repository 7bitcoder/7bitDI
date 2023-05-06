#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"

namespace sb::di
{
    INLINE ServiceDescriptor::ServiceDescriptor(TypeId serviceTypeId, ServiceLifeTime lifetime,
                                                IServiceFactory::Ptr implementationFactory)
        : _serviceTypeId(serviceTypeId), _lifetime(lifetime), _implementationFactory(std::move(implementationFactory))
    {
        if (!_implementationFactory)
        {
            throw NullPointnerException{"Implementation factory cannot be null"};
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
} // namespace sb::di
