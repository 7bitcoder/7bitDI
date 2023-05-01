#pragma once

#include <memory>
#include <unordered_map>

#include "SevenBit/ServiceProvider.hpp"
#include "SevenBit/_Internal/IServiceCreatorsProvider.hpp"
#include "SevenBit/_Internal/ServiceConstructor.hpp"
#include "SevenBit/_Internal/ServiceCreators.hpp"
#include "SevenBit/_Internal/ServiceDescriptor.hpp"
#include "SevenBit/_Internal/ServiceDescriptorList.hpp"
#include "SevenBit/_Internal/ServiceFactory.hpp"
#include "SevenBit/_Internal/ServiceLifeTime.hpp"
#include "SevenBit/_Internal/TypeId.hpp"

namespace sb
{
    class ServiceDescriptorsMap
    {
      private:
        std::unordered_map<TypeId, ServiceDescriptorList> _serviceCreatorsMap;

      public:
        using Ptr = std::unique_ptr<ServiceDescriptorsMap>;

        auto begin() const { return _serviceCreatorsMap.begin(); }
        auto end() const { return _serviceCreatorsMap.end(); }

        auto begin() { return _serviceCreatorsMap.begin(); }
        auto end() { return _serviceCreatorsMap.end(); }

        ServiceDescriptorsMap() = default;
        ServiceDescriptorsMap(const ServiceDescriptorsMap &) = delete;
        ServiceDescriptorsMap(ServiceDescriptorsMap &&) = default;
        template <class TDescriptorIt> ServiceDescriptorsMap(TDescriptorIt begin, TDescriptorIt end)
        {
            for (auto it = begin; it != end; ++it)
            {
                add(*it);
            }
            seal();
        }

        ServiceDescriptorsMap &operator=(const ServiceDescriptorsMap &) = delete;
        ServiceDescriptorsMap &operator=(ServiceDescriptorsMap &&) = default;

        void add(ServiceDescriptor descriptor)
        {
            _serviceCreatorsMap[descriptor.getServiceTypeId()].add(std::move(descriptor));
        }

        void seal()
        {
            for (auto &list : *this)
            {
                list.second.seal();
            }
        }

        const ServiceDescriptorList *getDescriptorsList(TypeId typeId) const
        {
            if (auto it = _serviceCreatorsMap.find(typeId); it != end())
            {
                return &it->second;
            }
            return nullptr;
        }

        const ServiceDescriptor *getDescriptor(TypeId typeId) const
        {
            if (auto descriptor = getDescriptorsList(typeId))
            {
                return &descriptor->last();
            }
            return nullptr;
        }
    };
} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/ServiceCollection.hpp"
#endif