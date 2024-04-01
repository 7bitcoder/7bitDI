#pragma once

#include <memory>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details
{
    template <class T> struct IsUniquePtr : std::false_type
    {
    };

    template <class T> struct IsUniquePtr<std::unique_ptr<T>> : std::true_type
    {
    };

    template <class T> struct IsUniquePtr<const std::unique_ptr<T>> : std::true_type
    {
    };

    template <class T> struct IsUniquePtr<const std::unique_ptr<T> &> : std::true_type
    {
    };

    template <class T> inline constexpr bool IsUniquePtrV = IsUniquePtr<T>::value;

    template <class T> struct IsVector : std::false_type
    {
    };

    template <class T> struct IsVector<std::vector<T>> : std::true_type
    {
    };

    template <class T> struct IsVector<const std::vector<T>> : std::true_type
    {
    };

    template <class T> struct IsVector<const std::vector<T> &> : std::true_type
    {
    };

    template <class T> inline constexpr bool IsVectorV = IsVector<T>::value;

    template <class T> struct isInitializerList : std::false_type
    {
    };

    template <class T> struct isInitializerList<std::initializer_list<T>> : std::true_type
    {
    };

    template <class T> struct isInitializerList<const std::initializer_list<T>> : std::true_type
    {
    };

    template <class T> struct isInitializerList<const std::initializer_list<T> &> : std::true_type
    {
    };

    template <class T> inline constexpr bool isInitializerListV = isInitializerList<T>::value;

} // namespace sb::di::details
