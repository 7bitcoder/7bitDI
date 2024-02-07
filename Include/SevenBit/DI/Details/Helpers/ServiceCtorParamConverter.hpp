#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceParamProvider.hpp"
#include "SevenBit/DI/Details/Utils/IsCopyCtor.hpp"

namespace sb::di::details::helpers
{
    template <class T> class ServiceCtorParamConverter
    {
        ServiceProvider &_provider;

      public:
        explicit ServiceCtorParamConverter(ServiceProvider &provider, size_t argPos) : _provider(provider) {}

        template <class U, class = std::enable_if_t<!utils::IsCopyCtorV<T, U>>> operator U()
        {
            return helpers::ServiceParamProvider<U>::getParam(_provider);
        }
        template <class U, class = std::enable_if_t<!utils::IsCopyCtorV<T, U>>> operator U &() const
        {
            return helpers::ServiceParamProvider<U &>::getParam(_provider);
        }
    };
} // namespace sb::di::details::helpers
