#pragma once

#include <functional>
#include <memory>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/ExternalService.hpp"
#include "SevenBit/DI/Details/ServiceFactoryWrapper.hpp"
#include "SevenBit/DI/Details/ServiceOwner.hpp"
#include "SevenBit/DI/Details/ServiceUniquePtrOwner.hpp"
#include "SevenBit/DI/Details/Utils.hpp"
#include "SevenBit/DI/IServiceFactory.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details
{
    template <class FactoryFcn> class ServiceFcnFactory final : public IServiceFactory
    {
      private:
        using FactoryWrapper = ServiceFactoryWrapper<FactoryFcn>;
        FactoryWrapper _wrapper;

      public:
        using ServiceType = typename FactoryWrapper::TService;

        ServiceFcnFactory(FactoryFcn &&factoryFunction) : _wrapper{std::move(factoryFunction)} {}

        TypeId getServiceTypeId() const override { return typeid(ServiceType); }

        IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider, bool inPlace) const override
        {
            if constexpr (FactoryWrapper::IsInPlaceObject::value && !FactoryWrapper::IsUniquePtr::value)
            {
                if (inPlace)
                {
                    return std::make_unique<ServiceOwner<ServiceType>>(_wrapper.invoke(serviceProvider));
                }
                auto service = std::make_unique<ServiceType>(_wrapper.invoke(serviceProvider));
                return std::make_unique<ServiceUniquePtrOwner<ServiceType>>(std::move(service));
            }
            return std::make_unique<ServiceUniquePtrOwner<ServiceType>>(_wrapper.invoke(serviceProvider));
        }
    };
} // namespace sb::di::details
