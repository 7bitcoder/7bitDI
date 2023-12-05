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
    template <class F> struct TraitsInfo
    {
        TraitsInfo() { static_assert(utils::notSupportedType<F>, "Object is not functor/lambda"); }
    };

    template <class R, class F, class... Args> struct TraitsInfo<R (F::*)(Args...) const>
    {
        using ReturnType = R;
        using TupleArgs = std::tuple<Args...>;
    };

    template <class R, class F, class... Args> struct TraitsInfo<R (F::*)(Args...)>
    {
        using ReturnType = R;
        using TupleArgs = std::tuple<Args...>;
    };

    template <class F> struct FunctorTraits
    {
      private:
        using Info = TraitsInfo<decltype(&F::operator())>;

      public:
        using Function = F;
        using ReturnType = typename Info::ReturnType;
        using TupleArgs = typename Info::TupleArgs;
        
        static constexpr size_t ArgsSize = std::tuple_size_v<TupleArgs>;

        template <size_t I> struct Arg
        {
            using Type = typename std::tuple_element<I, TupleArgs>::type;
        };
    };
} // namespace sb::di::details::helpers
