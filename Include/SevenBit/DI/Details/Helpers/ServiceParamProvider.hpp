#pragma once

#include <memory>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/IsInPlaceObject.hpp"
#include "SevenBit/DI/Details/Utils/NotSupportedType.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

namespace sb::di::details
{

    template <class T> struct ServiceParamProvider
    {
        static auto getParam(ServiceProvider &provider)
        {
            if constexpr (IsInPlaceObjectV<T>)
            {
                return provider.createServiceInPlace<T>();
            }
            else
            {
                static_assert(notSupportedType<T>,
                              "Type is not supported as function argument parameter use objects, pointers, references, "
                              "std::unique_ptr<T>, std::vector containing pointers or std::unique_ptr<T>");
            }
        }
    };

    template <class T> struct ServiceParamProvider<T *>
    {
        static T *getParam(ServiceProvider &sp) { return sp.tryGetService<T>(); }
    };
    template <class T> struct ServiceParamProvider<const T *>
    {
        static const T *getParam(ServiceProvider &sp) { return sp.tryGetService<T>(); }
    };
    template <class T> struct ServiceParamProvider<T *const>
    {
        static T *getParam(ServiceProvider &sp) { return sp.tryGetService<T>(); }
    };
    template <class T> struct ServiceParamProvider<const T *const>
    {
        static const T *getParam(ServiceProvider &sp) { return sp.tryGetService<T>(); }
    };

    template <class T> struct ServiceParamProvider<T &>
    {
        static T &getParam(ServiceProvider &sp) { return sp.getService<T>(); }
    };
    template <class T> struct ServiceParamProvider<const T &>
    {
        static const T &getParam(ServiceProvider &sp) { return sp.getService<T>(); }
    };

    template <class T> struct ServiceParamProvider<std::unique_ptr<T>>
    {
        static std::unique_ptr<T> getParam(ServiceProvider &sp) { return sp.createService<T>(); }
    };
    template <class T> struct ServiceParamProvider<const std::unique_ptr<T>>
    {
        static std::unique_ptr<T> getParam(ServiceProvider &sp) { return sp.createService<T>(); }
    };

    template <class T> struct ServiceParamProvider<std::vector<T *>>
    {
        static std::vector<T *> getParam(ServiceProvider &sp) { return sp.getServices<T>(); }
    };
    template <class T> struct ServiceParamProvider<const std::vector<T *>>
    {
        static std::vector<T *> getParam(ServiceProvider &sp) { return sp.getServices<T>(); }
    };

    template <class T> struct ServiceParamProvider<std::vector<std::unique_ptr<T>>>
    {
        static std::vector<std::unique_ptr<T>> getParam(ServiceProvider &sp) { return sp.createServices<T>(); }
    };
    template <class T> struct ServiceParamProvider<const std::vector<std::unique_ptr<T>>>
    {
        static std::vector<std::unique_ptr<T>> getParam(ServiceProvider &sp) { return sp.createServices<T>(); }
    };
} // namespace sb::di::details
