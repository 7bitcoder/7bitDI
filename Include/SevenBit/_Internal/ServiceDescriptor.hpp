#pragma once
#include <memory>
#include <unordered_set>

#include "SevenBit/_Internal/Exceptions.hpp"
#include "SevenBit/_Internal/IServiceAccessor.hpp"
#include "SevenBit/_Internal/IServiceCreator.hpp"
#include "SevenBit/_Internal/ServiceLifeTime.hpp"
#include "SevenBit/_Internal/TypeId.hpp"

namespace sb
{
    class ServiceDescriptor
    {
      private:
        const ServiceLifeTime _lifetime;

        TypeId _serviceTypeId;
        TypeId _implementationTypeId;

        IServiceCreator::Ptr _creator;
        IServiceAccessor::Ptr _accesor;

      public:
        using Ptr = std::unique_ptr<ServiceDescriptor>;

        ServiceDescriptor(TypeId typeIdUnderConstruction, std::unordered_set<TypeId> &typeIdsUnderConstruction);

        const ServiceLifeTime &getLifeTime() const { return _lifetime; }

        const TypeId &getServiceTypeId() const { return _serviceTypeId; }

        const TypeId &getImplementationTypeId() const { return _implementationTypeId; }

        const IServiceCreator *getImplementationCreator() const { return _creator.get(); }

        const IServiceAccessor *getImplementationAccessor() const { return _accesor.get(); }
    };

} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/ScopedGuard.hpp"
#endif