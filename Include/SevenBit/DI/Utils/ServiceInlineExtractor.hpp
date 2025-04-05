#pragma once

#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceGetter.hpp"
#include "SevenBit/DI/Details/Utils/Meta.hpp"

namespace sb::di
{
    struct ServiceInlineExtractor
    {
        ServiceProvider &provider;

        template <class S, class = std::enable_if_t<!std::is_null_pointer_v<S> && !details::IsInitializerListV<S>>>
        operator S()
        {
            return details::ServiceGetter<std::remove_cv_t<S>>::get(provider);
        }
        template <class S, class = std::enable_if_t<!std::is_const_v<S> && !std::is_null_pointer_v<S> &&
                                                    !details::IsInitializerListV<S>>>
        operator S &() const
        {
            return provider.getService<std::remove_cv_t<S>>();
        }
    };

    struct KeyedServiceInlineExtractor
    {
        ServiceProvider &provider;
        const std::string_view key;

        template <class S, class = std::enable_if_t<!std::is_null_pointer_v<S> && !details::IsInitializerListV<S>>>
        operator S()
        {
            return details::ServiceGetter<std::remove_cv_t<S>>::get(provider, key);
        }
        template <class S, class = std::enable_if_t<!std::is_const_v<S> && !std::is_null_pointer_v<S> &&
                                                    !details::IsInitializerListV<S>>>
        operator S &() const
        {
            return provider.getKeyedService<std::remove_cv_t<S>>(key);
        }
    };
} // namespace sb::di
