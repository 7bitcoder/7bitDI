#pragma once

#include <string>
#include <string_view>

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details
{
    class EXPORT Formatter
    {
        std::string_view formatString;
        std::string_view::size_type current = 0;
        std::string result;

      public:
        explicit Formatter(std::string_view formatString);

        template <class... Args> bool format(Args &&...args)
        {
            volatile auto processed = (... && process(args));
            append(formatString.substr(current));
            return processed;
        }

        std::string getResult() &&;

      private:
        template <class Arg> bool process(Arg &&arg)
        {
            if (auto start = formatString.find_first_of('{', current); start != std::string_view::npos)
            {
                append(formatString.substr(current, start - current));
                const auto end = formatString.find_first_of('}', start++);
                if (end == std::string_view::npos)
                {
                    throw std::runtime_error("Invalid fmt string '{' should end with '}'");
                }
                append(std::forward<Arg>(arg), formatString.substr(start, end - start));
                current = end + 1;
                return true;
            }
            return false;
        }

        void append(const char *arg, std::string_view fmt = "");
        void append(const std::string &arg, std::string_view fmt = "");
        void append(std::string_view arg, std::string_view fmt = "");
        void append(int arg, std::string_view fmt = "");
        void append(long arg, std::string_view fmt = "");
        void append(long long arg, std::string_view fmt = "");
        void append(unsigned arg, std::string_view fmt = "");
        void append(unsigned long arg, std::string_view fmt = "");
        void append(unsigned long long arg, std::string_view fmt = "");
        void append(float arg, std::string_view fmt = "");
        void append(double arg, std::string_view fmt = "");
        void append(long double arg, std::string_view fmt = "");

        template <class N> void appendNum(N arg, const std::string_view coreFmt, const char *baseFmt)
        {
            if (!coreFmt.empty())
            {
                const auto format = makeArgFmt(coreFmt, baseFmt);
                return appendFormatted(arg, format.c_str());
            }
            return appendFormatted(arg, baseFmt);
        }

        template <class T> void appendFormatted(T data, const char *fmt)
        {
            constexpr size_t buffSize = 100;
            char buffer[buffSize];
            auto size = assertFormatRes(std::snprintf(buffer, buffSize, fmt, data), fmt);
            if (size < buffSize)
            {
                result += std::string_view(buffer, size);
            }
            else
            {
                std::vector<char> largeBuff(size + 1);
                assertFormatRes(std::sprintf(largeBuff.data(), fmt, data), fmt);
                result += std::string_view(largeBuff.data(), size);
            }
        }

        static std::string makeArgFmt(std::string_view coreFmt, std::string_view baseFmt);

        static int assertFormatRes(int result, const char *fmt);
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Helpers/Impl/Formatter.hpp"
#endif
