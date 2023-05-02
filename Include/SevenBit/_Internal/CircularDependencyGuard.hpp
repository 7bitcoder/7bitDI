#pragma once
#include <unordered_set>

#include "SevenBit/_Internal/Exceptions.hpp"
#include "SevenBit/_Internal/ScopedGuard.hpp"
#include "SevenBit/_Internal/TypeId.hpp"

namespace sb
{
    class CircularDependencyGuard
    {
      private:
        std::unordered_set<TypeId> _typeIdsUnderConstruction;

      public:
        ScopedGuard spawnGuard(TypeId typeIdUnderConstruction);
    };
} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/CircularDependencyGuard.hpp"
#endif