#pragma once

#include <memory>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/FunctorInjector.hpp"
#include "SevenBit/DI/Details/Services/InPlaceService.hpp"
#include "SevenBit/DI/Details/Services/UniquePtrService.hpp"
#include "SevenBit/DI/Details/Utils/IsInPlaceObject.hpp"
#include "SevenBit/DI/Details/Utils/IsUniquePtr.hpp"
#include "SevenBit/DI/Details/Utils/RemoveUniquePtr.hpp"
#include "SevenBit/DI/IServiceFactory.hpp"

namespace sb::di::details
{
    template <class FactoryFcn> class ServiceFcnFactory final : public IServiceFactory
    {
        using FunctorInjector = FunctorInjector<FactoryFcn>;
        using FactoryReturnType = typename FunctorInjector::ReturnType;

        mutable FactoryFcn _factoryFunction;

      public:
        using ServiceType = RemoveUniquePtrT<FactoryReturnType>;

        explicit ServiceFcnFactory(FactoryFcn &&factoryFunction) : _factoryFunction{std::move(factoryFunction)}
        {
            static_assert(IsInPlaceObjectConstructableV<ServiceType> || IsUniquePtrV<FactoryReturnType> ||
                              notSupportedType<FactoryFcn>,
                          "Service factory return type must be std::unique_ptr<TService> or copyable/movable object");
        }

        IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider, const bool inPlaceRequest) const override
        {
            FunctorInjector injector{_factoryFunction, serviceProvider};
            if constexpr (IsUniquePtrV<FactoryReturnType>)
            {
                return std::make_unique<UniquePtrService<ServiceType>>(injector.call());
            }
            else
            {
                if (inPlaceRequest)
                {
                    return std::make_unique<InPlaceService<ServiceType>>(injector.call());
                }
                auto servicePtr = std::make_unique<ServiceType>(injector.call());
                return std::make_unique<UniquePtrService<ServiceType>>(std::move(servicePtr));
            }
        }
    };
} // namespace sb::di::details
