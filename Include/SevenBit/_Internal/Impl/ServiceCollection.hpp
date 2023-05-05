#pragma once

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/ServiceCollection.hpp"

namespace sb
{

    INLINE IServiceProvider::Ptr ServiceCollection::buildServiceProvider(ServiceProviderOptions options)
    {
        return std::make_unique<internal::ServiceProviderRoot>(begin(), end(), options);
    }

    INLINE ServiceDescriptor &ServiceCollection::at(size_t index) { return _serviceDescriptors.at(index); }
    INLINE const ServiceDescriptor &ServiceCollection::at(size_t index) const { return _serviceDescriptors.at(index); }

    INLINE ServiceDescriptor &ServiceCollection::first() { return at(0); }
    INLINE const ServiceDescriptor &ServiceCollection::first() const { return at(0); }

    INLINE ServiceDescriptor &ServiceCollection::last() { return at(size() - 1); }
    INLINE const ServiceDescriptor &ServiceCollection::last() const { return at(size() - 1); }

    INLINE ServiceDescriptor &ServiceCollection::operator[](size_t index) { return at(index); }
    INLINE const ServiceDescriptor &ServiceCollection::operator[](size_t index) const { return at(index); }

    INLINE size_t ServiceCollection::size() const { return _serviceDescriptors.size(); }
    INLINE size_t ServiceCollection::count() const { return size(); }

    INLINE bool ServiceCollection::empty() const { return _serviceDescriptors.empty(); }

    INLINE size_t ServiceCollection::capacity() const { return _serviceDescriptors.capacity(); }

    INLINE void ServiceCollection::reserve(size_t space) { _serviceDescriptors.reserve(space); }

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
        return _serviceDescriptors.insert(pos, std::move(descriptor));
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
        return _serviceDescriptors.erase(pos);
    }
    INLINE ServiceCollection::Iterator ServiceCollection::remove(ConstIterator pos)
    {
        return _serviceDescriptors.erase(pos);
    }

    INLINE ServiceCollection::Iterator ServiceCollection::removeRange(Iterator begin, Iterator end)
    {
        return _serviceDescriptors.erase(begin, end);
    }
    INLINE ServiceCollection::Iterator ServiceCollection::removeRange(ConstIterator begin, ConstIterator end)
    {
        return _serviceDescriptors.erase(begin, end);
    }

    INLINE ServiceCollection &ServiceCollection::removeAll(TypeId serviceTypeId)
    {
        removeIf([&](auto &descriptor) { return descriptor.getServiceTypeId() == serviceTypeId; });
        return *this;
    }

    INLINE ServiceCollection &ServiceCollection::remove(TypeId serviceTypeId, TypeId implementationTypeId)
    {
        removeIf([&](auto &descriptor) {
            return descriptor.getImplementationTypeId() == implementationTypeId &&
                   descriptor.getServiceTypeId() == serviceTypeId;
        });
        return *this;
    }

    INLINE void ServiceCollection::pop() { _serviceDescriptors.pop_back(); }

} // namespace sb
