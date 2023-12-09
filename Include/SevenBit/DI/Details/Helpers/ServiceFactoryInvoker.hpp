#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceParamProvider.hpp"

namespace sb::di::details::helpers
{
    namespace ServiceFactoryInvokerInternals
    {
        template <class R, class F, class... Args> class FunctorInvoker
        {
            F &_factory;
            ServiceProvider &_serviceProvider;

          public:
            using ReturnType = R;

            FunctorInvoker(F &factory, ServiceProvider &serviceProvider)
                : _factory(factory), _serviceProvider(serviceProvider)
            {
            }

            R invoke() { return _factory(ServiceParamProvider<Args>::getParam(_serviceProvider)...); }
        };

        template <class F> struct BadFunctor
        {
            using ReturnType = int;

            BadFunctor(F &factory, ServiceProvider &serviceProvider)
            {
                static_assert(utils::notSupportedType<F>, "Object is not functor/lambda");
            }

            int invoke() { return 0; }
        };

        template <class F> struct FunctorInvokerResolver
        {
            using Invoker = BadFunctor<F>;
        };

        template <class R, class F, class... Args> struct FunctorInvokerResolver<R (F::*)(Args...) const>
        {
            using Invoker = FunctorInvoker<R, F, Args...>;
        };

        template <class R, class F, class... Args> struct FunctorInvokerResolver<R (F::*)(Args...)>
        {
            using Invoker = FunctorInvoker<R, F, Args...>;
        };

    } // namespace ServiceFactoryInvokerInternals

    template <class FactoryFcn>
    using ServiceFactoryInvoker =
        typename ServiceFactoryInvokerInternals::FunctorInvokerResolver<decltype(&FactoryFcn::operator())>::Invoker;
} // namespace sb::di::details::helpers
