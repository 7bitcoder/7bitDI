#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceGetter.hpp"
#include "SevenBit/DI/Details/Utils/IsUniquePtr.hpp"

namespace sb::di::details
{
    class ServiceExtractor
    {
        ServiceProvider &_provider;

      public:
        explicit ServiceExtractor(ServiceProvider &provider) : _provider(provider) {}

        //
        // template <class S> operator S *() { return ServiceGetter<S *>::get(_provider); }
        // template <class S> operator std::vector<S *>() { return ServiceGetter<std::vector<S *>>::get(_provider); }
        // template <class S> operator std::vector<std::unique_ptr<S>>()
        // {
        //     return ServiceGetter<std::vector<std::unique_ptr<S>>>::get(_provider);
        // }

        // template <class S, class = std::enable_if_t<!IsUniquePtrV<S>>> operator S()
        // {
        //     return ServiceGetter<S>::get(_provider);
        // }
        //
        // template <class S> operator S() { return {}; }
        template <class S, class = std::enable_if_t<!std::is_null_pointer_v<S>>> operator S &&() const
        {
            return ServiceGetter<S>::get(_provider);
        }
        // template <class S, class = std::enable_if_t<!IsUniquePtrV<S> && !std::is_null_pointer_v<S> && !IsVectorV<S>>>
        // operator S &() const
        // {
        //     return _provider.getService<S>();
        // }
    };
} // namespace sb::di::details
