#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceCtorParamConverter.hpp"

namespace sb::di::details::utils
{
    template <class T, size_t... Ns>
    constexpr auto fieldsNumberCtor(size_t) -> decltype(T{helpers::ServiceCtorParamConverter<T, Ns>{}...}, 0)
    {
        return sizeof...(Ns);
    }

    template <class T, size_t... Ns> constexpr size_t fieldsNumberCtor(...)
    {
        return fieldsNumberCtor<T, Ns..., sizeof...(Ns)>(0);
    }

    template <class T> constexpr size_t ctorParamsNumber() { return fieldsNumberCtor<T>(0); };

} // namespace sb::di::details::utils
