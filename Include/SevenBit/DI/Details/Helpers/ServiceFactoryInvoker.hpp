#pragma once

#include <functional>
#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceFactoryParamResolver.hpp"
#include "SevenBit/DI/Details/Helpers/ServiceParamProvider.hpp"
#include "SevenBit/DI/Details/Utils/IsInPlaceObject.hpp"
#include "SevenBit/DI/Details/Utils/IsUniquePtr.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details::helpers
{
    template <class FactoryFcn> class ServiceFactoryInvoker
    {
      private:
        using ServiceFactoryParamResolver = ServiceFactoryParamResolver<FactoryFcn>;
        using IsUniquePtr = utils::IsUniquePtr<typename ServiceFactoryParamResolver::ReturnType>;

        FactoryFcn &_factory;
        ServiceProvider &_serviceProvider;

      public:
        static constexpr bool IsReturnTypeUniquePtr = IsUniquePtr::value;
        using TService = typename IsUniquePtr::Type;

        ServiceFactoryInvoker(FactoryFcn &factory, ServiceProvider &serviceProvider)
            : _factory(factory), _serviceProvider(serviceProvider)
        {
            static_assert(IsReturnTypeUniquePtr || utils::IsInPlaceObjectConstructableV<TService> ||
                              utils::notSupportedType<FactoryFcn>,
                          "Factory return type must be std::unique_ptr<TService> or movable/copyable object");
        }

        inline auto invoke() { return ServiceFactoryParamResolver::invoke(_factory, _serviceProvider); }
    };
} // namespace sb::di::details::helpers
