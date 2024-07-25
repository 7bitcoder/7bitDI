#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/String.hpp"

namespace sb::di::details
{
    INLINE std::string String::toString(const char *arg, std::string_view fmt)
    {
        if (!fmt.empty())
        {
            auto format = makeArgFmt(fmt, "s");
            return dataToString(format.data(), const_cast<char *>(arg));
        }
        return std::string(arg);
    }

    INLINE std::string String::toString(const std::string &arg, std::string_view fmt)
    {
        if (!fmt.empty())
        {
            auto format = makeArgFmt(fmt, "s");
            return dataToString(format.data(), const_cast<std::string &>(arg).data());
        }
        return arg;
    }

    INLINE std::string String::toString(std::string &&arg, std::string_view fmt)
    {
        if (!fmt.empty())
        {
            auto format = makeArgFmt(fmt, "s");
            return dataToString(format.data(), arg.data());
        }
        return std::move(arg);
    }

    INLINE std::string String::toString(std::string_view arg, std::string_view fmt)
    {
        std::string strArg{arg};
        if (!fmt.empty())
        {
            auto format = makeArgFmt(fmt, "s");
            return dataToString(format.data(), strArg.data());
        }
        return strArg;
    }

    INLINE std::string String::toString(int arg, std::string_view fmt) { return toString(arg, fmt, "d"); }
    INLINE std::string String::toString(long arg, std::string_view fmt) { return toString(arg, fmt, "ld"); }
    INLINE std::string String::toString(long long arg, std::string_view fmt) { return toString(arg, fmt, "lld"); }
    INLINE std::string String::toString(unsigned arg, std::string_view fmt) { return toString(arg, fmt, "u"); }
    INLINE std::string String::toString(unsigned long arg, std::string_view fmt) { return toString(arg, fmt, "lu"); }
    INLINE std::string String::toString(unsigned long long arg, std::string_view fmt)
    {
        return toString(arg, fmt, "llu");
    }
    INLINE std::string String::toString(float arg, std::string_view fmt) { return toString(arg, fmt, "f"); }
    INLINE std::string String::toString(double arg, std::string_view fmt) { return toString(arg, fmt, "f"); }
    INLINE std::string String::toString(long double arg, std::string_view fmt) { return toString(arg, fmt, "Lf"); }

    INLINE std::string String::makeArgFmt(std::string_view fmt, std::string_view type)
    {
        return "%" + std::string{fmt} + std::string{type};
    }
} // namespace sb::di::details
