#pragma once

#include <algorithm>
#include <charconv>
#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <typeinfo>
#include <vector>

#include "SevenBit/_Internal/Exceptions.hpp"

namespace sb::utils
{
    template <class T> T &getRequired(std::unique_ptr<T> &ptr)
    {
        if (ptr)
        {
            return *ptr;
        }
        throw NullReferenceException{};
    }

    // helper type for the visitor #4
    template <class... Ts> struct overloaded : Ts...
    {
        using Ts::operator()...;
    };
    // explicit deduction guide (not needed as of C++20)
    template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

    inline std::vector<std::string_view> split(std::string_view str, const char delim = '/')
    {
        std::vector<std::string_view> result;

        int indexCommaToLeftOfColumn = 0;
        int indexCommaToRightOfColumn = -1;

        for (int i = 0; i < static_cast<int>(str.size()); i++)
        {
            if (str[i] == delim)
            {
                indexCommaToLeftOfColumn = indexCommaToRightOfColumn;
                indexCommaToRightOfColumn = i;
                int index = indexCommaToLeftOfColumn + 1;
                int length = indexCommaToRightOfColumn - index;

                std::string_view column(str.data() + index, length);
                result.push_back(column);
            }
        }
        std::string_view finalColumn(str.data() + indexCommaToRightOfColumn + 1,
                                     str.size() - indexCommaToRightOfColumn - 1);
        result.push_back(finalColumn);
        return result;
    }

    template <class... T> inline constexpr bool notSupportedType = false;

    template <size_t N> struct StringLiteral
    {
        constexpr StringLiteral(const char (&str)[N])
        {
            for (size_t i = 0; i < N; ++i)
            {
                value[i] = str[i];
            }
        }

        char value[N] = {'\0'};
    };

    template <class T> T convert(const std::string &val)
    {
        static_assert(notSupportedType<T>, "This Type is not supported, please use int, string, bool");
    }

    template <> inline std::string convert(const std::string &val) { return val; }
    template <> inline int convert(const std::string &val) { return std::stoi(val); }
    template <> inline float convert(const std::string &val) { return std::stof(val); }
    template <> inline double convert(const std::string &val) { return std::stod(val); }
    template <> inline bool convert(const std::string &val)
    {
        auto copy = val;
        std::transform(copy.begin(), copy.end(), copy.begin(), [](unsigned char c) { return std::tolower(c); });
        if (copy == "true")
        {
            return true;
        }
        if (copy == "false")
        {
            return false;
        }
        try
        {
            return convert<int>(val) != 0;
        }
        catch (...)
        {
        }
        throw std::bad_cast{};
    }

    template <class T> bool tryConvert(const std::string &val, T &result)
    {
        try
        {
            result = convert<T>(val);
            return true;
        }
        catch (...)
        {
        }
        return false;
    }

    template <typename T, typename Enable = void> struct IsOptional : std::false_type
    {
    };

    template <typename T> struct IsOptional<std::optional<T>> : std::true_type
    {
    };

    template <typename T> inline constexpr bool IsOptionalV = IsOptional<T>::value;

    template <typename T, typename Enable = void> struct IsSimpleType : std::false_type
    {
    };

    template <> struct IsSimpleType<int> : std::true_type
    {
    };
    template <> struct IsSimpleType<bool> : std::true_type
    {
    };
    template <> struct IsSimpleType<float> : std::true_type
    {
    };
    template <> struct IsSimpleType<double> : std::true_type
    {
    };
    template <> struct IsSimpleType<std::string> : std::true_type
    {
    };

    template <> struct IsSimpleType<std::optional<int>> : std::true_type
    {
    };
    template <> struct IsSimpleType<std::optional<bool>> : std::true_type
    {
    };
    template <> struct IsSimpleType<std::optional<float>> : std::true_type
    {
    };
    template <> struct IsSimpleType<std::optional<double>> : std::true_type
    {
    };
    template <> struct IsSimpleType<std::optional<std::string>> : std::true_type
    {
    };
    template <typename T> inline constexpr bool IsSimpleTypeV = IsSimpleType<T>::value;

} // namespace sb::utils