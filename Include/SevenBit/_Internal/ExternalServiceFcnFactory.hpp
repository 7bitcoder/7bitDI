#pragma once

#include <functional>
#include <memory>

#include "SevenBit/_Internal/ExternalService.hpp"
#include "SevenBit/_Internal/IServiceFactory.hpp"
#include "SevenBit/_Internal/IServiceInstance.hpp"
#include "SevenBit/_Internal/ServiceOwner.hpp"
#include "SevenBit/_Internal/TypeId.hpp"

namespace sb
{
    template <class T, class FactoryFcn> class ExternalServiceFcnFactory final : public IServiceFactory
    {
      private:
        FactoryFcn _factoryFunction;

      public:
        ExternalServiceFcnFactory(FactoryFcn factoryFunction) : _factoryFunction{std::move(factoryFunction)} {}

        TypeId getServiceTypeId() const { return typeid(T); }

        IServiceInstance::Ptr createInstance(IServiceProvider &serviceProvider) const
        {
            return std::make_unique<ExternalService<T>>(_factoryFunction(serviceProvider));
        }

        IServiceFactory::Ptr clone() { return std::make_unique<ExternalServiceFcnFactory<T, FactoryFcn>>(*this); }
    };

} // namespace sb