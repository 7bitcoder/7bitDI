#pragma once

#include <memory>
#include <unordered_map>

#include "SevenBit/_Internal/Exceptions.hpp"
#include "SevenBit/_Internal/IServiceHolder.hpp"
#include "SevenBit/_Internal/IServiceInstance.hpp"
#include "SevenBit/_Internal/ServiceList.hpp"
#include "SevenBit/_Internal/TypeId.hpp"

namespace sb
{
    class ServicesMap
    {
      private:
        std::unordered_map<TypeId, ServiceList> _serviceListMap;

      public:
        using Ptr = std::unique_ptr<ServicesMap>;

        ServiceList &add(TypeId serviceTypeId, IServiceInstance::Ptr service)
        {
            return _serviceListMap[serviceTypeId].add(std::move(service));
        }

        ServiceList &operator[](TypeId serviceTypeId) { return _serviceListMap[serviceTypeId]; }

        ServiceList *getList(TypeId serviceTypeId)
        {
            auto it = _serviceListMap.find(serviceTypeId);
            return it != _serviceListMap.end() ? &it->second : nullptr;
        }

        const ServiceList *getList(TypeId serviceTypeId) const
        {
            auto it = _serviceListMap.find(serviceTypeId);
            return it != _serviceListMap.end() ? &it->second : nullptr;
        }
    };

} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/ServicesContainer.hpp"
#endif