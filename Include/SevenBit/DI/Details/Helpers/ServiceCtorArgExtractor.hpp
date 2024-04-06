#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceGetter.hpp"
#include "SevenBit/DI/Details/Utils/Meta.hpp"

namespace sb::di::details
{
    template <class T> class ServiceCtorArgExtractor
    {
        ServiceProvider &_provider;

      public:
        explicit ServiceCtorArgExtractor(ServiceProvider &provider, std::size_t) : _provider(provider) {}

        template <class S, class = std::enable_if_t<!IsCopyCtorV<T, S>>> operator S()
        {
            return ServiceGetter<std::remove_cv_t<S>>::get(_provider);
        }
        template <class S, class = std::enable_if_t<!IsCopyCtorV<T, S>>> operator S &() const
        {
            return _provider.getService<std::remove_cv_t<S>>();
        }
    };
} // namespace sb::di::details
