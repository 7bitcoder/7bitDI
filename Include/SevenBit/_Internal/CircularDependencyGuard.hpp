#pragma once
#include <unordered_set>

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/Exceptions.hpp"
#include "SevenBit/TypeId.hpp"
#include "SevenBit/_Internal/ScopedGuard.hpp"

namespace sb::internal
{
    class CircularDependencyGuard
    {
      private:
        std::unordered_set<TypeId> _typeIdsUnderConstruction;

      public:
        ScopedGuard spawnGuard(TypeId typeIdUnderConstruction);
    };
} // namespace sb::internal

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/CircularDependencyGuard.hpp"
#endif