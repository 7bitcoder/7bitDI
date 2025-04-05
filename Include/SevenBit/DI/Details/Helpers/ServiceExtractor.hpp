#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "ServiceGetter.hpp"
#include "SevenBit/DI/Details/Utils/Meta.hpp"

namespace sb::di::details
{
    template <class T> class ServiceExtractor
    {
        ServiceProvider *_provider = nullptr;

      public:
        explicit ServiceExtractor(ServiceProvider *provider, std::size_t paramNumber = 0) : _provider(provider) {}

        template <class S, class = std::enable_if_t<!IsCopyCtorV<T, S>>> operator S()
        {
            return ServiceGetter<std::remove_cv_t<S>>::get(*_provider);
        }
        template <class S, class = std::enable_if_t<!IsCopyCtorV<T, S>>> operator S &() const
        {
            return _provider->getService<std::remove_cv_t<S>>();
        }
    };

    template <class T> class KeyedServiceExtractor
    {
        ServiceProvider *_provider = nullptr;
        const std::string_view _key;

      public:
        explicit KeyedServiceExtractor(ServiceProvider *provider, const std::string_view key,
                                       std::size_t paramNumber = 0)
            : _provider(provider), _key(key)
        {
        }

        template <class S, class = std::enable_if_t<!IsCopyCtorV<T, S>>> operator S()
        {
            return ServiceGetter<std::remove_cv_t<S>>::get(*_provider, _key);
        }
        template <class S, class = std::enable_if_t<!IsCopyCtorV<T, S>>> operator S &() const
        {
            return _provider->getKeyedService<std::remove_cv_t<S>>(_key);
        }
    };
} // namespace sb::di::details
