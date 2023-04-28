#pragma once
#include <unordered_set>

#include "SevenBit/LibraryConfig.hpp"
#include "SevenBit/_Internal/ScopedGuard.hpp"

namespace sb
{
    INLINE ScopedGuard::ScopedGuard(TypeId typeIdUnderConstruction,
                                    std::unordered_set<TypeId> &typeIdsUnderConstruction)
        : _typeIdUnderConstruction(typeIdUnderConstruction), _typeIdsUnderConstruction(typeIdsUnderConstruction)
    {
        if (_typeIdsUnderConstruction.contains(_typeIdUnderConstruction))
        {
            throw CircularDependencyException{_typeIdUnderConstruction};
        }
        _typeIdsUnderConstruction.insert(_typeIdUnderConstruction);
    }

    INLINE ScopedGuard::~ScopedGuard() { _typeIdsUnderConstruction.erase(_typeIdUnderConstruction); }

} // namespace sb