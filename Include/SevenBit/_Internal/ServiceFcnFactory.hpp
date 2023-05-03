#pragma once

#include <functional>
#include <memory>

#include "SevenBit/IServiceProvider.hpp"
#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/IServiceFactory.hpp"
#include "SevenBit/IServiceInstance.hpp"
#include "SevenBit/TypeId.hpp"
#include "SevenBit/_Internal/ExternalService.hpp"
#include "SevenBit/_Internal/ServiceOwner.hpp"

namespace sb::internal
{
    template <class... T> inline static constexpr bool notSupportedFactory = false;

    template <class ReturnType, class FactoryFcn> class ServiceFcnFactory final : public IServiceFactory
    {
      private:
        FactoryFcn _factoryFunction;

      public:
        using Type = ReturnType;

        ServiceFcnFactory(ServiceLifeTime lifetime, FactoryFcn factoryFunction)
            : _factoryFunction{std::move(factoryFunction)}
        {
            static_assert(
                notSupportedFactory<FactoryFcn>,
                "Factory function should have this scheme: (IServiceProvider&) -> std::unique_ptr<T> | T * it "
                "should take as argument IServiceProvider& and return std::unique_ptr<T> or pointner T *");
        }

        TypeId getServiceTypeId() const { return typeid(ReturnType); }

        IServiceInstance::Ptr createInstance(IServiceProvider &serviceProvider) const { return nullptr; }

        IServiceFactory::Ptr clone() { return std::make_unique<ServiceFcnFactory<ReturnType, FactoryFcn>>(*this); }
    };

    template <class TImplementation, class FactoryFcn>
    class ServiceFcnFactory<TImplementation *, FactoryFcn> final : public IServiceFactory
    {
      private:
        FactoryFcn _factoryFunction;

      public:
        using Type = TImplementation;

        ServiceFcnFactory(ServiceLifeTime lifetime, FactoryFcn factoryFunction)
            : _factoryFunction{std::move(factoryFunction)}
        {
            if (lifetime.isTransient())
            {
                throw ForbiddenServiceFactoryException{
                    typeid(TImplementation),
                    "Factory for transient service cannot return pointner, return std::unique_ptr<T> instead."};
            }
        }

        TypeId getServiceTypeId() const { return typeid(TImplementation); }

        IServiceInstance::Ptr createInstance(IServiceProvider &serviceProvider) const
        {
            return std::make_unique<ExternalService<TImplementation>>(_factoryFunction(serviceProvider));
        }

        IServiceFactory::Ptr clone()
        {
            return std::make_unique<ServiceFcnFactory<TImplementation *, FactoryFcn>>(*this);
        }
    };

    template <class TImplementation, class FactoryFcn>
    class ServiceFcnFactory<std::unique_ptr<TImplementation>, FactoryFcn> final : public IServiceFactory
    {
      private:
        FactoryFcn _factoryFunction;

      public:
        using Type = TImplementation;

        ServiceFcnFactory(ServiceLifeTime lifetime, FactoryFcn factoryFunction)
            : _factoryFunction{std::move(factoryFunction)}
        {
        }

        TypeId getServiceTypeId() const { return typeid(TImplementation); }

        IServiceInstance::Ptr createInstance(IServiceProvider &serviceProvider) const
        {
            return std::make_unique<ServiceOwner<TImplementation>>(_factoryFunction(serviceProvider));
        }

        IServiceFactory::Ptr clone()
        {
            return std::make_unique<ServiceFcnFactory<std::unique_ptr<TImplementation>, FactoryFcn>>(*this);
        }
    };
} // namespace sb::internal