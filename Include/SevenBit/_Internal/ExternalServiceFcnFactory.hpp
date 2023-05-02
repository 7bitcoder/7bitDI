#pragma once

#include <memory>

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/IServiceFactory.hpp"
#include "SevenBit/IServiceInstance.hpp"
#include "SevenBit/TypeId.hpp"
#include "SevenBit/_Internal/ExternalService.hpp"

namespace sb::internal
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

} // namespace sb::internal