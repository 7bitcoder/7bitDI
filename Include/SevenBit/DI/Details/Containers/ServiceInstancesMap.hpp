#pragma once

#include <memory>
#include <unordered_map>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/ServiceInstanceList.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details::containers
{
    class EXPORT ServiceInstancesMap
    {
      private:
        std::unordered_map<TypeId, ServiceInstanceList> _serviceListMap;
        std::vector<IServiceInstance::Ptr *> _constructionOrder;
        bool _strongDestructionOrder = false;

      public:
        using Ptr = std::unique_ptr<ServiceInstancesMap>;

        explicit ServiceInstancesMap(bool strongDestructionOrder = false);

        ServiceInstancesMap(const ServiceInstancesMap &) = delete;
        ServiceInstancesMap(ServiceInstancesMap &&) noexcept = default;

        ServiceInstancesMap &operator=(const ServiceInstancesMap &) = delete;
        ServiceInstancesMap &operator=(ServiceInstancesMap &&) = default;

        ServiceInstanceList &insert(TypeId serviceTypeId, IServiceInstance::Ptr service);

        [[nodiscard]] bool contains(TypeId serviceTypeId) const;

        ServiceInstanceList *findServices(TypeId serviceTypeId);

        [[nodiscard]] bool empty() const;

        void clear();

        ~ServiceInstancesMap();

      private:
        ServiceInstanceList &add(TypeId serviceTypeId, IServiceInstance::Ptr &&service);
    };

} // namespace sb::di::details::containers

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Containers/Impl/ServiceInstancesMap.hpp"
#endif
