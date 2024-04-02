#pragma once

#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceGetter.hpp"
#include "SevenBit/DI/Details/Utils/Meta.hpp"

namespace sb::di
{
    struct ServiceExtractor
    {
        ServiceProvider &_provider;

        template <class S, class = std::enable_if_t<!std::is_null_pointer_v<S> && !details::IsInitializerListV<S>>>
        operator S()
        {
            return details::ServiceGetter<std::remove_cv_t<S>>::get(_provider);
        }
        template <class S, class = std::enable_if_t<!std::is_const_v<S> && !std::is_null_pointer_v<S> &&
                                                    !details::IsInitializerListV<S>>>
        operator S &() const
        {
            return _provider.getService<std::remove_cv_t<S>>();
        }
    };
} // namespace sb::di
