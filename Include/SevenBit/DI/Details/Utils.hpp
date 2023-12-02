#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"

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
    ForwardIt removeIf(ForwardIt first, ForwardIt last, UnaryPredicate p)
    {
        first = std::find_if(first, last, p);
        if (first != last)
        {
            for (ForwardIt i = first; ++i != last;)
            {
                if (!p(*i))
                {
                    *first++ = std::move(*i);
                }
            }
        }
        return first;
    }

    struct EXPORT Assert
    {
        template <class T>
        static std::unique_ptr<T> ptrAndGet(std::unique_ptr<T> ptr, std::string_view failMessage = "")
        {
            Assert::ptr(ptr.get(), failMessage);
            return std::move(ptr);
        }

        template <class T>
        static std::shared_ptr<T> ptrAndGet(std::shared_ptr<T> ptr, std::string_view failMessage = "")
        {
            Assert::ptr(ptr.get(), failMessage);
            return std::move(ptr);
        }

        template <class T>
        static std::unique_ptr<T> &&ptrAndMove(std::unique_ptr<T> &&ptr, std::string_view failMessage = "")
        {
            Assert::ptr(ptr.get(), failMessage);
            return std::move(ptr);
        }

        template <class T>
        static std::shared_ptr<T> &&ptrAndMove(std::shared_ptr<T> &&ptr, std::string_view failMessage = "")
        {
            Assert::ptr(ptr.get(), failMessage);
            return std::move(ptr);
        }

        template <class T> static T *ptrAndGet(T *ptr, std::string_view failMessage = "")
        {
            Assert::ptr(ptr, failMessage);
            return ptr;
        }

        template <class T> static void ptr(const std::unique_ptr<T> &ptr, std::string_view failMessage = "")
        {
            Assert::ptr(ptr.get(), failMessage);
        }

        template <class T> static void ptr(const std::shared_ptr<T> &ptr, std::string_view failMessage = "")
        {
            Assert::ptr(ptr.get(), failMessage);
        }

        template <class T> static void ptr(const T *ptr, std::string_view failMessage = "")
        {
            if (!ptr)
            {
                auto message = !failMessage.empty()
                                   ? std::string{failMessage}
                                   : std::string{"Object of type: '"} + typeid(T).name() + "' cannot be null";
                throw NullPointnerException(message);
            }
        }

        static IServiceInstance::Ptr &&serviceAndMove(IServiceInstance::Ptr &&service);

        static IServiceInstance::Ptr serviceAndGet(IServiceInstance::Ptr service);

        static IServiceInstance *serviceAndGet(IServiceInstance *service);

        static void service(const IServiceInstance *service);

        static void service(const IServiceInstance::Ptr &service);
    };
} // namespace sb::di::details::utils

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Impl/Utils.hpp"
#endif
