#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Helpers/CircularDependencyGuard.hpp"

namespace sb::di::details::helpers
{
    INLINE ScopedGuard CircularDependencyGuard::spawnGuard(TypeId typeIdUnderConstruction)
    {
        return ScopedGuard{typeIdUnderConstruction, _typeIdsUnderConstruction};
    }

    INLINE ScopedGuard CircularDependencyGuard::operator()(TypeId typeIdUnderConstruction)
    {
        return spawnGuard(typeIdUnderConstruction);
    }

} // namespace sb::di::details::helpers
