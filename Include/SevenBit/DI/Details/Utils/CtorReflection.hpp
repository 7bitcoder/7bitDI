#pragma once

#include <utility>
#include <vector>

#include "SevenBit/DI/Details/Utils/IsCopyCtor.hpp"
#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details::utils
{
#if defined(__GNUC__) && (__GNUC__ >= 7)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-template-friend"
#endif

    template <class T, int N> struct Conv
    {
        template <typename U, typename = typename std::enable_if_t<!IsCopyCtorV<T, U>>> operator U()
        {
            return helpers::ServiceParamProvider<U>{}.getParam(*provider);
        }
        template <typename U, typename = typename std::enable_if_t<!IsCopyCtorV<T, U>>> operator U &() const
        {
            return helpers::ServiceParamProvider<U &>{}.getParam(*provider);
        }

        ServiceProvider *provider = nullptr;
    };

    // template <typename T, int N> struct Tag
    // {
    //
    //     friend auto loophole(Tag<T, N>);
    //     constexpr friend int cloophole(Tag<T, N>);
    // };
    //
    // template <typename T, int N> struct RefTag
    // {
    //     friend auto &loopholeRef(RefTag<T, N>);
    //     constexpr friend int cloopholeRef(RefTag<T, N>);
    // };
    //
    // template <typename T, typename U, int N, bool B, typename = typename std::enable_if_t<!IsCopyCtorV<T, U>>>
    // struct FnDef
    // {
    //     friend auto loophole(Tag<T, N>) { return Conv<T, N>{}; }
    //
    //     constexpr friend int cloophole(Tag<T, N>) { return 0; }
    //
    //     friend auto &loopholeRef(RefTag<T, N>) { return Conv<T, N>{}; }
    //
    //     constexpr friend int cloopholeRef(RefTag<T, N>) { return 0; }
    // };
    //
    // template <typename T, typename U, int N> struct FnDef<T, U, N, true>
    // {
    // };
    //
    // template <typename T, int N> struct Cop
    // {
    //     template <typename U, int M> static auto ins(...) -> int;
    //     template <typename U, int M, int = cloophole(Tag<T, M>{})> static auto ins(int) -> char;
    //
    //     template <typename U, int M> static auto insRef(...) -> int;
    //     template <typename U, int M, int = cloopholeRef(RefTag<T, M>{})> static auto insRef(int) -> char;
    //
    //     template <typename U, int = sizeof(FnDef<T, U, N, sizeof(ins<U, N>(0)) == sizeof(char)>)> operator U();
    //     template <typename U, int = sizeof(FnDef<T, U, N, sizeof(insRef<U, N>(0)) == sizeof(char)>)>
    //     operator U &() const;
    // };

    template <typename T, int... Ns> constexpr auto fieldsNumberCtor(int) -> decltype(T{Conv<T, Ns>{}...}, 0)
    {
        return sizeof...(Ns);
    }

    template <typename T, int... Ns> constexpr int fieldsNumberCtor(...)
    {
        return fieldsNumberCtor<T, Ns..., sizeof...(Ns)>(0);
    }

#if defined(__GNUC__) && (__GNUC__ >= 7)
#pragma GCC diagnostic pop
#endif
} // namespace sb::di::details::utils
