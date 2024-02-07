#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/RequireBase.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"

namespace sb::di
{
    INLINE ServiceDescriptor::ServiceDescriptor(const TypeId serviceTypeId, const TypeId implementationTypeId,
                                                const ServiceLifeTime lifeTime,
                                                IServiceFactory::Ptr implementationFactory, const ptrdiff_t castOffset)
        : _serviceTypeId(serviceTypeId), _implementationTypeId(implementationTypeId), _lifeTime(lifeTime),
          _implementationFactory(std::move(implementationFactory)), _castOffset(castOffset)

    {
    }

    INLINE TypeId ServiceDescriptor::getServiceTypeId() const { return _serviceTypeId; }

    INLINE TypeId ServiceDescriptor::getImplementationTypeId() const { return _implementationTypeId; }

    INLINE ServiceLifeTime ServiceDescriptor::getLifeTime() const { return _lifeTime; }

    INLINE const IServiceFactory &ServiceDescriptor::getImplementationFactory() const
    {
        return *details::utils::RequireBase::notNullAndGet(_implementationFactory.get());
    }

    INLINE ptrdiff_t ServiceDescriptor::getCastOffset() const { return _castOffset; }

    INLINE bool ServiceDescriptor::isAlias() const { return !_implementationFactory; }

    INLINE bool ServiceDescriptor::operator==(const ServiceDescriptor &descriptor) const
    {
        return _serviceTypeId == descriptor._serviceTypeId &&
               _implementationTypeId == descriptor._implementationTypeId && _lifeTime == descriptor._lifeTime &&
               _implementationFactory == descriptor._implementationFactory && _castOffset == descriptor._castOffset;
    }

    INLINE bool ServiceDescriptor::operator!=(const ServiceDescriptor &descriptor) const
    {
        return !(*this == descriptor);
    }

} // namespace sb::di
