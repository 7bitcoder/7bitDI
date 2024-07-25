#pragma once

#include <string>
#include <string_view>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/Formatter.hpp"

namespace sb::di::details
{
    class Format
    {
      public:
        template <class... Args> static std::string fmt(std::string_view formatString, Args &&...args)
        {
            Formatter formatter{formatString};
            formatter.format(args...);
            return std::move(formatter).getResult();
        }
    };
} // namespace sb::di::details
