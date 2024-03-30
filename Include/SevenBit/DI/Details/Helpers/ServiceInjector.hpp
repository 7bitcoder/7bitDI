#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceGetter.hpp"
#include "SevenBit/DI/Details/Utils/IsCopyCtor.hpp"

namespace sb::di::details
{
    class ServiceInjector
    {
        ServiceProvider &_provider;

      public:
        explicit ServiceInjector(ServiceProvider &provider) : _provider(provider) {}

        template <class S> operator S() { return ServiceGetter<S>::get(_provider); }
        template <class S> operator S &() const { return ServiceGetter<S &>::get(_provider); }
    };
} // namespace sb::di::details
