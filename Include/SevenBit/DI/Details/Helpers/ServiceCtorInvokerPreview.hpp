#include <utility>

#include "SevenBit/DI/Details/Helpers/ServiceParamProvider.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

#define __7_BIT_DI_REQUIRES(...) typename std::enable_if<__VA_ARGS__, int>::type

#ifndef _7_BIT_DI_CTOR_LIMIT_SIZE
#define _7_BIT_DI_CTOR_LIMIT_SIZE 10
#endif

namespace sb::di::details::helpers
{

    namespace internals
    {

        template <class... Args> struct type_list
        {
            using type = type_list;
        };

        template <class, class> struct is_copy_ctor__ : std::false_type
        {
        };

        template <class T> struct is_copy_ctor__<T, T> : std::true_type
        {
        };

        template <class T> struct is_copy_ctor__<T, const T> : std::true_type
        {
        };

        template <class> struct any_type
        {
            template <class T> operator T() { return ServiceParamProvider<T>{}.getParam(injector_); }

            template <class T> operator T *() { return ServiceParamProvider<T *>{}.getParam(injector_); }

            template <class T> operator T &() { return ServiceParamProvider<T &>{}.getParam(injector_); }

            template <class T> operator T &&() { return ServiceParamProvider<T>{}.getParam(injector_); }

            ServiceProvider &injector_;
        };

        template <class> struct any_type_fwd
        {
            template <class T> operator T();

          private:
            template <class T> operator const T &() const;
        };
        template <class> struct any_type_ref_fwd
        {
            template <class T> operator T();
            template <class T> operator T &() const;
            template <class T> operator T &&() const;
            template <class T> operator const T &() const;
        };
        template <class TParent> struct any_type_1st_fwd
        {
            template <class T, class = __7_BIT_DI_REQUIRES(!is_copy_ctor__<TParent, T>::value)> operator T();

          private:
            template <class T, class = __7_BIT_DI_REQUIRES(!is_copy_ctor__<TParent, T>::value)>
            operator const T &() const;
        };
        template <class TParent> struct any_type_1st_ref_fwd
        {
            template <class T, class = __7_BIT_DI_REQUIRES(!is_copy_ctor__<TParent, T>::value)> operator T();
            template <class T, class = __7_BIT_DI_REQUIRES(!is_copy_ctor__<TParent, T>::value)> operator T &() const;
            template <class T, class = __7_BIT_DI_REQUIRES(!is_copy_ctor__<TParent, T>::value)> operator T &&() const;
            template <class T, class = __7_BIT_DI_REQUIRES(!is_copy_ctor__<TParent, T>::value)>
            operator const T &() const;
        };

        template <class T, int> using get = T;

        // template <template <class...> class, class, class, class = int> struct ctor_impl;
        //
        // template <template <class...> class TIsConstructible, class T>
        // struct ctor_impl<TIsConstructible, T, std::index_sequence<>> : type_list<>
        // {
        // };
        //
        // template <template <class...> class TIsConstructible, class T>
        // struct ctor_impl<TIsConstructible, T, std::index_sequence<0>,
        //                  __7_BIT_DI_REQUIRES(TIsConstructible<T, any_type<T>>::value)> : type_list<any_type<T>>
        // {
        // };
        //
        // template <template <class...> class TIsConstructible, class T, int... Ns>
        // struct ctor_impl<TIsConstructible, T, std::index_sequence<Ns...>,
        //                  __7_BIT_DI_REQUIRES((sizeof...(Ns) > 1) &&
        //                                      TIsConstructible<T, get<any_type<T>, Ns>...>::value)>
        //     : type_list<get<any_type<T>, Ns>...>
        // {
        // };
        //
        // template <template <class...> class TIsConstructible, class T, int... Ns>
        // struct ctor_impl<TIsConstructible, T, std::index_sequence<Ns...>,
        //                  __7_BIT_DI_REQUIRES((sizeof...(Ns) > 1) &&
        //                                      !TIsConstructible<T, get<any_type<T>, Ns>...>::value)>
        //     : std::conditional<
        //           TIsConstructible<T, get<any_type<T>, Ns>...>::value, type_list<get<any_type<T>, Ns>...>,
        //           typename ctor_impl<TIsConstructible, T, std::make_index_sequence<sizeof...(Ns) - 1>>::type>
        // {
        // };

        template <template <class...> class, class, class, class = int> struct ctor_impl;

        template <template <class...> class TIsConstructible, class T>
        struct ctor_impl<TIsConstructible, T, std::index_sequence<>> : type_list<>
        {
        };

        template <template <class...> class TIsConstructible, class T>
        struct ctor_impl<TIsConstructible, T, std::index_sequence<0>,
                         __7_BIT_DI_REQUIRES(TIsConstructible<T, any_type_1st_fwd<T>>::value)> : type_list<any_type<T>>
        {
        };

        template <template <class...> class TIsConstructible, class T>
        struct ctor_impl<TIsConstructible, T, std::index_sequence<0>,
                         __7_BIT_DI_REQUIRES(!TIsConstructible<T, any_type_1st_fwd<T>>::value)>
            : std::conditional_t<TIsConstructible<T, any_type_1st_ref_fwd<T>>::value, type_list<any_type<T>>,
                                 type_list<>>
        {
        };

        template <template <class...> class TIsConstructible, class T, int... Ns>
        struct ctor_impl<TIsConstructible, T, std::index_sequence<Ns...>,
                         __7_BIT_DI_REQUIRES((sizeof...(Ns) > 1) &&
                                             TIsConstructible<T, get<any_type_fwd<T>, Ns>...>::value)>
            : type_list<get<any_type<T>, Ns>...>
        {
        };

        template <template <class...> class TIsConstructible, class T, int... Ns>
        struct ctor_impl<TIsConstructible, T, std::index_sequence<Ns...>,
                         __7_BIT_DI_REQUIRES((sizeof...(Ns) > 1) &&
                                             !TIsConstructible<T, get<any_type_fwd<T>, Ns>...>::value)>
            : std::conditional<
                  TIsConstructible<T, get<any_type_ref_fwd<T>, Ns>...>::value, type_list<get<any_type<T>, Ns>...>,
                  typename ctor_impl<TIsConstructible, T, std::make_index_sequence<sizeof...(Ns) - 1>>::type>
        {
        };

        template <template <class...> class TIsConstructible, class T>
        using ctor_impl_t =
            typename ctor_impl<TIsConstructible, T, std::make_index_sequence<_7_BIT_DI_CTOR_LIMIT_SIZE>>::type;

        template <class...> struct ctor;

        template <class T, class... TArgs> struct ctor<T, type_list<TArgs...>> : type_list<TArgs...>
        {
        };

        template <class, class = int> struct ctor_traits;

        template <class T, class> struct ctor_traits : ctor<T, ctor_impl_t<std::is_constructible, T>>
        {
        };

        template <class T, class TInit, class... Ts> auto make_impl(std::pair<TInit, type_list<Ts...>>)
        {
            return new T{typename any<Ts, injector>::type{*this}...};
        }

        template <class T, class TBinding>
        auto make(const TBinding &) -> decltype(make_impl<T>(typename ctor_traits<T>::type{}))
        {
            return make_impl<T>(typename ctor_traits<T>::type{});
        }

        struct CtorInvoker
        {
            template <class T, class... TArgs> static auto invoke(TArgs &&...args)
            {
                return T(static_cast<TArgs &&>(args)...);
            }

            template <class T> static auto invoke() { return T(); }
        };

        template <class, class> struct Resolver;

        template <class T, template <class...> class TList, class... TCtor> struct Resolver<T, TList<TCtor...>>
        {
            auto reolve() { return CtorInvoker::invoke<T>(TCtor{injector_}...); }

            ServiceProvider &injector_;
        };

        template <class T, template <class...> class TList> struct Resolver<T, TList<void>>
        {
            auto reolve() { return CtorInvoker::invoke<T>(); }

            ServiceProvider &injector_;
        };
    } // namespace internals

    template <class T> struct ServiceCtorInvoker
    {
        auto invoke() { return internals::Resolver<T, internals::ctor_traits<T>::type>{injector_}.reolve(); }

        template <class TFunc> auto invokeWithCtorParams(TFunc &&func)
        {
            // return func(ParamProvider<Index>{}.getParam(_serviceProvider)...);
        }

        ServiceProvider &injector_;
    };
} // namespace sb::di::details::helpers
