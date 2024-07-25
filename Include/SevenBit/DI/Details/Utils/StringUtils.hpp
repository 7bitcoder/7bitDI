#pragma once

#include <string>
#include <string_view>

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details
{
    struct EXPORT StringUtils
    {
        static std::string toString(const char *arg, std::string_view fmt = "");
        static std::string toString(const std::string &arg, std::string_view fmt = "");
        static std::string toString(std::string &&arg, std::string_view fmt = "");
        static std::string toString(std::string_view arg, std::string_view fmt = "");
        static std::string toString(int arg, std::string_view fmt = "");
        static std::string toString(long arg, std::string_view fmt = "");
        static std::string toString(long long arg, std::string_view fmt = "");
        static std::string toString(unsigned arg, std::string_view fmt = "");
        static std::string toString(unsigned long arg, std::string_view fmt = "");
        static std::string toString(unsigned long long arg, std::string_view fmt = "");
        static std::string toString(float arg, std::string_view fmt = "");
        static std::string toString(double arg, std::string_view fmt = "");
        static std::string toString(long double arg, std::string_view fmt = "");

      private:
        template <class T> static std::string dataToString(const char *const fmt, T data)
        {
            char buffer[200];
            auto size = std::snprintf(buffer, sizeof(buffer), fmt, data);
            return std::string(buffer, size);
        }

        template <class Number> static std::string toString(Number arg, const std::string_view fmt, const std::string_view type)
        {
            if (!fmt.empty())
            {
                const auto format = makeArgFmt(fmt, type);
                return dataToString(format.c_str(), arg);
            }
            return std::to_string(arg);
        }

        static std::string makeArgFmt(std::string_view fmt, std::string_view type);
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Utils/Impl/StringUtils.hpp"
#endif
