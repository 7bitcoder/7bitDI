#pragma once

#include <memory>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceParamProvider.hpp"
#include "SevenBit/DI/Details/Services/InPlaceService.hpp"
#include "SevenBit/DI/Details/Utils/CtorReflection.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

namespace sb::di::details::helpers
{
    template <class T> class ServiceCtorInvoker
    {
      private:
        // using ConstructorTraits = utils::ConstructorTraits<T>;
        // template <size_t Index>
        // using ParamProvider = helpers::ServiceParamProvider<typename ConstructorTraits::template Arg<Index>::Type>;

        ServiceProvider &_serviceProvider;

      public:
        explicit ServiceCtorInvoker(ServiceProvider &serviceProvider) : _serviceProvider(serviceProvider) {}

        template <class TFunc> inline auto invokeWithCtorParams(TFunc &&func)
        {
            return invokeWithCtorParams<TFunc>(std::forward<TFunc>(func),
                                               std::make_integer_sequence<int, utils::fieldsNumberCtor<T>(0)>{});
        };

      private:
        // template <typename T, int... Ns> auto make(TFunc &&func, std::integer_sequence<int, Ns...>)
        // {
        //     using Type = std::tuple<decltype(loophole(Tag<T, Ns>{}))...>;
        // };
        //
        // template <typename T>
        // using AsTuple = typename loopholeTuple<T, std::make_integer_sequence<int, fieldsNumberCtor<T>(0)>>::Type;

        template <class TFunc, int... Ns>
        inline auto invokeWithCtorParams(TFunc &&func, std::integer_sequence<int, Ns...>)
        {
            return func(utils::Conv<T, Ns>{&_serviceProvider}...);
        }
    };

} // namespace sb::di::details::helpers
