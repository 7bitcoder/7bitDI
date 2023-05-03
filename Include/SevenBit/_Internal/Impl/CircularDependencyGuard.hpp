#pragma once

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/_Internal/CircularDependencyGuard.hpp"

namespace sb::internal
{
    INLINE ScopedGuard CircularDependencyGuard::spawnGuard(TypeId typeIdUnderConstruction)
    {
        return ScopedGuard{typeIdUnderConstruction, _typeIdsUnderConstruction};
    }

    INLINE ScopedGuard CircularDependencyGuard::operator()(TypeId typeIdUnderConstruction)
    {
        return spawnGuard(typeIdUnderConstruction);
    }

} // namespace sb::internal