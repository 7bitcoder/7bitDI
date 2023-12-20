#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/Require.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"

namespace sb::di
{
    INLINE ServiceDescriptor::ServiceDescriptor(const TypeId serviceTypeId, const ServiceLifeTime lifetime,
                                                IServiceFactory::Ptr implementationFactory)
        : _serviceTypeId(serviceTypeId), _lifetime(lifetime),
          _implementationFactory(details::utils::Require::notNullAndGet(std::move(implementationFactory),
                                                                        "Implementation factory cannot be null"))
    {
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

    INLINE bool ServiceDescriptor::operator==(const ServiceDescriptor &descriptor) const
    {
        return _serviceTypeId == descriptor.getServiceTypeId() && _lifetime == descriptor.getLifeTime() &&
               _implementationFactory.get() == &descriptor.getImplementationFactory();
    }

    INLINE bool ServiceDescriptor::operator!=(const ServiceDescriptor &descriptor) const
    {
        return !(*this == descriptor);
    }

} // namespace sb::di
