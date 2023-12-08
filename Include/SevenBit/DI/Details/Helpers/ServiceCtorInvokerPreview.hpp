#include <utility>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceParamProvider.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

#define __7_BIT_DI_REQUIRES(...) typename std::enable_if<__VA_ARGS__, int>::type

namespace sb::di::details::helpers
{

    namespace ServiceCtorInvokerInternals
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

        template <class> struct any_type_fwd
        {
            template <class T> operator T() { return ServiceParamProvider<T>{}.getParam(provider); }

            ServiceProvider &provider;

          private:
            template <class T> operator T &() { return ServiceParamProvider<T &>{}.getParam(provider); }
        };
        template <class> struct any_type_ref_fwd
        {
            template <class T> operator T() { return ServiceParamProvider<T>{}.getParam(provider); }

            ServiceProvider &provider;
        };
        template <class TParent> struct any_type_1st_fwd
        {
            template <class T, class = __7_BIT_DI_REQUIRES(!is_copy_ctor__<TParent, T>::value)> operator T()
            {
                return ServiceParamProvider<T>{}.getParam(provider);
            }

            ServiceProvider &provider;

          private:
            template <class T, class = __7_BIT_DI_REQUIRES(!is_copy_ctor__<TParent, T>::value)> operator T &()
            {
                return ServiceParamProvider<T &>{}.getParam(provider);
            }
        };
        template <class TParent> struct any_type_1st_ref_fwd
        {
            template <class T, class = __7_BIT_DI_REQUIRES(!is_copy_ctor__<TParent, T>::value)> operator T()
            {
                return ServiceParamProvider<T>{}.getParam(provider);
            }

            ServiceProvider &provider;
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
            typename ctor_impl<TIsConstructible, T, std::make_index_sequence<_7_BIT_DI_CTOR_LIMIT_SIZE>>::type;

        template <class...> struct ctor;

        template <class T, class... TArgs> struct ctor<T, type_list<TArgs...>> : type_list<TArgs...>
        {
        };

        template <class, class = int> struct ctor_traits;

        template <class T, class> struct ctor_traits : ctor<T, ctor_impl_t<std::is_constructible, T>>
        {
        };

        struct CtorInvoker
        {
            template <class T, class... TArgs> static auto invoke(TArgs &&...args)
            {
                return T(static_cast<TArgs &&>(args)...);
            }
        };

        template <class, class> struct Resolver;

        template <class T, template <class...> class TList, class... TCtor> struct Resolver<T, TList<TCtor...>>
        {
            auto resolve() { return CtorInvoker::invoke<T>(TCtor{provider}...); }

            template <class TFunc> auto resolveWithCtorParams(TFunc &&func) { return func(TCtor{provider}...); }

            ServiceProvider &provider;
        };
    } // namespace ServiceCtorInvokerInternals

    template <class T> struct ServiceCtorInvoker
    {
        auto invoke()
        {
            using namespace ServiceCtorInvokerInternals;
            return Resolver<T, typename ctor_traits<T>::type>{provider}.resolve();
        }

        template <class TFunc> auto invokeWithCtorParams(TFunc &&func)
        {
            using namespace ServiceCtorInvokerInternals;
            return Resolver<T, typename ctor_traits<T>::type>{provider}.resolveWithCtorParams(
                std::forward<TFunc>(func));
        }

        ServiceProvider &provider;
    };
} // namespace sb::di::details::helpers
