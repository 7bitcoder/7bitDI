#pragma once

#include <functional>
#include <memory>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/ExternalService.hpp"
#include "SevenBit/DI/Details/ServiceOwner.hpp"
#include "SevenBit/DI/Details/Utils.hpp"
#include "SevenBit/DI/IServiceFactory.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details
{

    template <class FactoryFcn> class ExternalServiceFcnFactory final : public IServiceFactory
    {
      private:
        FactoryFcn _factoryFunction;

      public:
        using ReturnType = std::invoke_result_t<FactoryFcn, ServiceProvider &>;
        using IsReturnTypePtr = std::is_pointer<ReturnType>;
        using ServiceType = std::remove_pointer_t<ReturnType>;

        ExternalServiceFcnFactory(FactoryFcn factoryFunction) : _factoryFunction{std::move(factoryFunction)} {}

        TypeId getServiceTypeId() const { return typeid(ServiceType); }

        IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider) const
        {
            return std::make_unique<ExternalService<ServiceType>>(_factoryFunction(serviceProvider));
        }

        IServiceFactory::Ptr clone() { return std::make_unique<ExternalServiceFcnFactory<FactoryFcn>>(*this); }
    };
} // namespace sb::di::details
