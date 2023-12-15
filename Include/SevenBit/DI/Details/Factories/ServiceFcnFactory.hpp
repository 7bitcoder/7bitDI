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
        using IsUniquePtr = utils::IsUniquePtr<FactoryReturnType>;
        using TService = typename IsUniquePtr::Type;

        mutable FactoryFcn _factoryFunction;

      public:
        using ServiceType = TService;

        explicit ServiceFcnFactory(FactoryFcn &&factoryFunction) : _factoryFunction{std::move(factoryFunction)}
        {
            static_assert(IsUniquePtr::value || utils::IsInPlaceObjectConstructableV<TService> ||
                              utils::notSupportedType<FactoryFcn>,
                          "Service factory return type must be std::unique_ptr<TService> or movable/copyable object");
        }

        [[nodiscard]] TypeId getServiceTypeId() const override { return typeid(ServiceType); }

        IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider, const bool inPlaceRequest) const override
        {
            ServiceFactoryInvoker invoker{_factoryFunction, serviceProvider};
            if constexpr (IsUniquePtr::value)
            {
                return std::make_unique<services::UniquePtrService<ServiceType>>(invoker.invoke());
            }
            else
            {
                if (inPlaceRequest)
                {
                    return std::make_unique<services::InPlaceService<ServiceType>>(invoker.invoke());
                }
                auto servicePtr = std::make_unique<ServiceType>(invoker.invoke());
                return std::make_unique<services::UniquePtrService<ServiceType>>(std::move(servicePtr));
            }
        }
    };
} // namespace sb::di::details::factories
