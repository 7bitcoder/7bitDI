#pragma once

#include <functional>
#include <memory>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceFactoryWrapper.hpp"
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
        using FactoryWrapper = helpers::ServiceFactoryWrapper<FactoryFcn>;
        FactoryWrapper _wrapper;

      public:
        using ServiceType = typename FactoryWrapper::TService;

        explicit ServiceFcnFactory(FactoryFcn &&factoryFunction) : _wrapper{std::move(factoryFunction)} {}

        [[nodiscard]] TypeId getServiceTypeId() const override { return typeid(ServiceType); }

        IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider, bool inPlaceRequest) const override
        {
            if constexpr (FactoryWrapper::IsUniquePtr::value)
            {
                return std::make_unique<services::UniquePtrService<ServiceType>>(_wrapper.invoke(serviceProvider));
            }
            else
            {
                if (inPlaceRequest)
                {
                    return std::make_unique<services::InPlaceService<ServiceType>>(_wrapper.invoke(serviceProvider));
                }
                auto service = std::make_unique<ServiceType>(_wrapper.invoke(serviceProvider));
                return std::make_unique<services::UniquePtrService<ServiceType>>(std::move(service));
            }
        }
    };
} // namespace sb::di::details::factories
