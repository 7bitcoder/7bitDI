#pragma once

#include <memory>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ServiceParamProvider.hpp"
#include "SevenBit/DI/Details/Utils/IsCopyCtor.hpp"

namespace sb::di::details::helpers
{
    template <class T, size_t N = 0> class ServiceCtorParamConverter
    {
      private:
        ServiceProvider *_provider = nullptr;

      public:
        ServiceCtorParamConverter() = default;
        ServiceCtorParamConverter(ServiceProvider &provider) { _provider = &provider; }

        template <class U, class = typename std::enable_if_t<!utils::IsCopyCtorV<T, U>>> operator U()
        {
            return helpers::ServiceParamProvider<U>{}.getParam(*_provider);
        }
        template <class U, class = typename std::enable_if_t<!utils::IsCopyCtorV<T, U>>> operator U &() const
        {
            return helpers::ServiceParamProvider<U &>{}.getParam(*_provider);
        }
    };
} // namespace sb::di::details::helpers
