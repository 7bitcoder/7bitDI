#pragma once

#include <string>
#include <string_view>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details
{

    class String
    {
      public:
        template <class... Args> static std::string join(std::string &&start, Args... args)
        {
            return (start + ... + (std::string{" "} + args));
        }

        template <class... Args> static std::string quote(std::string &&value) { return "'" + value + "'"; }
    };
} // namespace sb::di::details
