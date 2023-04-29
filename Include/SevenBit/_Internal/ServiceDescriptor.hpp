#pragma once
#include <memory>
#include <typeindex>
#include <unordered_set>

#include "SevenBit/_Internal/Exceptions.hpp"
#include "SevenBit/_Internal/IServiceCreator.hpp"
#include "SevenBit/_Internal/IServiceFactory.hpp"
#include "SevenBit/_Internal/ServiceLifeTime.hpp"
#include "SevenBit/_Internal/TypeId.hpp"

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

        ServiceDescriptor(TypeId serviceTypeId, ServiceLifeTime lifetime, IServiceFactory::Ptr implementationFactory)
            : _serviceTypeId(serviceTypeId), _lifetime(lifetime),
              _implementationFactory(std::move(implementationFactory))
        {
            if (!_implementationFactory)
            {
                // todo throw "sth"
            }
        }

        ServiceDescriptor(const ServiceDescriptor &other)
            : ServiceDescriptor(other._serviceTypeId, other._lifetime,
                                other._implementationFactory ? other._implementationFactory->clone() : nullptr)
        {
        }

        ServiceDescriptor(ServiceDescriptor &&) = default;

        ServiceDescriptor &operator=(const ServiceDescriptor &other)
        {
            _serviceTypeId = other._serviceTypeId;
            _lifetime = other._lifetime;
            _implementationFactory = other._implementationFactory ? other._implementationFactory->clone() : nullptr;
            return *this;
        }
        ServiceDescriptor &operator=(ServiceDescriptor &&other) = default;

        const ServiceLifeTime &getLifeTime() const { return _lifetime; }

        TypeId getServiceTypeId() const { return _serviceTypeId; }

        TypeId getImplementationTypeId() const { return _implementationFactory->getServiceTypeId(); }

        const IServiceFactory &getImplementationFactory() const { return *_implementationFactory; }
    };
} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/ScopedGuard.hpp"
#endif