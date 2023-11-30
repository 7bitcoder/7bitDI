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
        explicit OptimalList(size_t size) : _variant(std::vector<T>{}) { reserve(size); }
        explicit OptimalList(T &&mainElement) : _variant(std::move(mainElement)) {}

        OptimalList(const OptimalList &) = delete;
        OptimalList(OptimalList &&) = default;

        OptimalList &operator=(const OptimalList &) = delete;
        OptimalList &operator=(OptimalList &&) = default;

        [[nodiscard]] bool isList() const { return std::holds_alternative<std::vector<T>>(_variant); }

        std::vector<T> &getAsList() { return std::get<std::vector<T>>(_variant); }
        const std::vector<T> &getAsList() const { return std::get<std::vector<T>>(_variant); }

        T &getAsSingle() { return std::get<T>(_variant); }
        const T &getAsSingle() const { return std::get<T>(_variant); }

        std::vector<T> *tryGetAsList() { return std::get_if<std::vector<T>>(&_variant); }
        const std::vector<T> *tryGetAsList() const { return std::get_if<std::vector<T>>(&_variant); }

        T *tryGetAsSingle() { return std::get_if<T>(&_variant); }
        const T *tryGetAsSingle() const { return std::get_if<T>(&_variant); }

        void add(T &&element)
        {
            tryConvertToList();
            getAsList().emplace_back(std::move(element));
        }

        T &first() { return (*this)[0]; }
        const T &first() const { return (*this)[0]; }

        T &last() { return (*this)[size() - 1]; }
        const T &last() const { return (*this)[size() - 1]; }

        T &operator[](size_t index)
        {
            if (auto single = tryGetAsSingle())
            {
                return *single;
            }
            return getAsList().at(index);
        }
        const T &operator[](size_t index) const
        {
            if (auto single = tryGetAsSingle())
            {
                return *single;
            }
            return getAsList().at(index);
        }

        [[nodiscard]] size_t size() const
        {
            if (auto list = tryGetAsList())
            {
                return list->size();
            }
            return 1;
        }

        [[nodiscard]] bool empty() const { return !size(); }

        void reserve(size_t newCapacity)
        {
            tryConvertToList();
            getAsList().reserve(newCapacity);
        }

        void resize(size_t size)
        {
            tryConvertToList();
            getAsList().resize(size);
        }

        void shrink()
        {
            if (auto list = tryGetAsList())
            {
                list->shrink_to_fit();
            }
        }

      private:
        void tryConvertToList()
        {
            if (auto single = tryGetAsSingle())
            {
                std::vector<T> vec;
                vec.emplace_back(std::move(*single));
                _variant = std::move(vec);
            }
        }
    };

} // namespace sb::di::details
