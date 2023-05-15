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

        ServicesMap(bool strongDestructionOrder);

        ServiceList &add(TypeId serviceTypeId, IServiceInstance::Ptr service);

        ServiceList &operator[](TypeId serviceTypeId);

        ServiceList *getList(TypeId serviceTypeId);

        void clear();

        ~ServicesMap();
    };

} // namespace sb::di::details

#ifdef SEVEN_BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Impl/ServicesMap.hpp"
#endif