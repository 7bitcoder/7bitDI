#pragma once

#include <algorithm>
#include <vector>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/ServiceDescriptor.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"
#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details
{
    class ServiceDescriptorList
    {
      private:
        std::vector<ServiceDescriptor> _serviceDescriptors;

      public:
        ServiceDescriptorList() = default;

        ServiceDescriptorList(ServiceDescriptorList &&) = default;
        ServiceDescriptorList(const ServiceDescriptorList &) = delete;
        ServiceDescriptorList &operator=(const ServiceDescriptorList &) = delete;
        ServiceDescriptorList &operator=(ServiceDescriptorList &&) = default;

        auto begin() const { return _serviceDescriptors.begin(); }
        auto end() const { return _serviceDescriptors.end(); }

        auto begin() { return _serviceDescriptors.begin(); }
        auto end() { return _serviceDescriptors.end(); }

        auto rBegin() const { return _serviceDescriptors.rbegin(); }
        auto rEnd() const { return _serviceDescriptors.rend(); }

        const ServiceLifeTime &getLifeTime() const;

        TypeId getServiceTypeId() const;

        void seal();

        void add(ServiceDescriptor descriptor);

        bool contains(TypeId typeId);

        size_t size() const;
        bool empty() const;

        const ServiceDescriptor &last() const;

        ServiceDescriptor &last();

      private:
        void checkBaseType(ServiceDescriptor &descriptor);
        void checkLifeTime(ServiceDescriptor &descriptor);
    };

} // namespace sb::di::details

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/DI/Details/Impl/ServiceDescriptorList.hpp"
#endif