#pragma once

#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/CtorParamsNumber.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

namespace sb::di::details
{
    template <class T> class ServiceCtorInvoker
    {
        ServiceProvider &_serviceProvider;

      public:
        explicit ServiceCtorInvoker(ServiceProvider &serviceProvider) : _serviceProvider(serviceProvider) {}

        template <class TFunc> auto invokeWithCtorParams(TFunc &&func)
        {
            return invokeWithCtorParams<TFunc>(std::forward<TFunc>(func),
                                               std::make_index_sequence<ctorParamsNumber<T>()>{});
        };

      private:
        template <class TFunc, std::size_t... Index> auto invokeWithCtorParams(TFunc &&func, std::index_sequence<Index...>)
        {
            return func(ServiceCtorParamConverter<T>{_serviceProvider, Index}...);
        }
    };

} // namespace sb::di::details
