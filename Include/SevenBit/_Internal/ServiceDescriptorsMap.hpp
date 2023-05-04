#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "SevenBit/Exceptions.hpp"
#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/ServiceDescriptor.hpp"
#include "SevenBit/ServiceLifeTime.hpp"
#include "SevenBit/TypeId.hpp"
#include "SevenBit/_Internal/ServiceDescriptorList.hpp"

namespace sb::internal
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
        }

        ServiceDescriptorsMap &operator=(const ServiceDescriptorsMap &) = delete;
        ServiceDescriptorsMap &operator=(ServiceDescriptorsMap &&) = default;

        void add(ServiceDescriptor descriptor);

        void seal();

        const ServiceDescriptorList *getDescriptorsList(TypeId typeId) const;

        const ServiceDescriptor *getDescriptor(TypeId typeId) const;
    };
} // namespace sb::internal

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/ServiceDescriptorsMap.hpp"
#endif