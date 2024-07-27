#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Meta/ServiceGetter.hpp"

namespace sb::di::details
{
    template <class R, class F, class... Args> class FunctorInjector
    {
        F &_functor;
        ServiceProvider &_serviceProvider;

      public:
        using ReturnType = R;

        FunctorInjector(F &functor, ServiceProvider &serviceProvider)
            : _functor(functor), _serviceProvider(serviceProvider)
        {
        }

        template <class TWrapper> std::unique_ptr<TWrapper> makeUnique()
        {
            return std::make_unique<TWrapper>(_functor(ServiceGetter<Args>::get(_serviceProvider)...));
        }
    };

    template <class F> struct BadFunctorInjector
    {
        using ReturnType = int;

        BadFunctorInjector(F &, ServiceProvider &)
        {
            static_assert(notSupportedType<F>, "Object is not functor/lambda");
        }

        template <class TWrapper> std::unique_ptr<TWrapper> makeUnique() { return nullptr; }
    };
} // namespace sb::di::details
