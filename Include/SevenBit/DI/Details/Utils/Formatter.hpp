#pragma once

#include <string>
#include <string_view>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/String.hpp"

namespace sb::di::details
{
    class Formatter
    {
        std::string_view formatString;
        std::string_view::size_type current = 0;
        std::string result;

      public:
        explicit Formatter(std::string_view formatString) : formatString(formatString)
        {
            result.reserve(formatString.size());
        }

        template <class... Args> int format(Args &&...args)
        {
            auto used = (... + process(args));
            result += formatString.substr(current);
            return used;
        }

        std::string getResult() && { return std::move(result); }

      private:
        template <class Arg> int process(Arg &&arg)
        {
            if (auto start = formatString.find_first_of('{', current); start != std::string_view::npos)
            {
                result += formatString.substr(current, start - current);
                auto end = formatString.find_first_of('}', start++);
                if (end == std::string_view::npos)
                {
                    throw std::runtime_error("Invalid format string '{' should end with '}'");
                }
                result += String::toString(std::forward<Arg>(arg), formatString.substr(start, end - start));
                current = end + 1;
                return 1;
            }
            result += formatString.substr(current);
            current = formatString.size();
            return 0;
        }
    };
} // namespace sb::di::details
