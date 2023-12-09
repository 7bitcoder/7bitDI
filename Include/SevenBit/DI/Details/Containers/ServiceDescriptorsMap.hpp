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
        std::unordered_set<TypeId> _registeredServices;
        const bool _checkDescriptorUniqueness = false;

      public:
        using Ptr = std::unique_ptr<ServiceDescriptorsMap>;

        [[nodiscard]] auto begin() const { return _serviceCreatorsMap.begin(); }
        [[nodiscard]] auto end() const { return _serviceCreatorsMap.end(); }

        auto begin() { return _serviceCreatorsMap.begin(); }
        auto end() { return _serviceCreatorsMap.end(); }

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
        void addDescriptor(ServiceDescriptor &&descriptor);

        void checkIfAlreadyRegistered(TypeId implementationTypeId);
        void registerService(TypeId implementationTypeId);
    };
} // namespace sb::di::details::containers

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Containers/Impl/ServiceDescriptorsMap.hpp"
#endif
