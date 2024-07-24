#pragma once

#include <string>
#include <string_view>

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details
{
    class String
    {
      public:
        template <class... Args> static std::string join(std::string &&start, Args... strings)
        {
            return (start + ... + (std::string{" "} + strings));
        }

        static std::string quote(std::string &&value) { return "'" + value + "'"; }
    };
} // namespace sb::di::details
