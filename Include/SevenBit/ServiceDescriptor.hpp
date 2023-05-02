#pragma once

#include <memory>

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/Exceptions.hpp"
#include "SevenBit/IServiceFactory.hpp"
#include "SevenBit/ServiceLifeTime.hpp"
#include "SevenBit/TypeId.hpp"

namespace sb
{
    class ServiceDescriptor
    {
      private:
        ServiceLifeTime _lifetime;

        TypeId _serviceTypeId;

        IServiceFactory::Ptr _implementationFactory;

      public:
        using Ptr = std::unique_ptr<ServiceDescriptor>;

        ServiceDescriptor(TypeId serviceTypeId, ServiceLifeTime lifetime, IServiceFactory::Ptr implementationFactory);

        ServiceDescriptor(const ServiceDescriptor &other);

        ServiceDescriptor(ServiceDescriptor &&) = default;

        ServiceDescriptor &operator=(const ServiceDescriptor &other);
        ServiceDescriptor &operator=(ServiceDescriptor &&other) = default;

        const ServiceLifeTime &getLifeTime() const;

        TypeId getServiceTypeId() const;

        TypeId getImplementationTypeId() const;

        const IServiceFactory &getImplementationFactory() const;
    };
} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/ServiceDescriptor.hpp"
#endif