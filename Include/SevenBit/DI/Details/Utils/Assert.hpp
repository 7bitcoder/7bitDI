#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"

namespace sb::di::details::utils
{
    struct EXPORT Assert
    {
        template <class TService, class TImplementation> static void inheritance()
        {
            static_assert(std::is_same_v<TService, TImplementation> || std::is_base_of_v<TService, TImplementation>,
                          "Type TImplementation must inherit from TService");
        }
    };
} // namespace sb::di::details::utils