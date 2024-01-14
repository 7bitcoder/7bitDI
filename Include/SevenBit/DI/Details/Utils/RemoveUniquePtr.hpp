#pragma once

#include <memory>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details::utils
{
    template <class T> struct RemoveUniquePtr
    {
        using Type = T;
    };

    template <class T> struct RemoveUniquePtr<std::unique_ptr<T>>
    {
        using Type = T;
    };

    template <class T> using RemoveUniquePtrT = typename RemoveUniquePtr<T>::Type;
} // namespace sb::di::details::utils
