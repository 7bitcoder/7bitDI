#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceCtorParamConverter.hpp"

namespace sb::di::details::utils
{
    template <typename T, int N> struct Tag
    {
        friend auto loophole(Tag<T, N>);
        constexpr friend int cloophole(Tag<T, N>);
    };

    template <typename T, typename U, int N, bool B,
              typename = typename std::enable_if_t<!std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>,
                                                                   std::remove_cv_t<std::remove_reference_t<U>>>>>
    struct FnDef
    {
        friend auto loophole(Tag<T, N>) { return U{}; }
        constexpr friend int cloophole(Tag<T, N>) { return 0; }
    };

    template <typename T, typename U, int N> struct FnDef<T, U, N, true>
    {
    };

    template <typename T, int N> struct Cop
    {
        template <typename U, int M> static auto ins(...) -> int;
        template <typename U, int M, int = cloophole(Tag<T, M>{})> static auto ins(int) -> char;

        template <typename U, int = sizeof(FnDef<T, U, N, sizeof(ins<U, N>(0)) == sizeof(char)>)> operator U();
    };

    template <typename T, int... Ns> constexpr int fieldsNumber(...) { return sizeof...(Ns) - 1; }

    template <typename T, int... Ns>
    constexpr auto fieldsNumber(int) -> decltype(T{helpers::ServiceCtorParamConverter<T, Ns>{}...}, 0)
    {
        return fieldsNumber<T, Ns..., sizeof...(Ns)>(0);
    }

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
    template <class T> constexpr size_t ctorParamsNumber2() { return fieldsNumber<T>(0); };

} // namespace sb::di::details::utils
