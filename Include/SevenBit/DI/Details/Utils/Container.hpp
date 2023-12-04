#pragma once

#include <algorithm>

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di::details::utils
{
    struct Container
    {
        template <class ForwardIt, class UnaryPredicate>
        static ForwardIt removeIf(ForwardIt first, ForwardIt last, UnaryPredicate p)
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
    };
} // namespace sb::di::details::utils
