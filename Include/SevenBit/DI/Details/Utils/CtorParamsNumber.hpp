#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceCtorParamConverter.hpp"
#include "SevenBit/DI/Details/Utils/NotSupportedType.hpp"

namespace sb::di::details::utils
{
    namespace ctorParamsNumberInternals
    {
        template <class T> struct Conv
        {
            explicit Conv(size_t paramNumber) {}

            template <class U, class = typename std::enable_if_t<!utils::IsCopyCtorV<T, U>>> operator U();
            template <class U, class = typename std::enable_if_t<!utils::IsCopyCtorV<T, U>>> operator U &() const;
        };

        template <class T, size_t... Ns> constexpr auto paramsNumber(size_t) -> decltype(T{Conv<T>{Ns}...}, 0)
        {
            return sizeof...(Ns);
        }

        template <class T, size_t... Ns> constexpr size_t paramsNumber(...)
        {

            if constexpr (sizeof...(Ns) > _7BIT_DI_CTOR_PARAMS_LIMIT)
            {
                static_assert(
                    details::utils::notSupportedType<T>,
                    "Proper constructor for cpecified type was not found, reached maximum constructor params number "
                    "limit, to bump limit define macro _7BIT_DI_CTOR_PARAMS_LIMIT with new value befor including lib");
                return 0;
            }
            else
            {
                return paramsNumber<T, Ns..., sizeof...(Ns)>(0);
            }
        }
    } // namespace ctorParamsNumberInternals

    template <class T> constexpr size_t ctorParamsNumber() { return ctorParamsNumberInternals::paramsNumber<T>(0); };

} // namespace sb::di::details::utils
