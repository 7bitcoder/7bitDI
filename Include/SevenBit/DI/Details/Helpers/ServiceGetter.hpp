#pragma once

#include <memory>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/Meta.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

namespace sb::di::details
{

    template <class T> struct ServiceGetter
    {
        static_assert(IsInPlaceServiceV<T> || notSupportedType<T>,
                      "Type is not supported as function argument parameter use objects, pointers, references, "
                      "std::unique_ptr<T>, std::vector containing pointers or std::unique_ptr<T>");

        static auto get(ServiceProvider &provider) { return provider.createServiceInPlace<T>(); }
        static auto get(ServiceProvider &provider, const std::string_view key)
        {
            return provider.createKeyedServiceInPlace<T>(key);
        }
    };

    template <class T> struct ServiceGetter<T *>
    {
        static T *get(ServiceProvider &sp) { return sp.tryGetService<T>(); }
        static T *get(ServiceProvider &sp, const std::string_view key) { return sp.tryGetKeyedService<T>(key); }
    };

    template <class T> struct ServiceGetter<T &>
    {
        static T &get(ServiceProvider &sp) { return sp.getService<T>(); }
        static T &get(ServiceProvider &sp, const std::string_view key) { return sp.getKeyedService<T>(key); }
    };

    template <class T> struct ServiceGetter<std::vector<T *>>
    {
        static std::vector<T *> get(ServiceProvider &sp) { return sp.getServices<T>(); }
        static std::vector<T *> get(ServiceProvider &sp, const std::string_view key)
        {
            return sp.getKeyedServices<T>(key);
        }
    };

    template <class T> struct ServiceGetter<std::unique_ptr<T>>
    {
        static std::unique_ptr<T> get(ServiceProvider &sp) { return sp.createService<T>(); }
        static std::unique_ptr<T> get(ServiceProvider &sp, const std::string_view key)
        {
            return sp.createKeyedService<T>(key);
        }
    };

    template <class T> struct ServiceGetter<std::vector<std::unique_ptr<T>>>
    {
        static std::vector<std::unique_ptr<T>> get(ServiceProvider &sp) { return sp.createServices<T>(); }
        static std::vector<std::unique_ptr<T>> get(ServiceProvider &sp, const std::string_view key)
        {
            return sp.createKeyedServices<T>(key);
        }
    };
} // namespace sb::di::details
