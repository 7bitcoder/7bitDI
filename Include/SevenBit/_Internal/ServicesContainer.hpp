#pragma once

#include <memory>
#include <unordered_map>

#include "SevenBit/_Internal/IServiceHolder.hpp"
#include "SevenBit/_Internal/ServiceList.hpp"

namespace sb
{
    class ServicesContainer
    {
      private:
        std::unordered_map<TypeId, ServiceList> _serviceListMap;

      public:
        using Ptr = std::unique_ptr<ServicesContainer>;

        ServiceList *addAndGetList(IServiceHolder::Ptr holder);

        ServiceList *getList(TypeId interfaceType);

        const ServiceList *getList(TypeId interfaceType) const;

        bool contains(TypeId interfaceType) const;
    };

} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/ServicesContainer.hpp"
#endif