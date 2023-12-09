#pragma once

#include <memory>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceParamProvider.hpp"
#include "SevenBit/DI/Details/Services/InPlaceService.hpp"
#include "SevenBit/DI/Details/Utils/CtorParamsNumber.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

namespace sb::di::details::helpers
{
    template <class T> class ServiceCtorInvoker
    {
      private:
        ServiceProvider &_serviceProvider;

      public:
        explicit ServiceCtorInvoker(ServiceProvider &serviceProvider) : _serviceProvider(serviceProvider) {}

        template <class TFunc> inline auto invokeWithCtorParams(TFunc &&func)
        {
            return invokeWithCtorParams<TFunc>(std::forward<TFunc>(func),
                                               std::make_index_sequence<utils::ctorParamsNumber<T>()>{});
        };

      private:
        template <class TFunc, size_t... Index>
        inline auto invokeWithCtorParams(TFunc &&func, std::index_sequence<Index...>)
        {
            return func(ServiceCtorParamConverter<T, Index>{_serviceProvider}...);
        }
    };

} // namespace sb::di::details::helpers
