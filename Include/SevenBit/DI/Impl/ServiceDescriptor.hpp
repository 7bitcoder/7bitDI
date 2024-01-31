#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/Require.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"

namespace sb::di
{
    INLINE ServiceDescriptor::ServiceDescriptor(const TypeId serviceTypeId, const ServiceLifeTime lifeTime,
                                                IServiceFactory::Ptr implementationFactory, const int castOffset,
                                                const bool isAlias)
        : _serviceTypeId(serviceTypeId), _lifeTime(lifeTime), _isAlias(isAlias),
          _implementationFactory(std::move(implementationFactory)), _castOffset(castOffset)

    {
        details::utils::Require::notNull(_implementationFactory, "Implementation factory cannot be null");
    }

    INLINE ServiceLifeTime ServiceDescriptor::getLifeTime() const { return _lifeTime; }

    INLINE TypeId ServiceDescriptor::getServiceTypeId() const { return _serviceTypeId; }

    INLINE TypeId ServiceDescriptor::getImplementationTypeId() const
    {
        return getImplementationFactory().getServiceTypeId();
    }

    INLINE const IServiceFactory &ServiceDescriptor::getImplementationFactory() const
    {
        return *_implementationFactory;
    }

    INLINE int ServiceDescriptor::getCastOffset() const { return _castOffset; }

    INLINE bool ServiceDescriptor::isAlias() const { return _isAlias; }

    INLINE bool ServiceDescriptor::operator==(const ServiceDescriptor &descriptor) const
    {
        return _isAlias == descriptor.isAlias() && _serviceTypeId == descriptor.getServiceTypeId() &&
               _lifeTime == descriptor.getLifeTime() &&
               _implementationFactory.get() == &descriptor.getImplementationFactory();
    }

    INLINE bool ServiceDescriptor::operator!=(const ServiceDescriptor &descriptor) const
    {
        return !(*this == descriptor);
    }

} // namespace sb::di
