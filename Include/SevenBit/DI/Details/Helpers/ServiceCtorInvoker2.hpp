#include <string>
#include <tuple>
#include <utility>

#include "ParamsProvider.hpp"

#define __7_BIT_DI_REQUIRES(...) typename std::enable_if<__VA_ARGS__, int>::type    // __pph__
#define __7_BIT_DI_REQUIRES_MSG(...) typename std::concept_check<__VA_ARGS__>::type // __pph__
#if !defined(_7_BIT_DI_CFG_CTOR_LIMIT_SIZE)                                         // __pph__
#define _7_BIT_DI_CFG_CTOR_LIMIT_SIZE 10                                            // __pph__
#endif

namespace sevenBit
{

    template <class, class = int> struct ctor_traits;

    namespace type_traits
    {

        template <class... Args> struct type_list : public std::tuple<Args...>
        {
            using type = type_list;
            using tuple = std::tuple<Args...>;
        };

        template <class...> struct valid
        {
            using type = int;
        };

        template <class... Ts> using valid_t = typename valid<Ts...>::type;

        template <class, class = int> struct is_injectable : std::false_type
        {
        };

        template <class T> struct is_injectable<T, valid_t<typename T::call_name>> : std::true_type
        {
        };

        template <class TParent, class TInjector, class TError = std::false_type> struct any_type
        {
            template <class T> operator T() { return ServiceParamProvider<T>{}.getParam(&injector_); }

            const ServiceProvider &injector_;
        };

        template <class TParent, class TInjector, class TError = std::false_type, class TRefError = std::false_type>
        struct any_type_ref
        {
            template <class T> operator T()
            {
                //            return static_cast<const
                //            core::injector__<TInjector>&>(injector_).create_impl(aux::type<T>{});
            }

            template <class T> operator T &&() const
            {
                //            return static_cast<const
                //            core::injector__<TInjector>&>(injector_).create_impl(aux::type<T&&>{});
            }

            template <class T> operator T &() const
            {
                //            return static_cast<const
                //            core::injector__<TInjector>&>(injector_).create_impl(aux::type<T&>{});
            }

            template <class T> operator const T &() const
            {
                //            return static_cast<const
                //            core::injector__<TInjector>&>(injector_).create_impl(aux::type<const T&>{});
            }

            const TInjector &injector_;
        };

        template <class TParent, class TInjector, class TError = std::false_type> struct any_type_1st
        {
            template <class T> operator T()
            {
                //            return static_cast<const
                //            core::injector__<TInjector>&>(injector_).create_impl(aux::type<T>{});
            }

            const TInjector &injector_;
        };

        template <class TParent, class TInjector, class TError = std::false_type, class TRefError = std::false_type>
        struct any_type_1st_ref
        {
            template <class T> operator T()
            {
                //            return static_cast<const
                //            core::injector__<TInjector>&>(injector_).create_impl(aux::type<T>{});
            }

            template <class T> operator T &&() const
            {
                //            return static_cast<const
                //            core::injector__<TInjector>&>(injector_).create_impl(aux::type<T&&>{});
            }

            template <class T> operator T &() const
            {
                //            return static_cast<const
                //            core::injector__<TInjector>&>(injector_).create_impl(aux::type<T&>{});
            }

            template <class T> operator const T &() const
            {
                //            return static_cast<const
                //            core::injector__<TInjector>&>(injector_).create_impl(aux::type<const T&>{});
            }

            const TInjector &injector_;
        };

        template <class> struct any_type_fwd
        {
            template <class T> operator T() { return injector_.makeService<T>(); }

            ServiceProvider &injector_;

          private:
            template <class T> operator const T &() const { return injector_.getService<T>(); }
        };

        template <class> struct any_type_ref_fwd
        {
            template <class T> operator T() { return injector_.makeService<T>(); }

            template <class T> operator T &() const { return injector_.getService<T>(); }

            template <class T> operator T &&() const { return injector_.getService<T>(); }

            template <class T> operator const T &() const { return injector_.getService<T>(); }

            ServiceProvider &injector_;
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

        using swallow = int[];

        template <class = type_list<>> struct pool;

        template <class... TArgs> using pool_t = pool<type_list<TArgs...>>;

        template <class... TArgs> struct pool<type_list<TArgs...>> : TArgs...
        {
            template <class... Ts> explicit pool(Ts... args) noexcept : Ts(static_cast<Ts &&>(args))... {}

            template <class... Ts, class TPool>
            pool(const type_list<Ts...> &, TPool p) noexcept : pool(static_cast<Ts &&>(p)...)
            {
                (void)p;
            }

            template <class T> pool &operator=(T &&other) noexcept
            {
                (void)swallow{0, (static_cast<TArgs &>(*this).operator=(static_cast<TArgs &&>(other)), 0)...};
                return *this;
            }
        };

        template <class T, class... TArgs>
        decltype(void(T{declval<TArgs>()...}), std::true_type{}) test_is_braces_constructible(int);

        template <class, class...> std::false_type test_is_braces_constructible(...);

        template <class T, class... TArgs>
        using is_braces_constructible = decltype(test_is_braces_constructible<T, TArgs...>(0));
        template <class T, class... TArgs>
        using is_braces_constructible_t = typename is_braces_constructible<T, TArgs...>::type;

        struct direct
        {
        };
        struct uniform
        {
        };

        template <class T, int> using get = T;

        template <template <class...> class, class, class, class = int> struct ctor_impl;

        template <template <class...> class TIsConstructible, class T>
        struct ctor_impl<TIsConstructible, T, std::index_sequence<>> : type_list<>
        {
        };

        template <template <class...> class TIsConstructible, class T>
        struct ctor_impl<TIsConstructible, T, std::index_sequence<0>,
                         __7_BIT_DI_REQUIRES(TIsConstructible<T, any_type_1st_fwd<T>>::value)>
            : type_list<any_type_1st_fwd<T>>
        {
        };

        template <template <class...> class TIsConstructible, class T>
        struct ctor_impl<TIsConstructible, T, std::index_sequence<0>,
                         __7_BIT_DI_REQUIRES(!TIsConstructible<T, any_type_1st_fwd<T>>::value)>
            : std::conditional_t<TIsConstructible<T, any_type_1st_ref_fwd<T>>::value,
                                 type_list<any_type_1st_ref_fwd<T>>, type_list<>>
        {
        };

        template <template <class...> class TIsConstructible, class T, int... Ns>
        struct ctor_impl<TIsConstructible, T, std::index_sequence<Ns...>,
                         __7_BIT_DI_REQUIRES((sizeof...(Ns) > 1) &&
                                             TIsConstructible<T, get<any_type_fwd<T>, Ns>...>::value)>
            : type_list<get<any_type_fwd<T>, Ns>...>
        {
        };

        template <template <class...> class TIsConstructible, class T, int... Ns>
        struct ctor_impl<TIsConstructible, T, std::index_sequence<Ns...>,
                         __7_BIT_DI_REQUIRES((sizeof...(Ns) > 1) &&
                                             !TIsConstructible<T, get<any_type_fwd<T>, Ns>...>::value)>
            : std::conditional<
                  TIsConstructible<T, get<any_type_ref_fwd<T>, Ns>...>::value,
                  type_list<get<any_type_ref_fwd<T>, Ns>...>,
                  typename ctor_impl<TIsConstructible, T, std::make_index_sequence<sizeof...(Ns) - 1>>::type>
        {
        };

        template <template <class...> class TIsConstructible, class T>
        using ctor_impl_t =
            typename ctor_impl<TIsConstructible, T, std::make_index_sequence<_7_BIT_DI_CFG_CTOR_LIMIT_SIZE>>::type;

        template <class...> struct ctor;

        template <class T> struct ctor<T, type_list<>> : std::pair<uniform, ctor_impl_t<is_braces_constructible, T>>
        {
        };

        template <class T, class... TArgs> struct ctor<T, type_list<TArgs...>> : std::pair<direct, type_list<TArgs...>>
        {
        };

        template <class T, class, class = typename is_injectable<ctor_traits<T>>::type> struct ctor_traits_impl;

        template <class T, class = void, class = void, class = typename is_injectable<T>::type> struct ctor_traits__;

        template <class T, class _1, class _2>
        struct ctor_traits__<T, _1, _2, std::true_type>
            : std::pair<T, std::pair<direct, typename T::boost_di_inject__::type>>
        {
        };

        template <class T, class _1, class _2>
        struct ctor_traits__<T, _1, _2, std::false_type> : ctor_traits_impl<T, _1>
        {
        };

        template <class T, class _1, class... Ts>
        struct ctor_traits__<T, _1, pool_t<Ts...>, std::false_type> : std::pair<T, std::pair<uniform, type_list<Ts...>>>
        {
        };

        template <class T, class _>
        struct ctor_traits_impl<T, _, std::true_type>
            : std::pair<T, std::pair<direct, typename ctor_traits<T>::boost_di_inject__::type>>
        {
        };

        template <class T, class _>
        struct ctor_traits_impl<T, _, std::false_type> : std::pair<T, typename ctor_traits<T>::type>
        {
        };

    } // namespace type_traits

    template <class T, class>
    struct ctor_traits : type_traits::ctor<T, type_traits::ctor_impl_t<std::is_constructible, T>>
    {
    };

    template <class T> struct ctor_traits<std::initializer_list<T>>
    {
        using boost_di_inject__ = type_traits::type_list<>;
    };

    template <class... Ts> struct ctor_traits<type_traits::type_list<Ts...>>
    {
        using boost_di_inject__ = type_traits::type_list<Ts...>;
    };

    template <class T>
    struct ctor_traits<T, __7_BIT_DI_REQUIRES(std::is_same<std::char_traits<char>, typename T::traits_type>::value)>
    {
        using boost_di_inject__ = type_traits::type_list<>;
    };

    template <class T> struct ctor_traits<T, __7_BIT_DI_REQUIRES(!std::is_class<T>::value)>
    {
        using boost_di_inject__ = type_traits::type_list<>;
    };

    template <class...> struct any;

    template <class TParent, class TInjector> struct any<type_traits::any_type_fwd<TParent>, TInjector>
    {
        using type = type_traits::any_type<TParent, TInjector>;
    };

    template <class TParent, class TInjector> struct any<type_traits::any_type_ref_fwd<TParent>, TInjector>
    {
        using type = type_traits::any_type_ref<TParent, TInjector>;
    };

    template <class TParent, class TInjector> struct any<type_traits::any_type_1st_fwd<TParent>, TInjector>
    {
        using type = type_traits::any_type_1st<TParent, TInjector>;
    };

    template <class TParent, class TInjector> struct any<type_traits::any_type_1st_ref_fwd<TParent>, TInjector>
    {
        using type = type_traits::any_type_1st_ref<TParent, TInjector>;
    };

    template <class T, class TInit, class... Ts> auto make_impl(std::pair<TInit, type_traits::type_list<Ts...>>)
    {
        return new T{typename any<Ts, injector>::type{*this}...};
    }

    template <class T, class TBinding>
    auto make(const TBinding &) -> decltype(make_impl<T>(typename ctor_traits<T>::type{}))
    {
        return make_impl<T>(typename ctor_traits<T>::type{});
    }

    struct A
    {
        A(int &, int, bool) {}
    };

    template <class T> struct false_type : std::false_type
    {
    };

    struct Maker
    {
        template <class T, class... TArgs> static auto get(TArgs &&...args)
        {
            return T(static_cast<TArgs &&>(args)...);
        }
    };

    template <class, class> struct provider;

    template <class T, template <class...> class TList, class... TCtor> struct provider<T, TList<TCtor...>>
    {
        using LIST = TList<TCtor...>;

        auto get() { return Maker::get<T>(TCtor{injector_}...); }

        ServiceProvider &injector_;
    };
} // namespace sevenBit
