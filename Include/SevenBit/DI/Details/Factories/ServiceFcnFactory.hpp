#pragma once

#include <functional>
#include <memory>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceFactoryInvoker.hpp"
#include "SevenBit/DI/Details/Services/InPlaceService.hpp"
#include "SevenBit/DI/Details/Services/UniquePtrService.hpp"
#include "SevenBit/DI/IServiceFactory.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details::factories
{
    template <class FactoryFcn> class ServiceFcnFactory : public IServiceFactory
    {
      private:
        using ServiceFactoryInvoker = helpers::ServiceFactoryInvoker<FactoryFcn>;

        mutable FactoryFcn _factoryFunction;

      public:
        using ServiceType = typename ServiceFactoryInvoker::TService;

        explicit ServiceFcnFactory(FactoryFcn &&factoryFunction) : _factoryFunction{std::move(factoryFunction)} {}

        [[nodiscard]] TypeId getServiceTypeId() const override { return typeid(ServiceType); }

        IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider, bool inPlaceRequest) const override
        {
            ServiceFactoryInvoker invoker{_factoryFunction, serviceProvider};
            if constexpr (ServiceFactoryInvoker::IsReturnTypeUniquePtr)
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
