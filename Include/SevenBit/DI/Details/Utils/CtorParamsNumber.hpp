#pragma once

#include <utility>
#include <vector>

#include "SevenBit/DI/Details/Helpers/ServiceCtorParamConverter.hpp"
#include "SevenBit/DI/Details/Utils/IsCopyCtor.hpp"
#include "SevenBit/DI/LibraryConfig.hpp"

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
