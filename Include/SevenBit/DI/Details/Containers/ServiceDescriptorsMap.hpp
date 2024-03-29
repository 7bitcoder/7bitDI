#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details::containers
{
    class EXPORT ServiceDescriptorsMap
    {
        std::unordered_map<TypeId, ServiceDescriptorList> _serviceCreatorsMap;
        std::unique_ptr<std::unordered_set<TypeId>> _registeredServicesCheck;

      public:
        using Ptr = std::unique_ptr<ServiceDescriptorsMap>;

        [[nodiscard]] auto begin() const { return _serviceCreatorsMap.begin(); }
        [[nodiscard]] auto end() const { return _serviceCreatorsMap.end(); }

        explicit ServiceDescriptorsMap(bool checkDescriptorUniqueness);

        template <class TDescriptorIt>
        ServiceDescriptorsMap(TDescriptorIt begin, TDescriptorIt end, const bool checkDescriptorUniqueness = false)
            : ServiceDescriptorsMap(checkDescriptorUniqueness)
        {
            for (auto it = begin; it != end; ++it)
            {
                add(*it);
            }
        }

        ServiceDescriptorsMap(const ServiceDescriptorsMap &) = delete;
        ServiceDescriptorsMap(ServiceDescriptorsMap &&) noexcept = default;

        ServiceDescriptorsMap &operator=(const ServiceDescriptorsMap &) = delete;

        void add(ServiceDescriptor descriptor);

        void seal();

        [[nodiscard]] const ServiceDescriptorList *findDescriptors(TypeId typeId) const;

      private:
        void addDescriptorWithCheck(ServiceDescriptor &&descriptor);
        void addDescriptor(ServiceDescriptor &&descriptor);
    };
} // namespace sb::di::details::containers

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Containers/Impl/ServiceDescriptorsMap.hpp"
#endif
