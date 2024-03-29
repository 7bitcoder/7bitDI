#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceInstanceList.hpp"
#include "SevenBit/DI/Details/Models/ServiceId.hpp"

namespace sb::di::details
{
    class EXPORT ServiceInstancesMap
    {
        std::unordered_map<ServiceId, ServiceInstanceList> _instancesMap;
        std::vector<ServiceId> _constructionOrder;
        const bool _strongDestructionOrder = false;

      public:
        using Ptr = std::unique_ptr<ServiceInstancesMap>;

        explicit ServiceInstancesMap(bool strongDestructionOrder = false);

        ServiceInstancesMap(const ServiceInstancesMap &) = delete;
        ServiceInstancesMap(ServiceInstancesMap &&) noexcept = default;

        ServiceInstancesMap &operator=(const ServiceInstancesMap &) = delete;

        ServiceInstanceList &insert(const ServiceId &id, ServiceInstance &&instance);

        ServiceInstanceList &insert(const ServiceId &id, ServiceInstanceList &&instances);

        [[nodiscard]] bool contains(const ServiceId &id) const;

        ServiceInstanceList *findInstances(const ServiceId &id);

        [[nodiscard]] bool empty() const;

        void clear();

        ~ServiceInstancesMap();
    };

} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Containers/Impl/ServiceInstancesMap.hpp"
#endif
