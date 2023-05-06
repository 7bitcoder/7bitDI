#pragma once

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/Details/CircularDependencyGuard.hpp"

namespace sb::details
{
    INLINE ScopedGuard CircularDependencyGuard::spawnGuard(TypeId typeIdUnderConstruction)
    {
        return ScopedGuard{typeIdUnderConstruction, _typeIdsUnderConstruction};
    }

    INLINE ScopedGuard CircularDependencyGuard::operator()(TypeId typeIdUnderConstruction)
    {
        return spawnGuard(typeIdUnderConstruction);
    }

} // namespace sb::details