#pragma once

#include <string>
#include <string_view>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/StringUtils.hpp"

namespace sb::di::details
{
    class Formatter
    {
        std::string_view formatString;
        std::string_view::size_type current = 0;
        std::string result;

      public:
        explicit Formatter(const std::string_view formatString) : formatString(formatString)
        {
            result.reserve(formatString.size());
        }

        template <class... Args> bool format(Args &&...args)
        {
            volatile auto processed = (... && process(args));
            result += formatString.substr(current);
            return processed;
        }

        std::string getResult() && { return std::move(result); }

      private:
        template <class Arg> bool process(Arg &&arg)
        {
            if (auto start = formatString.find_first_of('{', current); start != std::string_view::npos)
            {
                result += formatString.substr(current, start - current);
                const auto end = formatString.find_first_of('}', start++);
                if (end == std::string_view::npos)
                {
                    throw std::runtime_error("Invalid format string '{' should end with '}'");
                }
                result += StringUtils::toString(std::forward<Arg>(arg), formatString.substr(start, end - start));
                current = end + 1;
                return true;
            }
            return false;
        }
    };
} // namespace sb::di::details
