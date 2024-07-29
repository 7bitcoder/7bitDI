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
        using ReturnType = decltype(Injector{nullptr, nullptr}());

        static_assert(IsUniquePtrV<ReturnType> || IsInPlaceServiceV<ReturnType> || notSupportedType<FactoryFcn>,
                      "Service factory return type must be std::unique_ptr<TService> or copyable/movable object");

        mutable FactoryFcn _factoryFunction;

      public:
        using ServiceType = RemoveUniquePtrT<ReturnType>;

        explicit ServiceFcnFactory(FactoryFcn &&factoryFunction) : _factoryFunction{std::move(factoryFunction)} {}

        IServiceInstance::Ptr createInstance(ServiceProvider &serviceProvider, const bool inPlaceRequest) const override
        {
            Injector injector{_factoryFunction, serviceProvider};
            if constexpr (!IsUniquePtrV<ReturnType>)
            {
                if (inPlaceRequest)
                {
                    return std::make_unique<InPlaceService<ServiceType>>(injector());
                }
            }
            return std::make_unique<UniquePtrService<ServiceType>>(injector());
        }
    };
} // namespace sb::di::details
