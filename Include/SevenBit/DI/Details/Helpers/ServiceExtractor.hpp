#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceGetter.hpp"

namespace sb::di::details
{
    class ServiceExtractor
    {
        ServiceProvider &_provider;

      public:
        explicit ServiceExtractor(ServiceProvider &provider) : _provider(provider) {}

        template <class T> operator T() { return ServiceGetter<T>::get(_provider); }
        template <class T> operator T &() const { return ServiceGetter<T &>::get(_provider); }
    };
} // namespace sb::di::details
