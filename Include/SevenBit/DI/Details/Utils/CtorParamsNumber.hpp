#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/IsCopyCtor.hpp"
#include "SevenBit/DI/Details/Utils/NotSupportedType.hpp"

namespace sb::di::details
{
    namespace ctorParamsNumberInternals
    {
        template <class T> struct Conv
        {
            explicit Conv(std::size_t paramNumber) {}

            template <class U, class = std::enable_if_t<!IsCopyCtorV<T, U>>> operator U();
            template <class U, class = std::enable_if_t<!IsCopyCtorV<T, U>>> operator U &() const;
        };

        template <class T, std::size_t... Ns> constexpr auto paramsNumber(std::size_t) -> decltype(T{Conv<T>{Ns}...}, 0)
        {
            return sizeof...(Ns);
        }

        template <class T, std::size_t... Ns> constexpr std::size_t paramsNumber(...)
        {

            if constexpr (sizeof...(Ns) > _7BIT_DI_CTOR_PARAMS_LIMIT)
            {
                static_assert(
                    details::notSupportedType<T>,
                    "Proper constructor for specified type was not found, reached maximum constructor params number "
                    "limit, to bump limit define macro _7BIT_DI_CTOR_PARAMS_LIMIT with new value befor including lib");
                return 0;
            }
            else
            {
                return paramsNumber<T, Ns..., sizeof...(Ns)>(0);
            }
        }
    } // namespace ctorParamsNumberInternals

    template <class T> constexpr std::size_t ctorParamsNumber()
    {
        return ctorParamsNumberInternals::paramsNumber<T>(0);
    };

} // namespace sb::di::details
