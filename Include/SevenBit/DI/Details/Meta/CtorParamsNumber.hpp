#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Meta/Meta.hpp"

namespace sb::di::details
{
    class CtorParamsNumber
    {
        template <class S>
        struct Conv
        {
            explicit Conv(std::size_t paramNumber) {}

            template <class U, class = std::enable_if_t<!IsCopyCtorV<S, U>>> operator U();
            template <class U, class = std::enable_if_t<!IsCopyCtorV<S, U>>> operator U &() const;
        };

        template <class S, std::size_t... Ns> static  constexpr auto paramsNumber(std::size_t) -> decltype(S{Conv<S>{Ns}...}, 0)
        {
            return sizeof...(Ns);
        }

        template <class S, std::size_t... Ns> static constexpr std::size_t paramsNumber(...)
        {

            if constexpr (sizeof...(Ns) > _7BIT_DI_CTOR_PARAMS_LIMIT)
            {
                static_assert(
                    details::notSupportedType<S>,
                    "Proper constructor for specified type was not found, reached maximum constructor params number "
                    "limit, to bump limit define macro _7BIT_DI_CTOR_PARAMS_LIMIT with new value befor including lib");
                return 0;
            }
            else
            {
                return paramsNumber<S, Ns..., sizeof...(Ns)>(0);
            }
        }
	public:
      	template<class F> static constexpr std::size_t value = paramsNumber<F>(0);
	};
} // namespace sb::di::details
