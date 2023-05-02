#pragma once

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/_Internal/CircularDependencyGuard.hpp"

namespace sb::internal
{
    INLINE ScopedGuard CircularDependencyGuard::spawnGuard(TypeId typeIdUnderConstruction)
    {
        return ScopedGuard{typeIdUnderConstruction, _typeIdsUnderConstruction};
    }
} // namespace sb::internal