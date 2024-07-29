#pragma once

#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceExtractor.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

namespace sb::di::details
{
    template <class T> class CtorInjector
    {
        template <std::size_t... Ns>
        static constexpr auto parametersNumber(std::size_t) -> decltype(T{ServiceExtractor<T>{nullptr, Ns}...}, 0)
        {
            return sizeof...(Ns);
        }

        template <std::size_t... Ns> static constexpr std::size_t parametersNumber(...)
        {
            if constexpr (sizeof...(Ns) > _7BIT_DI_CTOR_PARAMS_LIMIT)
            {
                static_assert(
                    details::notSupportedType<T>,
                    "Proper constructor for specified type was not found, reached maximum constructor params number "
                    "limit, to bump limit define macro _7BIT_DI_CTOR_PARAMS_LIMIT with new value before including lib");
                return 0;
            }
            else
            {
                return parametersNumber<Ns..., sizeof...(Ns)>(0);
            }
        }

        ServiceProvider &_serviceProvider;

      public:
        static constexpr std::size_t paramsNumber = parametersNumber(0);

        explicit CtorInjector(ServiceProvider &serviceProvider) : _serviceProvider(serviceProvider) {}

        template <class F> auto operator()(F &&f) { return injectInto(f, std::make_index_sequence<paramsNumber>{}); };

      private:
        template <class F, std::size_t... ParamNumber> auto injectInto(F &&f, std::index_sequence<ParamNumber...>)
        {
            return f(ServiceExtractor<T>(&_serviceProvider, ParamNumber)...);
        }
    };

} // namespace sb::di::details
