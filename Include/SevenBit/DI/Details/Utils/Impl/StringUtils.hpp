#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/StringUtils.hpp"

namespace sb::di::details
{
    INLINE std::string StringUtils::toString(const char *arg, std::string_view fmt)
    {
        if (!fmt.empty())
        {
            const auto format = makeArgFmt(fmt, "s");
            return dataToString(format.c_str(), const_cast<char *>(arg));
        }
        return std::string(arg);
    }

    INLINE std::string StringUtils::toString(const std::string &arg, std::string_view fmt)
    {
        if (!fmt.empty())
        {
            const auto format = makeArgFmt(fmt, "s");
            return dataToString(format.c_str(), const_cast<std::string &>(arg).c_str());
        }
        return arg;
    }

    INLINE std::string StringUtils::toString(std::string &&arg, std::string_view fmt)
    {
        if (!fmt.empty())
        {
            const auto format = makeArgFmt(fmt, "s");
            return dataToString(format.c_str(), arg.c_str());
        }
        return std::move(arg);
    }

    INLINE std::string StringUtils::toString(std::string_view arg, std::string_view fmt)
    {
        std::string strArg{arg};
        if (!fmt.empty())
        {
            const auto format = makeArgFmt(fmt, "s");
            return dataToString(format.c_str(), strArg.c_str());
        }
        return strArg;
    }

    INLINE std::string StringUtils::toString(int arg, std::string_view fmt) { return toString(arg, fmt, "d"); }
    INLINE std::string StringUtils::toString(long arg, std::string_view fmt) { return toString(arg, fmt, "ld"); }
    INLINE std::string StringUtils::toString(long long arg, std::string_view fmt) { return toString(arg, fmt, "lld"); }
    INLINE std::string StringUtils::toString(unsigned arg, std::string_view fmt) { return toString(arg, fmt, "u"); }
    INLINE std::string StringUtils::toString(unsigned long arg, std::string_view fmt) { return toString(arg, fmt, "lu"); }
    INLINE std::string StringUtils::toString(unsigned long long arg, std::string_view fmt)
    {
        return toString(arg, fmt, "llu");
    }
    INLINE std::string StringUtils::toString(float arg, std::string_view fmt) { return toString(arg, fmt, "f"); }
    INLINE std::string StringUtils::toString(double arg, std::string_view fmt) { return toString(arg, fmt, "f"); }
    INLINE std::string StringUtils::toString(long double arg, std::string_view fmt) { return toString(arg, fmt, "Lf"); }

    INLINE std::string StringUtils::makeArgFmt(std::string_view fmt, std::string_view type)
    {
        return "%" + std::string{fmt} + std::string{type};
    }
} // namespace sb::di::details
