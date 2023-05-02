#pragma once

#include <memory>
#include <unordered_map>

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/Exceptions.hpp"
#include "SevenBit/IServiceInstance.hpp"
#include "SevenBit/TypeId.hpp"
#include "SevenBit/_Internal/ServiceList.hpp"

namespace sb::internal
{
    class ServicesMap
    {
      private:
        std::unordered_map<TypeId, ServiceList> _serviceListMap;

      public:
        using Ptr = std::unique_ptr<ServicesMap>;

        ServiceList &add(TypeId serviceTypeId, IServiceInstance::Ptr service);

        ServiceList &operator[](TypeId serviceTypeId);

        ServiceList *getList(TypeId serviceTypeId);

        const ServiceList *getList(TypeId serviceTypeId) const;
    };

} // namespace sb::internal

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/ServicesMap.hpp"
#endif