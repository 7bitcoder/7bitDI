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
        std::vector<IServiceInstance::Ptr *> _constructionOrder;
        bool _strongDestructionOrder = false;

      public:
        using Ptr = std::unique_ptr<ServicesMap>;

        ServicesMap(bool strongDestructionOrder);

        ServiceList &add(TypeId serviceTypeId, IServiceInstance::Ptr service);

        ServiceList &operator[](TypeId serviceTypeId);

        ServiceList *getList(TypeId serviceTypeId);

        const ServiceList *getList(TypeId serviceTypeId) const;

        ~ServicesMap();
    };

} // namespace sb::internal

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/ServicesMap.hpp"
#endif