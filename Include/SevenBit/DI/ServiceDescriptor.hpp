#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceFactory.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di
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
} // namespace sb::di

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/DI/Details/Impl/ServiceDescriptor.hpp"
#endif