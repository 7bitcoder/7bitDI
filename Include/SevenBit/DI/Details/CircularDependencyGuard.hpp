#pragma once
#include <unordered_set>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/ScopedGuard.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details
{
    class EXPORT CircularDependencyGuard
    {
      private:
        std::unordered_set<TypeId> _typeIdsUnderConstruction;

      public:
        ScopedGuard spawnGuard(TypeId typeIdUnderConstruction);

        ScopedGuard operator()(TypeId typeIdUnderConstruction);
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Impl/CircularDependencyGuard.hpp"
#endif