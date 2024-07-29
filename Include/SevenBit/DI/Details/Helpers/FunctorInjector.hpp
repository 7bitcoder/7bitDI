#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "ServiceGetter.hpp"

namespace sb::di::details
{
    template <class F> class FunctorInjector
    {
        static_assert(IsFunctorV<F> || notSupportedType<F>, "Object is not functor/lambda");

        F &_functor;
        ServiceProvider &_serviceProvider;

      public:
        FunctorInjector(F &functor, ServiceProvider &provider) : _functor(functor), _serviceProvider(provider) {}
        FunctorInjector(F *functor, ServiceProvider *provider) : FunctorInjector(*functor, *provider) {}

        auto operator()() { return matchCall(&F::operator()); }

        template <class TWrapper> std::unique_ptr<TWrapper> makeUnique()
        {
            return std::make_unique<TWrapper>((*this)());
        }

      private:
        template <class R, class T, class... Args> R matchCall(R (T::*)(Args...)) { return call<R, Args...>(); }

        template <class R, class T, class... Args> R matchCall(R (T::*)(Args...) const) { return call<R, Args...>(); }

        template <class R, class... Args> R call() { return _functor(ServiceGetter<Args>::get(_serviceProvider)...); }
    };
} // namespace sb::di::details
