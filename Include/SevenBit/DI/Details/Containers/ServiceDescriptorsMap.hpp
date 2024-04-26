#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Details/Models/ServiceId.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"

namespace sb::di::details
{
    class EXPORT ServiceDescriptorsMap
    {
        std::unordered_map<ServiceId, ServiceDescriptorList> _descriptorsMap;
        std::unique_ptr<std::unordered_set<ServiceId>> _registeredServices;

      public:
        using Ptr = std::unique_ptr<ServiceDescriptorsMap>;

        [[nodiscard]] auto begin() const { return _descriptorsMap.begin(); }
        [[nodiscard]] auto end() const { return _descriptorsMap.end(); }

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
        ServiceDescriptorsMap(ServiceDescriptorsMap &&) = default;

        ServiceDescriptorsMap &operator=(const ServiceDescriptorsMap &) = delete;
        ServiceDescriptorsMap &operator=(ServiceDescriptorsMap &&) = delete;

        void add(ServiceDescriptor descriptor);

        void seal();

        [[nodiscard]] const ServiceDescriptorList *findDescriptors(const ServiceId &id) const;

      private:
        void addDescriptorWithCheck(ServiceDescriptor &&descriptor);
        void addDescriptor(ServiceDescriptor &&descriptor);
    };
} // namespace sb::di::details

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Containers/Impl/ServiceDescriptorsMap.hpp"
#endif
