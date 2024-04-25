#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceGetter.hpp"

namespace sb::di::details
{
    template <class R, class F, class... Args> class InternalFunctorInjector
    {
        F &_functor;
        ServiceProvider &_serviceProvider;

      public:
        using ReturnType = R;

        InternalFunctorInjector(F &functor, ServiceProvider &serviceProvider)
            : _functor(functor), _serviceProvider(serviceProvider)
        {
        }

        R call() { return _functor(ServiceGetter<Args>::get(_serviceProvider)...); }
    };

    template <class F> struct InternalBadFunctor
    {
        using ReturnType = int;

        InternalBadFunctor(F &, ServiceProvider &)
        {
            static_assert(notSupportedType<F>, "Object is not functor/lambda");
        }

        int call() { return 0; }
    };

    template <class F> struct FunctorInjectorResolver
    {
        using Injector = InternalBadFunctor<F>;
    };

    template <class R, class F, class... Args> struct FunctorInjectorResolver<R (F::*)(Args...) const>
    {
        using Injector = InternalFunctorInjector<R, F, Args...>;
    };

    template <class R, class F, class... Args> struct FunctorInjectorResolver<R (F::*)(Args...)>
    {
        using Injector = InternalFunctorInjector<R, F, Args...>;
    };

    template <class TFunctor>
    using FunctorInjector = typename FunctorInjectorResolver<decltype(&TFunctor::operator())>::Injector;
} // namespace sb::di::details
