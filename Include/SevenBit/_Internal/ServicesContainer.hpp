#pragma once

#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "SevenBit/_Internal/IServiceHolder.hpp"
#include "SevenBit/_Internal/ServiceList.hpp"

namespace sb
{
    class ServiceContainer
    {
      private:
        std::unordered_map<TypeId, ServiceList> _serviceListMap;

      public:
        using Ptr = std::unique_ptr<ServiceContainer>;

        ServiceList *addAndGetList(IServiceHolder::Ptr holder)
        {
            if (!holder)
            {
                throw ServiceHolderInvalidException{};
            }
            auto interfaceTypeId = holder->getServiceInterfaceTypeId();
            auto &serviceList = _serviceListMap[interfaceTypeId];
            serviceList.add(std::move(holder));
            return &serviceList;
        }

        ServiceList *getList(TypeId interfaceType)
        {
            auto it = _serviceListMap.find(interfaceType);
            return it != _serviceListMap.end() ? &it->second : nullptr;
        }

        const ServiceList *getList(TypeId interfaceType) const
        {
            auto it = _serviceListMap.find(interfaceType);
            return it != _serviceListMap.end() ? &it->second : nullptr;
        }

        bool contains(TypeId interfaceType) const { return getList(interfaceType); }
    };

} // namespace sb