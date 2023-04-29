#pragma once

#include <functional>
#include <memory>

#include "SevenBit/_Internal/ExternalService.hpp"
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

    template <class T> class ExternalServiceFcnFactory final : public IServiceFactory
    {
      public:
        using FactoryFcn = std::function<T *(ServiceProvider &)>;

      private:
        FactoryFcn _factoryFunction;

      public:
        ExternalServiceFcnFactory(FactoryFcn factoryFunction) : _factoryFunction{std::move(factoryFunction)} {}

        TypeId getServiceTypeId() const { return typeid(T); }

        IServiceInstance::Ptr get(ServiceProvider &serviceProvider) const
        {
            return std::make_unique<ExternalService<T>>(_factoryFunction(serviceProvider));
        }

        IServiceFactory::Ptr clone() { return std::make_unique<ExternalServiceFcnFactory<T>>(*this); }
    };

} // namespace sb