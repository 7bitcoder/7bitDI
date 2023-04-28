#pragma once

#include <memory>
#include <vector>

#include "SevenBit/ServiceProvider.hpp"

namespace sb
{
    template <class... T> inline constexpr bool notSupportedType = false;

    template <class T> struct CtorArgProvider
    {
        auto getService(ServiceProvider &provider)
        {
            static_assert(notSupportedType<T>, "Type is not supported as function augument parameter");
        }
    };

    template <class T> struct CtorArgProvider<T *>
    {
        auto getService(ServiceProvider &sp) { return &sp.getRequiredService<T>(); }
    };
    template <class T> struct CtorArgProvider<const T *>
    {
        auto getService(ServiceProvider &sp) { return &sp.getRequiredService<T>(); }
    };
    template <class T> struct CtorArgProvider<T *const>
    {
        auto getService(ServiceProvider &sp) { return &sp.getRequiredService<T>(); }
    };
    template <class T> struct CtorArgProvider<const T *const>
    {
        auto getService(ServiceProvider &sp) { return &sp.getRequiredService<T>(); }
    };

    template <class T> struct CtorArgProvider<T &>
    {
        auto getService(ServiceProvider &sp) { return sp.getRequiredService<T>(); }
    };
    template <class T> struct CtorArgProvider<const T &>
    {
        auto getService(ServiceProvider &sp) { return sp.getRequiredService<T>(); }
    };

    template <class T> struct CtorArgProvider<std::unique_ptr<T>>
    {
        auto getService(ServiceProvider &sp) { return sp.createService<T>(); }
    };
    template <class T> struct CtorArgProvider<const std::unique_ptr<T>>
    {
        auto getService(ServiceProvider &sp) { return sp.createService<T>(); }
    };

    template <class T> struct CtorArgProvider<std::vector<T *>>
    {
        auto getService(ServiceProvider &sp) { return sp.getServices<T>(); }
    };
    template <class T> struct CtorArgProvider<const std::vector<T *>>
    {
        auto getService(ServiceProvider &sp) { return sp.getServices<T>(); }
    };

    template <class T> struct CtorArgProvider<std::vector<std::unique_ptr<T>>>
    {
        auto getService(ServiceProvider &sp) { return sp.createServices<T>(); }
    };
    template <class T> struct CtorArgProvider<const std::vector<std::unique_ptr<T>>>
    {
        auto getService(ServiceProvider &sp) { return sp.createServices<T>(); }
    };

    template <class T> struct CtorArgProvider<std::vector<T>>
    {
        static_assert(notSupportedType<T>,
                      "Vector should contain pointners or unique pointners for transient services");
    };
} // namespace sb
