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
        using FunctorReturnType = decltype(Injector{nullptr, nullptr}());
        static constexpr bool isReturnTypeOk = IsUniquePtrV<FunctorReturnType> || IsInPlaceServiceV<FunctorReturnType>;
        static_assert(isReturnTypeOk || notSupportedType<FactoryFcn>,
                      "Service factory return type must be std::unique_ptr<TService> or copyable/movable object");

        mutable FactoryFcn _factoryFunction;

      public:
        using ServiceType = RemoveUniquePtrT<FunctorReturnType>;

        explicit ServiceFcnFactory(FactoryFcn &&factoryFunction) : _factoryFunction{std::move(factoryFunction)} {}

        IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider, const bool inPlaceRequest) const override
        {
            Injector injector{_factoryFunction, serviceProvider};
            if constexpr (!IsUniquePtrV<FunctorReturnType>)
            {
                if (inPlaceRequest)
                {
                    return injector.template makeUnique<InPlaceService<ServiceType>>();
                }
            }
            return injector.template makeUnique<UniquePtrService<ServiceType>>();
        }
    };
} // namespace sb::di::details
