#pragma once

#include <utility>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Core/DefaultServiceProvider.hpp"
#include "SevenBit/DI/Details/Core/DefaultServiceProviderData.hpp"
#include "SevenBit/DI/ServiceCollection.hpp"

namespace sb::di
{

    INLINE ServiceProvider::Ptr ServiceCollection::buildServiceProvider(ServiceProviderOptions options)
    {
        auto core = std::make_unique<details::core::DefaultServiceProviderData>(begin(), end(), options);
        return std::make_unique<details::core::DefaultServiceProvider>(std::move(core));
    }

    INLINE ServiceDescriptor &ServiceCollection::at(const size_t index) { return _serviceDescriptors.at(index); }
    INLINE const ServiceDescriptor &ServiceCollection::at(const size_t index) const
    {
        return _serviceDescriptors.at(index);
    }

    INLINE ServiceDescriptor &ServiceCollection::first() { return at(0); }
    INLINE const ServiceDescriptor &ServiceCollection::first() const { return at(0); }

    INLINE ServiceDescriptor &ServiceCollection::last() { return at(size() - 1); }
    INLINE const ServiceDescriptor &ServiceCollection::last() const { return at(size() - 1); }

    INLINE ServiceDescriptor &ServiceCollection::operator[](const size_t index) { return at(index); }
    INLINE const ServiceDescriptor &ServiceCollection::operator[](const size_t index) const { return at(index); }

    INLINE size_t ServiceCollection::size() const { return _serviceDescriptors.size(); }
    INLINE size_t ServiceCollection::count() const { return size(); }

    INLINE bool ServiceCollection::empty() const { return _serviceDescriptors.empty(); }

    INLINE size_t ServiceCollection::capacity() const { return _serviceDescriptors.capacity(); }

    INLINE void ServiceCollection::reserve(const size_t space) { _serviceDescriptors.reserve(space); }

    INLINE void ServiceCollection::shrinkToFit() { _serviceDescriptors.shrink_to_fit(); }

    INLINE void ServiceCollection::clear() { _serviceDescriptors.clear(); }

    INLINE bool ServiceCollection::contains(TypeId serviceTypeId) const
    {
        return containsIf([&](auto &descriptor) { return descriptor.getServiceTypeId() == serviceTypeId; });
    }

    INLINE bool ServiceCollection::containsExact(TypeId serviceTypeId, TypeId implementationTypeId) const
    {
        return containsIf([&](auto &descriptor) {
            return descriptor.getImplementationTypeId() == implementationTypeId &&
                   descriptor.getServiceTypeId() == serviceTypeId;
        });
    }

    INLINE ServiceCollection::Iterator ServiceCollection::insert(ConstIterator pos, ServiceDescriptor descriptor)
    {
        return _serviceDescriptors.insert(std::move(pos), std::move(descriptor));
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

    INLINE ServiceCollection::Iterator ServiceCollection::remove(Iterator pos)
    {
        return _serviceDescriptors.erase(std::move(pos));
    }
    INLINE ServiceCollection::Iterator ServiceCollection::remove(ConstIterator pos)
    {
        return _serviceDescriptors.erase(std::move(pos));
    }

    INLINE ServiceCollection::Iterator ServiceCollection::removeRange(Iterator begin, Iterator end)
    {
        return _serviceDescriptors.erase(std::move(begin), std::move(end));
    }
    INLINE ServiceCollection::Iterator ServiceCollection::removeRange(ConstIterator begin, ConstIterator end)
    {
        return _serviceDescriptors.erase(std::move(begin), std::move(end));
    }

    INLINE size_t ServiceCollection::removeAll(TypeId serviceTypeId)
    {
        return removeIf([&](auto &descriptor) { return descriptor.getServiceTypeId() == serviceTypeId; });
    }

    INLINE size_t ServiceCollection::remove(TypeId serviceTypeId, TypeId implementationTypeId)
    {
        return removeIf([&](auto &descriptor) {
            return descriptor.getImplementationTypeId() == implementationTypeId &&
                   descriptor.getServiceTypeId() == serviceTypeId;
        });
    }

    INLINE void ServiceCollection::pop() { _serviceDescriptors.pop_back(); }

} // namespace sb::di
