#pragma once

#include <memory>
#include <type_traits>

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details::utils
{
    template <class T> using RemovePtr = std::remove_pointer<T>;

    template <class T> using RemovePtrT = RemovePtr<T>::type;
} // namespace sb::di::details::utils
