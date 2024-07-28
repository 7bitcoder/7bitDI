#pragma once

#include <memory>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/FunctorInjector.hpp"
#include "SevenBit/DI/Details/Services/InPlaceService.hpp"
#include "SevenBit/DI/Details/Services/UniquePtrService.hpp"
#include "SevenBit/DI/Details/Utils/Meta.hpp"
#include "SevenBit/DI/IServiceFactory.hpp"

namespace sb::di::details
{
    template <class FactoryFcn> class ServiceFcnFactory final : public IServiceFactory
    {
        using Injector = FunctorInjector<FactoryFcn>;

        mutable FactoryFcn _factoryFunction;

      public:
        using FunctorReturnType = decltype(Injector{nullptr, nullptr}());
        using ServiceType = RemoveUniquePtrT<FunctorReturnType>;

        explicit ServiceFcnFactory(FactoryFcn &&factoryFunction) : _factoryFunction{std::move(factoryFunction)} {}

        IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider, const bool inPlaceRequest) const override
        {
            Injector injector{_factoryFunction, serviceProvider};
            if constexpr (IsUniquePtrV<FunctorReturnType>)
            {
                return injector.template makeUnique<UniquePtrService<ServiceType>>();
            }
            else if constexpr (IsInPlaceServiceV<FunctorReturnType>)
            {
                if (inPlaceRequest)
                {
                    return injector.template makeUnique<InPlaceService<ServiceType>>();
                }
                return injector.template makeUnique<UniquePtrService<ServiceType>>();
            }
            else
            {
                badFunctor();
                return nullptr;
            }
        }

      private:
        static void badFunctor()
        {
            static_assert(notSupportedType<FactoryFcn>,
                          "Service factory return type must be std::unique_ptr<TService> or copyable/movable object");
        }
    };
} // namespace sb::di::details
