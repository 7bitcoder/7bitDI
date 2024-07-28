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
        static constexpr auto paramsNumber(std::size_t) -> decltype(T{ServiceExtractor<T>{nullptr, Ns}...}, 0)
        {
            return sizeof...(Ns);
        }

        template <std::size_t... Ns> static constexpr std::size_t paramsNumber(...)
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
                return paramsNumber<Ns..., sizeof...(Ns)>(0);
            }
        }

        ServiceProvider &_serviceProvider;

      public:
        static constexpr std::size_t parametersNumber = paramsNumber(0);

        explicit CtorInjector(ServiceProvider &serviceProvider) : _serviceProvider(serviceProvider) {}
        explicit CtorInjector(ServiceProvider *serviceProvider) : CtorInjector(*serviceProvider) {}

        template <class TWrapper> std::unique_ptr<TWrapper> makeUnique()
        {
            return makeUnique<TWrapper>(std::make_index_sequence<parametersNumber>{});
        };

      private:
        template <class TWrapper, std::size_t... ParamNumber>
        std::unique_ptr<TWrapper> makeUnique(std::index_sequence<ParamNumber...>)
        {
            return std::make_unique<TWrapper>(ServiceExtractor<T>(&_serviceProvider, ParamNumber)...);
        }
    };

} // namespace sb::di::details
