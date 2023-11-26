#pragma once

#include <memory>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

namespace sb::di::details
{

    template <class... T> inline constexpr bool notSupportedType = false;

    template <class T> struct ServiceParamProvider
    {
        auto getService(ServiceProvider &provider)
        {
            static_assert(
                utils::notSupportedType<T>,
                "Type is not supported as function argument parameter, use "
                "poitners, references, std::unique_ptr<T>, or std::vector containing pointners or std::unique_ptr<T>");
        }
    };

    template <class T> struct ServiceParamProvider<T *>
    {
        T *getService(ServiceProvider &sp) { return &sp.getService<T>(); }
    };
    template <class T> struct ServiceParamProvider<const T *>
    {
        const T *getService(ServiceProvider &sp) { return &sp.getService<T>(); }
    };
    template <class T> struct ServiceParamProvider<T *const>
    {
        T *const getService(ServiceProvider &sp) { return &sp.getService<T>(); }
    };
    template <class T> struct ServiceParamProvider<const T *const>
    {
        const T *const getService(ServiceProvider &sp) { return &sp.getService<T>(); }
    };

    template <class T> struct ServiceParamProvider<T &>
    {
        T &getService(ServiceProvider &sp) { return sp.getService<T>(); }
    };
    template <class T> struct ServiceParamProvider<const T &>
    {
        const T &getService(ServiceProvider &sp) { return sp.getService<T>(); }
    };

    template <class T> struct ServiceParamProvider<std::unique_ptr<T>>
    {
        std::unique_ptr<T> getService(ServiceProvider &sp) { return sp.createService<T>(); }
    };
    template <class T> struct ServiceParamProvider<const std::unique_ptr<T>>
    {
        const std::unique_ptr<T> getService(ServiceProvider &sp) { return sp.createService<T>(); }
    };

    template <class T> struct ServiceParamProvider<std::vector<T *>>
    {
        std::vector<T *> getService(ServiceProvider &sp) { return sp.getServices<T>(); }
    };
    template <class T> struct ServiceParamProvider<const std::vector<T *>>
    {
        const std::vector<T *> getService(ServiceProvider &sp) { return sp.getServices<T>(); }
    };

    template <class T> struct ServiceParamProvider<std::vector<std::unique_ptr<T>>>
    {
        std::vector<std::unique_ptr<T>> getService(ServiceProvider &sp) { return sp.createServices<T>(); }
    };
    template <class T> struct ServiceParamProvider<const std::vector<std::unique_ptr<T>>>
    {
        const std::vector<std::unique_ptr<T>> getService(ServiceProvider &sp) { return sp.createServices<T>(); }
    };

    template <class T> struct ServiceParamProvider<std::vector<T>>
    {
        static_assert(notSupportedType<T>,
                      "Vector should contain pointners or unique pointners for transient services");
    };
} // namespace sb::di::details
