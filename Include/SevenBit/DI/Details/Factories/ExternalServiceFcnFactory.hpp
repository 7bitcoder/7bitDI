#pragma once

#include <memory>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceFactoryInvoker.hpp"
#include "SevenBit/DI/Details/Services/ExternalService.hpp"
#include "SevenBit/DI/IServiceFactory.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details::factories
{

    template <class FactoryFcn> class ExternalServiceFcnFactory : public IServiceFactory
    {
        using ServiceFactoryInvoker = helpers::ServiceFactoryInvoker<FactoryFcn>;
        using FactoryReturnType = typename ServiceFactoryInvoker::ReturnType;
        using IsPointer = std::is_pointer<FactoryReturnType>;
        using TService = std::remove_pointer_t<FactoryReturnType>;

        mutable FactoryFcn _factoryFunction;

      public:
        using ServiceType = TService;

        explicit ExternalServiceFcnFactory(FactoryFcn factoryFunction) : _factoryFunction{std::move(factoryFunction)}
        {
            static_assert(IsPointer::value || utils::notSupportedType<FactoryFcn>,
                          "External service factory return type must be pointner to object");
        }

        [[nodiscard]] TypeId getServiceTypeId() const override { return typeid(ServiceType); }

        IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider, const bool inPlaceRequest) const override
        {
            ServiceFactoryInvoker invoker{_factoryFunction, serviceProvider};
            return std::make_unique<services::ExternalService<ServiceType>>(invoker.invoke());
        }
    };
} // namespace sb::di::details::factories
