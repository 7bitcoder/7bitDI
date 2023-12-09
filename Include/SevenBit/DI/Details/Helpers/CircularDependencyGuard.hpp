#pragma once

#include <unordered_set>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/ScopedGuard.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details::helpers
{
    class EXPORT CircularDependencyGuard
    {
        std::unordered_set<TypeId> _typeIdsUnderConstruction;

      public:
        ScopedGuard spawnGuard(TypeId typeIdUnderConstruction);

        ScopedGuard operator()(TypeId typeIdUnderConstruction);
    };
} // namespace sb::di::details::helpers

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Helpers/Impl/CircularDependencyGuard.hpp"
#endif
