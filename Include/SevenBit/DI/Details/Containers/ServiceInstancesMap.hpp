#pragma once

#include <memory>
#include <unordered_map>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceInstanceList.hpp"
#include "SevenBit/DI/ServiceInstance.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details::containers
{
    class EXPORT ServiceInstancesMap
    {
        std::unordered_map<TypeId, ServiceInstanceList> _serviceListMap;
        std::vector<TypeId> _constructionOrder;
        const bool _strongDestructionOrder = false;

      public:
        using Ptr = std::unique_ptr<ServiceInstancesMap>;

        explicit ServiceInstancesMap(bool strongDestructionOrder = false);

        ServiceInstancesMap(const ServiceInstancesMap &) = delete;
        ServiceInstancesMap(ServiceInstancesMap &&) noexcept = default;

        ServiceInstancesMap &operator=(const ServiceInstancesMap &) = delete;

        ServiceInstanceList &insert(TypeId serviceTypeId, ServiceInstance::Ptr instance);

        ServiceInstanceList &insert(TypeId serviceTypeId, ServiceInstanceList instances);

        [[nodiscard]] bool contains(TypeId serviceTypeId) const;

        ServiceInstanceList *findServices(TypeId serviceTypeId);

        [[nodiscard]] bool empty() const;

        void clear();

        ~ServiceInstancesMap();
    };

} // namespace sb::di::details::containers

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Containers/Impl/ServiceInstancesMap.hpp"
#endif
