#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/FunctorInjector.hpp"

namespace sb::di::details
{
    template <class F> struct FunctorInjectorResolver
    {
        using Injector = BadFunctorInjector<F>;
    };

    template <class R, class F, class... Args> struct FunctorInjectorResolver<R (F::*)(Args...) const>
    {
        using Injector = FunctorInjector<R, F, Args...>;
    };

    template <class R, class F, class... Args> struct FunctorInjectorResolver<R (F::*)(Args...)>
    {
        using Injector = FunctorInjector<R, F, Args...>;
    };

    template <class TFunctor>
    using ResolveFunctorInjector = typename FunctorInjectorResolver<decltype(&TFunctor::operator())>::Injector;
} // namespace sb::di::details
