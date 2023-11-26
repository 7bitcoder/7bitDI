#pragma once

#include <functional>
#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/ServiceParamProvider.hpp"
#include "SevenBit/DI/Details/Utils.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details
{
    template <class R, class F, class... Args> struct ServiceFactoryInvoker
    {
      private:
        using IsUniquePtr = utils::IsUniquePtr<R>;
        mutable F _factory;

      public:
        using TService = typename IsUniquePtr::Type;

        ServiceFactoryInvoker(F &&factory) : _factory(std::move(factory))
        {
            static_assert(IsUniquePtr::value || notSupportedType<F>,
                          "Factory return type must be std::unique_ptr<TService>");
        }

        R invoke(sb::di::ServiceProvider &provider) const
        {
            return _factory(ServiceParamProvider<Args>{}.getService(provider)...);
        }
    };

    template <class F> struct ServiceTraitsExtractor
    {
        using TService = F;

        ServiceTraitsExtractor(F &&factory)
        {
            static_assert(utils::notSupportedType<F>,
                          "Factory function should have this scheme: (Services...) -> "
                          "std::unique_ptr<T>, each service can be reference, pointner, std::unique_ptr<T> or vector "
                          "containing pointners or std::unique_ptr<T>");
        }

        std::unique_ptr<int> invoke(sb::di::ServiceProvider &provider) { return std::make_unique<int>(0); }
    };

    template <class R, class F, class... Args>
    struct ServiceTraitsExtractor<R (F::*)(Args...) const> : public ServiceFactoryInvoker<R, F, Args...>
    {
        ServiceTraitsExtractor(F &&factory) : ServiceFactoryInvoker<R, F, Args...>(std::move(factory)) {}
    };

    template <class R, class F, class... Args> struct ServiceTraitsExtractor<R (F::*)(Args...)>
    {
        ServiceTraitsExtractor(F &&factory) : ServiceFactoryInvoker<R, F, Args...>(std::move(factory)) {}
    };

    template <class F, typename = void> struct ServiceFactoryWrapper : public ServiceTraitsExtractor<F>
    {
        ServiceFactoryWrapper(F &&factory) : ServiceTraitsExtractor<F>(std::move(factory)) {}
    };

    template <class F>
    struct ServiceFactoryWrapper<F, decltype(void(&F::operator()))>
        : public ServiceTraitsExtractor<decltype(&F::operator())>
    {
        ServiceFactoryWrapper(F &&factory) : ServiceTraitsExtractor<decltype(&F::operator())>(std::move(factory)) {}
    };
} // namespace sb::di::details
