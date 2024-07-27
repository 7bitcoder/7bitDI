#pragma once

#include <memory>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Meta/Type.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

namespace sb::di::details
{

    template <class T> struct ServiceGetter
    {
        static auto get(ServiceProvider &provider)
        {
            if constexpr (IsInPlaceServiceV<T>)
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

    template <class T> struct ServiceGetter<T *>
    {
        static T *get(ServiceProvider &sp) { return sp.tryGetService<T>(); }
    };

    template <class T> struct ServiceGetter<T &>
    {
        static T &get(ServiceProvider &sp) { return sp.getService<T>(); }
    };

    template <class T> struct ServiceGetter<std::vector<T *>>
    {
        static std::vector<T *> get(ServiceProvider &sp) { return sp.getServices<T>(); }
    };

    template <class T> struct ServiceGetter<std::unique_ptr<T>>
    {
        static std::unique_ptr<T> get(ServiceProvider &sp) { return sp.createService<T>(); }
    };

    template <class T> struct ServiceGetter<std::vector<std::unique_ptr<T>>>
    {
        static std::vector<std::unique_ptr<T>> get(ServiceProvider &sp) { return sp.createServices<T>(); }
    };
} // namespace sb::di::details
