#pragma once

#include <unordered_set>

#include <SevenBit/DI/LibraryConfig.hpp>

#include <SevenBit/DI/Details/Helpers/ScopedGuard.hpp>
#include <SevenBit/DI/Exceptions.hpp>

namespace sb::di::details
{
    INLINE ScopedGuard::ScopedGuard(const TypeId typeIdUnderConstruction,
                                    std::unordered_set<TypeId> &typeIdsUnderConstruction)
        : _typeIdsUnderConstruction(typeIdsUnderConstruction), _typeIdUnderConstruction(typeIdUnderConstruction)
    {
        if (_typeIdsUnderConstruction.count(_typeIdUnderConstruction))
        {
            throw CircularDependencyException{_typeIdUnderConstruction};
        }
        _typeIdsUnderConstruction.insert(_typeIdUnderConstruction);
    }

    INLINE ScopedGuard::~ScopedGuard() { _typeIdsUnderConstruction.erase(_typeIdUnderConstruction); }

} // namespace sb::di::details
