#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/ServiceDescriptorList.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details
{
    class EXPORT ServiceDescriptorsMap
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

      private:
        void checkIfAlreadyRegistered(ServiceDescriptor &descriptor);
    };
} // namespace sb::di::details

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/DI/Details/Impl/ServiceDescriptorsMap.hpp"
#endif