#pragma once

#include <functional>
#include <memory>

#include "SevenBit/_Internal/IServiceCreator.hpp"
#include "SevenBit/_Internal/IServiceFactory.hpp"
#include "SevenBit/_Internal/IServiceHolder.hpp"
#include "SevenBit/_Internal/IServiceInstance.hpp"
#include "SevenBit/_Internal/ServiceOwner.hpp"
#include "SevenBit/_Internal/ServiceOwner2.hpp"
#include "SevenBit/_Internal/TypeId.hpp"

namespace sb
{
    class ServiceProvider;

    template <class T> class ServiceFcnFactory final : public IServiceFactory
    {
      public:
        using FactoryFcn = std::function<std::unique_ptr<T>(ServiceProvider &)>;

      private:
        FactoryFcn _factoryFunction;

      public:
        ServiceFcnFactory(FactoryFcn factoryFunction) : _factoryFunction{std::move(factoryFunction)} {}

        TypeId getServiceTypeId() const { return typeid(T); }

        IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider) const
        {
            return std::make_unique<ServiceOwner2<T>>(_factoryFunction(serviceProvider));
        }

        IServiceFactory::Ptr clone() { return std::make_unique<ServiceFcnFactory<T>>(*this); }
    };

} // namespace sb