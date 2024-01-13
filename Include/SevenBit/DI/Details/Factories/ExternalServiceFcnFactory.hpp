#pragma once

#include <memory>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceFactoryInvoker.hpp"
#include "SevenBit/DI/Details/Services/InPlaceService.hpp"
#include "SevenBit/DI/Details/Services/UniquePtrService.hpp"
#include "SevenBit/DI/Details/Utils/IsUniquePtr.hpp"
#include "SevenBit/DI/IServiceFactory.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details::factories
{
    template <class FactoryFcn> class ServiceFcnFactory final : public IServiceFactory
    {
        using ServiceFactoryInvoker = helpers::ServiceFactoryInvoker<FactoryFcn>;
        using FactoryReturnType = typename ServiceFactoryInvoker::ReturnType;

        mutable FactoryFcn _factoryFunction;

      public:
        explicit ServiceFcnFactory(FactoryFcn &&factoryFunction) : _factoryFunction{std::move(factoryFunction)} {}

        [[nodiscard]] TypeId getServiceTypeId() const override { return typeid(ServiceType); }

        IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider, const bool inPlaceRequest) const override
        {
            ServiceFactoryInvoker invoker{_factoryFunction, serviceProvider};
            if constexpr (IsUniquePtr::value)
            {
                return std::make_unique<services::UniquePtrService<ServiceType>>(invoker.invoke());
            }
            else if (IsPtr::value)
            {
                return std::make_unique<services::ExternalService<ServiceType>>(invoker.invoke());
            }
            else if (utils::IsInPlaceObjectConstructableV<ServiceType>)
            {
                if (inPlaceRequest)
                {
                    return std::make_unique<services::InPlaceService<ServiceType>>(invoker.invoke());
                }
                auto servicePtr = std::make_unique<ServiceType>(invoker.invoke());
                return std::make_unique<services::UniquePtrService<ServiceType>>(std::move(servicePtr));
            }
            else
            {
                static_assert(utils::notSupportedType<FactoryFcn>,
                              "Service factory return type must be pointer or std::unique_ptr<TService> or "
                              "movable/copyable object");
            }
        }
    };
} // namespace sb::di::details::factories
