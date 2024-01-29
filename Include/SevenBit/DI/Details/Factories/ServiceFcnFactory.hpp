#pragma once

#include <memory>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceFactoryInvoker.hpp"
#include "SevenBit/DI/Details/Services/InPlaceService.hpp"
#include "SevenBit/DI/Details/Services/UniquePtrService.hpp"
#include "SevenBit/DI/Details/Utils/IsUniquePtr.hpp"
#include "SevenBit/DI/Details/Utils/RemoveUniquePtr.hpp"
#include "SevenBit/DI/IServiceFactory.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details::factories
{
    template <class FactoryFcn> class ServiceFcnFactory final : public IServiceFactory
    {
        using ServiceFactoryInvoker = helpers::ServiceFactoryInvoker<FactoryFcn>;
        using FactoryReturnType = typename ServiceFactoryInvoker::ReturnType;

        mutable FactoryFcn _factoryFunction;

      public:
        using ServiceType = utils::RemoveUniquePtrT<FactoryReturnType>;

        explicit ServiceFcnFactory(FactoryFcn &&factoryFunction) : _factoryFunction{std::move(factoryFunction)}
        {
            static_assert(utils::IsInPlaceObjectConstructableV<ServiceType> || utils::IsUniquePtrV<FactoryReturnType> ||
                              utils::notSupportedType<FactoryFcn>,
                          "Service factory return type must be std::unique_ptr<TService> or copyable/movable object");
        }

        [[nodiscard]] TypeId getServiceTypeId() const override { return typeid(ServiceType); }

        IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider, const bool inPlaceRequest) const override
        {
            ServiceFactoryInvoker invoker{_factoryFunction, serviceProvider};
            if constexpr (utils::IsUniquePtrV<FactoryReturnType>)
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
