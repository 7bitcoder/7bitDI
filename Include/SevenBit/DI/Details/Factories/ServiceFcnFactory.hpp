#pragma once

#include <memory>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/FunctorInjector.hpp"
#include "SevenBit/DI/Details/Meta/Meta.hpp"
#include "SevenBit/DI/Details/Services/InPlaceService.hpp"
#include "SevenBit/DI/Details/Services/UniquePtrService.hpp"
#include "SevenBit/DI/IServiceFactory.hpp"

namespace sb::di::details
{
    template <class FactoryFcn> class ServiceFcnFactory final : public IServiceFactory
    {
        mutable FactoryFcn _factoryFunction;

      public:
        using FunctorReturnType = typename FunctorInjector<FactoryFcn>::ReturnType;
        using ServiceType = RemoveUniquePtrT<FunctorReturnType>;

        explicit ServiceFcnFactory(FactoryFcn &&factoryFunction) : _factoryFunction{std::move(factoryFunction)} {}

        IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider, const bool inPlaceRequest) const override
        {
            FunctorInjector<FactoryFcn> injector{_factoryFunction, serviceProvider};
            if constexpr (IsUniquePtrV<FunctorReturnType>)
            {
                return std::make_unique<UniquePtrService<ServiceType>>(injector.call());
            }
            else if constexpr (IsInPlaceServiceV<FunctorReturnType>)
            {
                if (inPlaceRequest)
                {
                    return std::make_unique<InPlaceService<ServiceType>>(injector.call());
                }
                return std::make_unique<UniquePtrService<ServiceType>>(injector.call());
            }
            else
            {
                badFunctor();
                return nullptr;
            }
        }

        static void badFunctor()
        {
            static_assert(notSupportedType<FactoryFcn>,
                          "Service factory return type must be std::unique_ptr<TService> or copyable/movable object");
        }
    };
} // namespace sb::di::details
