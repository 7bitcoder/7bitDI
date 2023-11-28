#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Exceptions.hpp"

namespace sb::di::details::utils
{
    template <class... T> inline constexpr bool notSupportedType = false;

    template <class T> struct IsUniquePtr : std::false_type
    {
        using Type = T;
    };

    template <class T> struct IsUniquePtr<std::unique_ptr<T>> : std::true_type
    {
        using Type = T;
    };

    template <class T> bool IsUniquePtrV = IsUniquePtr<T>::value;

    template <class TService, class TImplementation> static void inheritCheck()
    {
        static_assert(std::is_base_of_v<TService, TImplementation>, "Type TImplementation must inherit from TService");
    }

    template <class ForwardIt, class UnaryPredicate>
    static ForwardIt removeIf(ForwardIt first, ForwardIt last, UnaryPredicate p)
    {
        first = std::find_if(first, last, p);
        if (first != last)
            for (ForwardIt i = first; ++i != last;)
                if (!p(*i))
                    *first++ = std::move(*i);
        return first;
    }

    template <class T>
    static std::unique_ptr<T> assertPtrAndMove(std::unique_ptr<T> ptr, std::string_view failMessage = "")
    {
        assertPtr(ptr.get(), failMessage);
        return ptr;
    }

    template <class T>
    static std::shared_ptr<T> assertPtrAndMove(std::shared_ptr<T> ptr, std::string_view failMessage = "")
    {
        assertPt(ptr.get(), failMessage);
        return ptr;
    }

    template <class T> static T *assertPtrAndMove(T *ptr, std::string_view failMessage = "")
    {
        assertPtr(ptr, failMessage);
        return ptr;
    }

    template <class T> static void assertPtr(const std::unique_ptr<T> &ptr, std::string_view failMessage = "")
    {
        assertPtr(ptr.get(), failMessage);
    }

    template <class T> static void assertPtr(const std::shared_ptr<T> &ptr, std::string_view failMessage = "")
    {
        assertPtr(ptr.get(), failMessage);
    }

    template <class T> static void assertPtr(const T *ptr, std::string_view failMessage = "")
    {
        if (!ptr)
        {
            auto message = !failMessage.empty()
                               ? std::string{failMessage}
                               : std::string{"Object of type: '"} + typeid(T).name() + "' cannot be null";
            throw NullPointnerException(message);
        }
    }
} // namespace sb::di::details::utils
