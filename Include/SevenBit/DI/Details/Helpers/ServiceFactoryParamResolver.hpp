#pragma once

#include <functional>
#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceParamProvider.hpp"
#include "SevenBit/DI/Details/Utils/IsInPlaceObject.hpp"
#include "SevenBit/DI/Details/Utils/IsUniquePtr.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details::helpers
{
    template <class F> struct FunctorResolver
    {
        using ReturnType = int;

        static int invoke(F &factory, ServiceProvider &provider)
        {
            static_assert(utils::notSupportedType<F>, "Object is not functor/lambda");
            return 0;
        }
    };

    template <class R, class F, class... Args> struct FunctorResolver<R (F::*)(Args...) const>
    {
        using ReturnType = R;

        static R invoke(F &factory, ServiceProvider &provider)
        {
            return factory(ServiceParamProvider<Args>{}.getParam(provider)...);
        }
    };

    template <class R, class F, class... Args> struct FunctorResolver<R (F::*)(Args...)>
    {
        using ReturnType = R;

        static R invoke(F &factory, ServiceProvider &provider)
        {
            return factory(ServiceParamProvider<Args>{}.getParam(provider)...);
        }
    };

    template <class F> using ServiceFactoryParamResolver = FunctorResolver<decltype(&F::operator())>;
} // namespace sb::di::details::helpers
