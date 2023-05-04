#pragma once

#include <concepts>
#include <functional>
#include <memory>
#include <type_traits>

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/IServiceFactory.hpp"
#include "SevenBit/IServiceInstance.hpp"
#include "SevenBit/IServiceProvider.hpp"
#include "SevenBit/TypeId.hpp"
#include "SevenBit/_Internal/ExternalService.hpp"
#include "SevenBit/_Internal/ServiceOwner.hpp"
#include "SevenBit/_Internal/Utils.hpp"

namespace sb::internal
{

    template <class FactoryFcn> class ServiceFcnFactory final : public IServiceFactory
    {
      private:
        FactoryFcn _factoryFunction;

      public:
        using ReturnType = std::invoke_result_t<FactoryFcn, IServiceProvider &>;
        using IsReturnTypeUniquePtr = utils::IsUniquePtr<ReturnType>;
        using ServiceType = typename IsReturnTypeUniquePtr::Type;

        ServiceFcnFactory(FactoryFcn factoryFunction) : _factoryFunction{std::move(factoryFunction)} {}

        TypeId getServiceTypeId() const { return typeid(ServiceType); }

        IServiceInstance::Ptr createInstance(IServiceProvider &serviceProvider) const
        {
            return std::make_unique<ServiceOwner<ServiceType>>(_factoryFunction(serviceProvider));
        }

        IServiceFactory::Ptr clone() { return std::make_unique<ServiceFcnFactory<FactoryFcn>>(*this); }
    };
} // namespace sb::internal