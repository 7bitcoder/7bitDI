#pragma once

#include <string>
#include <string_view>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/Formatter.hpp"

namespace sb::di::details
{
    struct String
    {
        template <class... Args> static std::string fmt(const std::string_view formatString, Args &&...args)
        {
            Formatter formatter{formatString};
            formatter.format(args...);
            return std::move(formatter).getResult();
        }

        template <class T> static std::string toString(T &&value) { return fmt("{}", value); }
    };
} // namespace sb::di::details
