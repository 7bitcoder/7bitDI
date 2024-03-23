#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceId.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstanceList.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details
{
    class EXPORT ServiceInstancesMap
    {
        std::unordered_map<ServiceId, ServiceInstanceList> _serviceListMap;
        std::vector<ServiceId> _constructionOrder;
        const bool _strongDestructionOrder = false;

      public:
        using Ptr = std::unique_ptr<ServiceInstancesMap>;

        explicit ServiceInstancesMap(bool strongDestructionOrder = false);

        ServiceInstancesMap(const ServiceInstancesMap &) = delete;
        ServiceInstancesMap(ServiceInstancesMap &&) noexcept = default;

        ServiceInstancesMap &operator=(const ServiceInstancesMap &) = delete;

        ServiceInstanceList &insert(ServiceId &&serviceId, ServiceInstance &&instance);

        ServiceInstanceList &insert(ServiceId &&serviceId, ServiceInstanceList &&instances);

        [[nodiscard]] bool contains(const ServiceId &serviceId) const;

        ServiceInstanceList *findInstances(const ServiceId &serviceId);

        [[nodiscard]] bool empty() const;

        void clear();

        ~ServiceInstancesMap();
    };

} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Containers/Impl/ServiceInstancesMap.hpp"
#endif
