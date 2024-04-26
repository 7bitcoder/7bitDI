#pragma once

#include <utility>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/ServiceInstanceProviderRoot.hpp"
#include "SevenBit/DI/ServiceCollection.hpp"

namespace sb::di
{
    INLINE ServiceProvider ServiceCollection::buildServiceProvider(ServiceProviderOptions options)
    {
        return ServiceProvider{std::make_unique<details::ServiceInstanceProviderRoot>(begin(), end(), options)};
    }

    INLINE ServiceProvider::Ptr ServiceCollection::buildServiceProviderAsPtr(ServiceProviderOptions options)
    {
        auto instanceProvider = std::make_unique<details::ServiceInstanceProviderRoot>(begin(), end(), options);
        return std::make_unique<ServiceProvider>(std::move(instanceProvider));
    }

    INLINE bool ServiceCollection::contains(const TypeId serviceTypeId) const
    {
        return containsIf(
            [&](const ServiceDescriptor &descriptor) { return descriptor.getServiceTypeId() == serviceTypeId; });
    }

    INLINE bool ServiceCollection::containsKeyed(const TypeId serviceTypeId, const std::string_view serviceKey) const
    {
        return containsIf([&](const ServiceDescriptor &descriptor) {
            return descriptor.getServiceTypeId() == serviceTypeId && descriptor.getServiceKey() &&
                   *descriptor.getServiceKey() == serviceKey;
        });
    }

    INLINE bool ServiceCollection::containsExact(const TypeId serviceTypeId, const TypeId implementationTypeId) const
    {
        return containsIf([&](const ServiceDescriptor &descriptor) {
            return descriptor.getImplementationTypeId() == implementationTypeId &&
                   descriptor.getServiceTypeId() == serviceTypeId;
        });
    }

    INLINE bool ServiceCollection::containsKeyedExact(const TypeId serviceTypeId, const TypeId implementationTypeId,
                                                      const std::string_view serviceKey) const
    {
        return containsIf([&](const ServiceDescriptor &descriptor) {
            return descriptor.getImplementationTypeId() == implementationTypeId &&
                   descriptor.getServiceTypeId() == serviceTypeId && descriptor.getServiceKey() &&
                   *descriptor.getServiceKey() == serviceKey;
        });
    }

    INLINE ServiceCollection &ServiceCollection::add(ServiceDescriptor descriptor)
    {
        _serviceDescriptors.push_back(std::move(descriptor));
        return *this;
    }

    INLINE ServiceCollection &ServiceCollection::add(const ServiceCollection &collection)
    {
        reserve(size() + collection.size());
        for (auto &descriptor : collection)
        {
            add(descriptor);
        }
        return *this;
    }

    INLINE std::size_t ServiceCollection::removeAll(const TypeId serviceTypeId)
    {
        return removeIf(
            [&](const ServiceDescriptor &descriptor) { return descriptor.getServiceTypeId() == serviceTypeId; });
    }

    INLINE std::size_t ServiceCollection::removeAllKeyed(const TypeId serviceTypeId, const std::string_view serviceKey)
    {
        return removeIf([&](const ServiceDescriptor &descriptor) {
            return descriptor.getServiceTypeId() == serviceTypeId && descriptor.getServiceKey() &&
                   *descriptor.getServiceKey() == serviceKey;
        });
    }

    INLINE std::size_t ServiceCollection::remove(const TypeId serviceTypeId, const TypeId implementationTypeId)
    {
        return removeIf([&](const ServiceDescriptor &descriptor) {
            return descriptor.getImplementationTypeId() == implementationTypeId &&
                   descriptor.getServiceTypeId() == serviceTypeId;
        });
    }

    INLINE std::size_t ServiceCollection::removeKeyed(const TypeId serviceTypeId, const TypeId implementationTypeId,
                                                      const std::string_view serviceKey)
    {
        return removeIf([&](const ServiceDescriptor &descriptor) {
            return descriptor.getImplementationTypeId() == implementationTypeId &&
                   descriptor.getServiceTypeId() == serviceTypeId && descriptor.getServiceKey() &&
                   *descriptor.getServiceKey() == serviceKey;
        });
    }
} // namespace sb::di
