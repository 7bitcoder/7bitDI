#pragma once

#include <cstdio>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/Formatter.hpp"

namespace sb::di::details
{
    namespace FormatterInternal
    {
        int assertFormatRes(int result, const char *fmt)
        {
            if (result < 0)
            {
                throw std::runtime_error(std::string{"Format string error: "} + fmt);
            }
            return result;
        }

        template <class T> void appendFormatted(std::string &result, T data, const char *fmt)
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
                assertFormatRes(std::snprintf(largeBuff.data(), size + 1, fmt, data), fmt);
                result += std::string_view(largeBuff.data(), size);
            }
        }

        std::string makeArgFmt(std::string_view coreFmt, std::string_view baseFmt)
        {
            std::string fmt = "%";
            fmt += coreFmt;
            // base fmt contains % at the beginning
            fmt += baseFmt.substr(1);
            return fmt;
        }

        template <class N>
        void appendNum(std::string &result, N arg, const std::string_view coreFmt, const char *baseFmt)
        {
            if (!coreFmt.empty())
            {
                const auto format = makeArgFmt(coreFmt, baseFmt);
                return appendFormatted(result, arg, format.c_str());
            }
            return appendFormatted(result, arg, baseFmt);
        }
    } // namespace FormatterInternal

    INLINE Formatter::Formatter(const std::string_view formatString) : formatString(formatString)
    {
        result.reserve(formatString.size());
    }

    INLINE std::string Formatter::getResult() && { return std::move(result); }

    INLINE void Formatter::append(const char *arg, std::string_view fmt)
    {
        if (!fmt.empty())
        {
            const auto format = FormatterInternal::makeArgFmt(fmt, "%s");
            return FormatterInternal::appendFormatted(result, arg, format.c_str());
        }
        result += arg;
    }

    INLINE void Formatter::append(const std::string &arg, std::string_view fmt)
    {
        if (!fmt.empty())
        {
            return append(arg.c_str(), fmt);
        }
        result += arg;
    }

    INLINE void Formatter::append(std::string_view arg, std::string_view fmt)
    {
        if (!fmt.empty())
        {
            return append(std::string{arg}, fmt);
        }
        result += arg;
    }

    INLINE void Formatter::append(int arg, std::string_view fmt)
    {
        return FormatterInternal::appendNum(result, arg, fmt, "%d");
    }
    INLINE void Formatter::append(long arg, std::string_view fmt)
    {
        return FormatterInternal::appendNum(result, arg, fmt, "%ld");
    }
    INLINE void Formatter::append(long long arg, std::string_view fmt)
    {
        return FormatterInternal::appendNum(result, arg, fmt, "%lld");
    }
    INLINE void Formatter::append(unsigned arg, std::string_view fmt)
    {
        return FormatterInternal::appendNum(result, arg, fmt, "%u");
    }
    INLINE void Formatter::append(unsigned long arg, std::string_view fmt)
    {
        return FormatterInternal::appendNum(result, arg, fmt, "%lu");
    }
    INLINE void Formatter::append(unsigned long long arg, std::string_view fmt)
    {
        return FormatterInternal::appendNum(result, arg, fmt, "%llu");
    }
    INLINE void Formatter::append(float arg, std::string_view fmt)
    {
        return FormatterInternal::appendNum(result, arg, fmt, "%f");
    }
    INLINE void Formatter::append(double arg, std::string_view fmt)
    {
        return FormatterInternal::appendNum(result, arg, fmt, "%f");
    }
    INLINE void Formatter::append(long double arg, std::string_view fmt)
    {
        return FormatterInternal::appendNum(result, arg, fmt, "%Lf");
    }
} // namespace sb::di::details
