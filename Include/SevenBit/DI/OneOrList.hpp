#pragma once

#include <cstddef>
#include <variant>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/Meta.hpp"

namespace sb::di
{

    template <class T> class OneOrList
    {
        struct Uninitialized
        {
        };

        std::variant<Uninitialized, T, std::vector<T>> _variant{};

      public:
        OneOrList() = default;
        explicit OneOrList(const std::size_t size) : _variant(std::vector<T>{}) { reserve(size); }
        explicit OneOrList(T &&mainElement) : _variant(std::move(mainElement)) {}

        OneOrList(const OneOrList &) = delete;
        OneOrList(OneOrList &&other) noexcept = default;

        OneOrList &operator=(const OneOrList &) = delete;
        OneOrList &operator=(OneOrList &&other) noexcept = default;

        [[nodiscard]] bool isUninitialized() const { return std::holds_alternative<Uninitialized>(_variant); }
        [[nodiscard]] bool isSingle() const { return std::holds_alternative<T>(_variant); }
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

        explicit operator bool() const { return !isUninitialized(); }

        void add(T &&element)
        {
            if (isUninitialized())
            {
                _variant = std::move(element);
            }
            else
            {
                tryConvertToList();
                getAsList().emplace_back(std::move(element));
            }
        }

        void add(OneOrList &&other)
        {
            if (auto list = other.tryGetAsList())
            {
                tryConvertToList();
                auto &thisList = getAsList();
                thisList.reserve(thisList.size() + list->size());
                for (auto &element : *list)
                {
                    thisList.emplace_back(std::move(element));
                }
            }
            else if (auto single = other.tryGetAsSingle())
            {
                add(std::move(*single));
            }
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

        T &operator[](std::size_t index)
        {
            auto single = tryGetAsSingle();
            return single ? *single : getAsList()[index];
        }
        const T &operator[](std::size_t index) const
        {
            auto single = tryGetAsSingle();
            return single ? *single : getAsList()[index];
        }

        T &at(std::size_t index)
        {
            auto single = tryGetAsSingle();
            return single ? *single : getAsList().at(index);
        }
        const T &at(std::size_t index) const
        {
            auto single = tryGetAsSingle();
            return single ? *single : getAsList().at(index);
        }

        [[nodiscard]] std::size_t size() const
        {
            if (isSingle())
            {
                return 1;
            }
            auto list = tryGetAsList();
            return list ? list->size() : 0;
        }

        [[nodiscard]] bool empty() const { return !size(); }

        void reserve(std::size_t newCapacity)
        {
            if (newCapacity > 1)
            {
                tryConvertToList();
                getAsList().reserve(newCapacity);
            }
        }

        void shrink()
        {
            if (auto list = tryGetAsList())
            {
                list->shrink_to_fit();
            }
        }

        template <class TFunc> void forEach(TFunc fcn)
        {
            if (auto single = tryGetAsSingle())
            {
                callFcn(fcn, *single, 0);
            }
            else if (auto list = tryGetAsList())
            {
                std::size_t index = 0;
                for (auto &instance : *list)
                {
                    callFcn(fcn, instance, index++);
                }
            }
        }

        template <class TFunc> void forEach(TFunc fcn) const
        {
            if (const auto single = tryGetAsSingle())
            {
                callFcn(fcn, *single, 0);
            }
            else if (auto list = tryGetAsList())
            {
                std::size_t index = 0;
                for (const auto &instance : *list)
                {
                    callFcn(fcn, instance, index++);
                }
            }
        }

        template <class TFunc> auto map(TFunc mapFcn)
        {
            std::vector<decltype(callFcn(mapFcn, first(), 0))> result;
            result.reserve(size());
            forEach([&](T &item, std::size_t index) { result.push_back(callFcn(mapFcn, item, index)); });
            return result;
        }

        template <class TFunc> auto map(TFunc mapFcn) const
        {
            std::vector<decltype(callFcn(mapFcn, first(), 0))> result;
            result.reserve(size());
            forEach([&](const T &item, std::size_t index) { result.push_back(callFcn(mapFcn, item, index)); });
            return result;
        }

      private:
        void tryConvertToList()
        {
            if (!isList())
            {
                if (auto single = tryGetAsSingle())
                {
                    std::vector<T> vec;
                    vec.emplace_back(std::move(*single));
                    _variant = std::move(vec);
                }
                else
                {
                    _variant = std::vector<T>{};
                }
            }
        }

        template <class TFunc, class TItem> static auto callFcn(TFunc &fcn, TItem &item, std::size_t index)
        {
            if constexpr (std::is_invocable_v<TFunc, TItem &>)
            {
                return fcn(item);
            }
            else if constexpr (std::is_invocable_v<TFunc, TItem &, std::size_t>)
            {
                return fcn(item, index);
            }
            else
            {
                badFunctor<TFunc>();
            }
        }

        template <class TFunc> static void badFunctor()
        {
            static_assert(details::notSupportedType<TFunc>,
                          "Functor should take as arguments: T and additionally index");
        }
    };

} // namespace sb::di
