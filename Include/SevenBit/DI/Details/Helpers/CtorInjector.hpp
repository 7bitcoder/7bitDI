#pragma once

#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/CtorParamsNumber.hpp"
#include "SevenBit/DI/Details/Helpers/CtorArgServiceInjector.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

namespace sb::di::details
{
    template <class T> class CtorInjector
    {
        ServiceProvider &_serviceProvider;

      public:
        explicit CtorInjector(ServiceProvider &serviceProvider) : _serviceProvider(serviceProvider) {}

        template <class TFunc> auto callWithCtorArgs(TFunc &&func)
        {
            return callWithCtorArgs<TFunc>(std::forward<TFunc>(func),
                                               std::make_index_sequence<ctorParamsNumber<T>()>{});
        };

      private:
        template <class TFunc, std::size_t... Index>
        auto callWithCtorArgs(TFunc &&func, std::index_sequence<Index...>)
        {
            return func(CtorArgServiceInjector<T>{_serviceProvider, Index}...);
        }
    };

} // namespace sb::di::details
