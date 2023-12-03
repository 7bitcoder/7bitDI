#pragma once

#include <variant>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di
{
    template <class T> class OneOrList
    {
      private:
        std::variant<T, std::vector<T>> _variant;

      public:
        explicit OneOrList(size_t size) : _variant(std::vector<T>{}) { reserve(size); }
        explicit OneOrList(T &&mainElement) : _variant(std::move(mainElement)) {}

        OneOrList(const OneOrList &) = delete;
        OneOrList(OneOrList &&other) noexcept = default;

        OneOrList &operator=(const OneOrList &) = delete;
        OneOrList &operator=(OneOrList &&other) noexcept = default;

        [[nodiscard]] bool isList() const { return std::holds_alternative<std::vector<T>>(_variant); }

        std::vector<T> &getAsList() { return std::get<std::vector<T>>(_variant); }
        const std::vector<T> &getAsList() const { return std::get<std::vector<T>>(_variant); }

        std::variant<T, std::vector<T>> &getVariant() { return _variant; }
        const std::variant<T, std::vector<T>> &getVariant() const { return _variant; }

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

        T &first()
        {
            auto single = tryGetAsSingle();
            return single ? *single : getAsList().front();
        }
        const T &first() const
        {
            auto single = tryGetAsSingle();
            return single ? *single : getAsList().front();
        }

        T &last()
        {
            auto single = tryGetAsSingle();
            return single ? *single : getAsList().back();
        }
        const T &last() const
        {
            auto single = tryGetAsSingle();
            return single ? *single : getAsList().back();
        }

        T &operator[](size_t index)
        {
            auto single = tryGetAsSingle();
            return single ? *single : getAsList().at(index);
        }
        const T &operator[](size_t index) const
        {
            auto single = tryGetAsSingle();
            return single ? *single : getAsList().at(index);
        }

        [[nodiscard]] size_t size() const
        {
            auto list = tryGetAsList();
            return list ? list->size() : 1;
        }

        [[nodiscard]] bool empty() const { return !size(); }

        void reserve(size_t newCapacity)
        {
            tryConvertToList();
            getAsList().reserve(newCapacity);
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

} // namespace sb::di
