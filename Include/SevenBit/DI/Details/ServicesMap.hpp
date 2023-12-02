#pragma once

#include <memory>
#include <unordered_map>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/ServiceList.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details
{
    class EXPORT ServicesMap
    {
      private:
        std::unordered_map<TypeId, ServiceList> _serviceListMap;
        std::vector<IServiceInstance::Ptr *> _constructionOrder;
        bool _strongDestructionOrder = false;

      public:
        using Ptr = std::unique_ptr<ServicesMap>;

        explicit ServicesMap(bool strongDestructionOrder = false);

        ServicesMap(const ServicesMap &) = delete;
        ServicesMap(ServicesMap &&) noexcept = default;

        ServicesMap &operator=(const ServicesMap &) = delete;
        ServicesMap &operator=(ServicesMap &&) = default;

        ServiceList &insert(TypeId serviceTypeId, IServiceInstance::Ptr service);

        [[nodiscard]] bool contains(TypeId serviceTypeId) const;

        ServiceList *findServices(TypeId serviceTypeId);

        [[nodiscard]] bool empty() const;

        void clear();

        ~ServicesMap();

      private:
        ServiceList &add(TypeId serviceTypeId, IServiceInstance::Ptr &&service);
    };

} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Impl/ServicesMap.hpp"
#endif
