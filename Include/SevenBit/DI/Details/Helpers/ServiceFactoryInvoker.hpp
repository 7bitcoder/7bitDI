#pragma once

#include <functional>
#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceParamProvider.hpp"
#include "SevenBit/DI/Details/Utils/FunctionReflection.hpp"
#include "SevenBit/DI/Details/Utils/IsInPlaceObject.hpp"
#include "SevenBit/DI/Details/Utils/IsUniquePtr.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details::helpers
{
    template <class FactoryFcn> struct ServiceFactoryInvoker
    {
      private:
        using FunctorTraits = utils::FunctorTraits<FactoryFcn>;
        using IsUniquePtr = utils::IsUniquePtr<typename FunctorTraits::ReturnType>;
        template <size_t Index>
        using ParamProvider = helpers::ServiceParamProvider<typename FunctorTraits::template Arg<Index>::Type>;

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

        inline auto invoke() { return invoke(std::make_index_sequence<FunctorTraits::ArgsSize>{}); }

      private:
        template <size_t... Index> inline auto invoke(std::index_sequence<Index...>)
        {
            return _factory(ParamProvider<Index>{}.getParam(_serviceProvider)...);
        }
    };
} // namespace sb::di::details::helpers
