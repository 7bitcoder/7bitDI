#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <vector>

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/ServiceProvider.hpp"
#include "SevenBit/_Internal/IServiceFactory.hpp"
#include "SevenBit/_Internal/ServiceDescriber.hpp"
#include "SevenBit/_Internal/ServiceDescriptor.hpp"
#include "SevenBit/_Internal/ServiceLifeTime.hpp"
#include "SevenBit/_Internal/ServiceProviderOptions.hpp"
#include "SevenBit/_Internal/TypeId.hpp"

namespace sb
{
    class ServiceCollection
    {
      private:
        std::vector<ServiceDescriptor> _serviceDescriptors;

      public:
        using Iterator = std::vector<ServiceDescriptor>::iterator;
        using ConstIterator = std::vector<ServiceDescriptor>::const_iterator;
        using ReverseIterator = std::vector<ServiceDescriptor>::reverse_iterator;
        using ConstReverseIterator = std::vector<ServiceDescriptor>::const_reverse_iterator;

        using Ptr = std::unique_ptr<ServiceCollection>;

        ServiceCollection() = default;
        ServiceCollection(const ServiceCollection &) = default;
        ServiceCollection(ServiceCollection &&) = default;

        ServiceCollection &operator=(const ServiceCollection &) = default;
        ServiceCollection &operator=(ServiceCollection &&) = default;

        IServiceProvider::Ptr buildServiceProvider(ServiceProviderOptions options = {})
        {
            return std::make_unique<ServiceProvider>(begin(), end(), options);
        }

        Iterator begin() { return _serviceDescriptors.begin(); }
        Iterator end() { return _serviceDescriptors.end(); }

        ConstIterator cBegin() const { return _serviceDescriptors.cbegin(); }
        ConstIterator cEnd() const { return _serviceDescriptors.cend(); }

        ConstIterator begin() const { return cBegin(); }
        ConstIterator end() const { return cEnd(); }

        ReverseIterator rBegin() { return _serviceDescriptors.rbegin(); }
        ReverseIterator rEnd() { return _serviceDescriptors.rend(); }

        ConstReverseIterator crBegin() const { return _serviceDescriptors.crbegin(); }
        ConstReverseIterator crEnd() const { return _serviceDescriptors.crend(); }

        ConstReverseIterator rbegin() const { return crBegin(); }
        ConstReverseIterator rend() const { return crEnd(); }

        ServiceDescriptor &at(size_t index) { return _serviceDescriptors.at(index); }
        const ServiceDescriptor &at(size_t index) const { return _serviceDescriptors.at(index); }

        ServiceDescriptor &front() { return _serviceDescriptors.front(); }
        const ServiceDescriptor &front() const { return _serviceDescriptors.front(); }

        ServiceDescriptor &back() { return _serviceDescriptors.back(); }
        const ServiceDescriptor &back() const { return _serviceDescriptors.back(); }

        ServiceDescriptor &operator[](size_t index) { return at(index); }
        const ServiceDescriptor &operator[](size_t index) const { return at(index); }

        size_t size() const { return _serviceDescriptors.size(); }
        size_t count() const { return size(); }

        bool empty() const { return _serviceDescriptors.empty(); }

        size_t capacity() const { return _serviceDescriptors.capacity(); }

        void reserve(size_t space) { _serviceDescriptors.reserve(space); }

        void shrinkToFit() { _serviceDescriptors.shrink_to_fit(); }

        void clear() { _serviceDescriptors.clear(); }

        template <class TPred> Iterator findIf(const TPred &pred) { return std::find_if(begin(), end(), pred); }

        template <class TPred> ConstIterator findIf(const TPred &pred) const
        {
            return std::find_if(begin(), end(), pred);
        }

        template <class TPred> bool containsIf(const TPred &pred) const { return findIf(pred) != end(); }

        template <class TService> bool contains() const { return contains(typeid(TService)); }

        bool contains(TypeId serviceTypeId) const
        {
            return containsIf([&](auto &descriptor) { return descriptor.getServiceTypeId() == serviceTypeId; });
        }

        template <class TService, class TImplementation> bool containsExact() const
        {
            return containsExact(typeid(TService), typeid(TImplementation));
        }

        bool containsExact(TypeId serviceTypeId, TypeId implementationTypeId) const
        {
            return containsIf([&](auto &descriptor) {
                return descriptor.getImplementationTypeId() == implementationTypeId &&
                       descriptor.getServiceTypeId() == serviceTypeId;
            });
        }

        Iterator insert(ConstIterator pos, ServiceDescriptor descriptor)
        {
            return _serviceDescriptors.insert(pos, std::move(descriptor));
        }

        ServiceCollection &add(ServiceDescriptor descriptor)
        {
            _serviceDescriptors.push_back(std::move(descriptor));
            return *this;
        }

        ServiceCollection &add(const ServiceCollection &collection)
        {
            reserve(size() + collection.size());
            for (auto &descriptor : collection)
            {
                add(descriptor);
            }
            return *this;
        }

        Iterator remove(Iterator pos) { return _serviceDescriptors.erase(pos); }
        Iterator remove(ConstIterator pos) { return _serviceDescriptors.erase(pos); }

        Iterator removeRange(Iterator begin, Iterator end) { return _serviceDescriptors.erase(begin, end); }
        Iterator removeRange(ConstIterator begin, ConstIterator end) { return _serviceDescriptors.erase(begin, end); }

        template <class TPred> size_t removeIf(const TPred &pred) { return std::erase_if(_serviceDescriptors, pred); }

        template <class TService> ServiceCollection &removeAll() { return removeAll(typeid(TService)); }

        ServiceCollection &removeAll(TypeId serviceTypeId)
        {
            removeIf([&](auto &descriptor) { return descriptor.getServiceTypeId() == serviceTypeId; });
            return *this;
        }

        template <class TService, class TImplementation = TService> ServiceCollection &remove()
        {
            return remove(typeid(TService), typeid(TImplementation));
        }

        ServiceCollection &remove(TypeId serviceTypeId, TypeId implementationTypeId)
        {
            removeIf([&](auto &descriptor) {
                return descriptor.getImplementationTypeId() == implementationTypeId &&
                       descriptor.getServiceTypeId() == serviceTypeId;
            });
            return *this;
        }

        void pop() { _serviceDescriptors.pop_back(); }

        template <class TService, class TImplementation = TService> ServiceCollection &add(ServiceLifeTime lifeTime)
        {
            return add(ServiceDescriber::describe<TService, TImplementation>(lifeTime));
        }
        template <class TService, class TImplementation = TService> ServiceCollection &addSingleton()
        {
            return add(ServiceDescriber::describeSingleton<TService, TImplementation>());
        }
        template <class TService, class TImplementation = TService> ServiceCollection &addScoped()
        {
            return add(ServiceDescriber::describeScoped<TService, TImplementation>());
        }
        template <class TService, class TImplementation = TService> ServiceCollection &addTransient()
        {
            return add(ServiceDescriber::describeTransient<TService, TImplementation>());
        }

        template <class TService, class FactoryFcn>
        ServiceCollection &addTransient(ServiceLifeTime lifeTime, FactoryFcn factory)
        {
            return add(ServiceDescriber::describe<TService, FactoryFcn>(lifeTime, std::move(factory)));
        }

        template <class TService, class FactoryFcn> ServiceCollection &addSingleton(FactoryFcn factory)
        {
            return add(ServiceDescriber::describeSingleton<TService, FactoryFcn>(std::move(factory)));
        }
        template <class TService, class FactoryFcn> ServiceCollection &addScoped(FactoryFcn factory)
        {
            return add(ServiceDescriber::describeScoped<TService, FactoryFcn>(std::move(factory)));
        }
        template <class TService, class FactoryFcn> ServiceCollection &addTransient(FactoryFcn factory)
        {
            return add(ServiceDescriber::describeTransient<TService, FactoryFcn>(std::move(factory)));
        }

        template <class FactoryFcn> ServiceCollection &addTransient(ServiceLifeTime lifeTime, FactoryFcn factory)
        {
            return add(ServiceDescriber::describe(lifeTime, std::move(factory)));
        }

        template <class FactoryFcn> ServiceCollection &addSingleton(FactoryFcn factory)
        {
            return add(ServiceDescriber::describeSingleton(std::move(factory)));
        }
        template <class FactoryFcn> ServiceCollection &addScoped(FactoryFcn factory)
        {
            return add(ServiceDescriber::describeScoped(std::move(factory)));
        }
        template <class FactoryFcn> ServiceCollection &addTransient(FactoryFcn factory)
        {
            return add(ServiceDescriber::describeTransient(std::move(factory)));
        }
    };
} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#endif