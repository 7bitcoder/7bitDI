#pragma once

#include <memory>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details
{
    template <class, class> struct IsCopyCtor : std::false_type
    {
    };
    template <class T> struct IsCopyCtor<T, T> : std::true_type
    {
    };
    template <class T> struct IsCopyCtor<T, const T> : std::true_type
    {
    };
    template <class T, class U> inline static constexpr bool IsCopyCtorV = IsCopyCtor<T, U>::value;

    template <class T> struct IsUniquePtr : std::false_type
    {
    };
    template <class T> struct IsUniquePtr<std::unique_ptr<T>> : std::true_type
    {
    };
    template <class T> inline constexpr bool IsUniquePtrV = IsUniquePtr<T>::value;

    template <class T> struct IsInitializerList : std::false_type
    {
    };
    template <class T> struct IsInitializerList<std::initializer_list<T>> : std::true_type
    {
    };
    template <class T> inline constexpr bool IsInitializerListV = IsInitializerList<T>::value;

    template <class T> struct RemoveUniquePtr
    {
        using Type = T;
    };
    template <class T> struct RemoveUniquePtr<std::unique_ptr<T>>
    {
        using Type = T;
    };
    template <class T> using RemoveUniquePtrT = typename RemoveUniquePtr<T>::Type;

    template <class... T> inline constexpr bool notSupportedType = false;

    template <class T, class I> inline constexpr bool InheritanceV = std::is_same_v<T, I> || std::is_base_of_v<T, I>;

    template <class T>
    inline constexpr bool IsServiceTypeV =
        !std::is_pointer_v<T> && !std::is_reference_v<T> &&
        (std::is_arithmetic_v<T> || std::is_array_v<T> || std::is_enum_v<T> || std::is_class_v<T>);

    template <class T>
    inline constexpr bool IsInPlaceServiceV =
        IsServiceTypeV<T> && (std::is_move_constructible_v<T> || std::is_copy_constructible_v<T>);

} // namespace sb::di::details
