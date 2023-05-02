#pragma once
#include <unordered_set>

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/Exceptions.hpp"
#include "SevenBit/TypeId.hpp"

namespace sb::internal
{
    class ScopedGuard
    {
      private:
        std::unordered_set<TypeId> &_typeIdsUnderConstruction;
        TypeId _typeIdUnderConstruction;

      public:
        ScopedGuard(TypeId typeIdUnderConstruction, std::unordered_set<TypeId> &typeIdsUnderConstruction);

        ~ScopedGuard();
    };

} // namespace sb::internal

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/ScopedGuard.hpp"
#endif