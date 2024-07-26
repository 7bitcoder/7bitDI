#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/Formatter.hpp"

namespace sb::di::details
{
    INLINE Formatter::Formatter(const std::string_view formatString) : formatString(formatString)
    {
        result.reserve(formatString.size());
    }

    INLINE std::string Formatter::getResult() && { return std::move(result); }

    INLINE void Formatter::append(const char *arg, std::string_view fmt)
    {
        if (!fmt.empty())
        {
            const auto format = makeArgFmt(fmt, "%s");
            return appendFormatted(arg, format.c_str());
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

    INLINE void Formatter::append(int arg, std::string_view fmt) { return appendNum(arg, fmt, "%d"); }
    INLINE void Formatter::append(long arg, std::string_view fmt) { return appendNum(arg, fmt, "%ld"); }
    INLINE void Formatter::append(long long arg, std::string_view fmt) { return appendNum(arg, fmt, "%lld"); }
    INLINE void Formatter::append(unsigned arg, std::string_view fmt) { return appendNum(arg, fmt, "%u"); }
    INLINE void Formatter::append(unsigned long arg, std::string_view fmt) { return appendNum(arg, fmt, "%lu"); }
    INLINE void Formatter::append(unsigned long long arg, std::string_view fmt) { return appendNum(arg, fmt, "%llu"); }
    INLINE void Formatter::append(float arg, std::string_view fmt) { return appendNum(arg, fmt, "%f"); }
    INLINE void Formatter::append(double arg, std::string_view fmt) { return appendNum(arg, fmt, "%f"); }
    INLINE void Formatter::append(long double arg, std::string_view fmt) { return appendNum(arg, fmt, "%Lf"); }

    INLINE std::string Formatter::makeArgFmt(std::string_view coreFmt, std::string_view baseFmt)
    {
        std::string fmt = "%";
        fmt += coreFmt;
        // base fmt contains % at the beginning
        fmt += baseFmt.substr(1);
        return fmt;
    }

    INLINE int Formatter::assertFormatRes(int result, const char *fmt)
    {
        if (result < 0)
        {
            throw std::runtime_error(std::string{"Format string error: "} + fmt);
        }
        return result;
    }
} // namespace sb::di::details
