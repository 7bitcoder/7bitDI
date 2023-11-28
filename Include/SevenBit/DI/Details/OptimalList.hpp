#pragma once

#include <variant>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details
{
    template <class T> class EXPORT OptimalList
    {
      private:
        std::variant<T, std::vector<T>> _variant;

      public:
        OptimalList(size_t size) : _variant(std::vector<T>{}) { getAsList().reserve(size); }
        OptimalList(T mainElement) : _variant(mainElement) {}
        OptimalList(T &&mainElement) : _variant(std::move(mainElement)) {}

        OptimalList(const OptimalList &) = delete;
        OptimalList(OptimalList &&) = default;

        OptimalList &operator=(OptimalList &&) = default;
        OptimalList &operator=(const OptimalList &) = delete;

        T &first() { return hasOne() ? getAsOne() : getAsList().front(); }

        T &last() { return hasOne() ? getAsOne() : getAsList().back(); }

        bool hasMany() { return std::holds_alternative<std::vector<T>>(_variant); }

        bool hasOne() { return std::holds_alternative<T>(_variant); }

        std::vector<T> &getAsList() { return std::get<std::vector<T>>(_variant); }

        T &getAsOne() { return std::get<T>(_variant); }

        void add(T element)
        {
            if (hasOne())
            {
                std::vector<T> vec;
                vec.emplace_back(std::move(getAsOne()));
                _variant = std::move(vec);
            }
            getAsList().emplace_back(std::move(element));
        }

        size_t size() { hasMany() ? getAsList().size() : 1; }

        void shrink()
        {
            if (hasMany())
            {
                getAsList().shrink_to_fit();
            }
        }
    };

} // namespace sb::di::details
