#pragma once

#include <functional>
#include <memory>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Services/ExternalService.hpp"
#include "SevenBit/DI/Details/Services/InPlaceService.hpp"
#include "SevenBit/DI/IServiceFactory.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details::factories
{

    template <class FactoryFcn> class ExternalServiceFcnFactory : public IServiceFactory
    {
      private:
        FactoryFcn _factoryFunction;

      public:
        using ReturnType = std::invoke_result_t<FactoryFcn, ServiceProvider &>;
        using IsReturnTypePtr = std::is_pointer<ReturnType>;
        using ServiceType = std::remove_pointer_t<ReturnType>;

        explicit ExternalServiceFcnFactory(FactoryFcn factoryFunction) : _factoryFunction{std::move(factoryFunction)} {}

        [[nodiscard]] TypeId getServiceTypeId() const override { return typeid(ServiceType); }

        IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider, bool inPlaceRequest) const override
        {
            return std::make_unique<services::ExternalService<ServiceType>>(_factoryFunction(serviceProvider));
        }
    };
} // namespace sb::di::details::factories
