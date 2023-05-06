#pragma once
#include <unordered_set>

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/Details/ScopedGuard.hpp"
#include "SevenBit/TypeId.hpp"

namespace sb::details
{
    class CircularDependencyGuard
    {
      private:
        std::unordered_set<TypeId> _typeIdsUnderConstruction;

      public:
        ScopedGuard spawnGuard(TypeId typeIdUnderConstruction);

        ScopedGuard operator()(TypeId typeIdUnderConstruction);
    };
} // namespace sb::details

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/Details/Impl/CircularDependencyGuard.hpp"
#endif