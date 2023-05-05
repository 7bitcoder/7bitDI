#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <vector>

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/IServiceFactory.hpp"
#include "SevenBit/ServiceDescriber.hpp"
#include "SevenBit/ServiceDescriptor.hpp"
#include "SevenBit/ServiceLifeTime.hpp"
#include "SevenBit/ServiceProviderOptions.hpp"
#include "SevenBit/TypeId.hpp"
#include "SevenBit/_Internal/ServiceProviderRoot.hpp"

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

        IServiceProvider::Ptr buildServiceProvider(ServiceProviderOptions options = {});

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

        ServiceDescriptor &at(size_t index);
        const ServiceDescriptor &at(size_t index) const;

        ServiceDescriptor &first();
        const ServiceDescriptor &first() const;

        ServiceDescriptor &last();
        const ServiceDescriptor &last() const;

        ServiceDescriptor &operator[](size_t index);
        const ServiceDescriptor &operator[](size_t index) const;

        size_t size() const;
        size_t count() const;

        bool empty() const;

        size_t capacity() const;

        void reserve(size_t space);

        void shrinkToFit();

        void clear();

        template <class TPred> Iterator findIf(const TPred &pred) { return std::find_if(begin(), end(), pred); }

        template <class TPred> ConstIterator findIf(const TPred &pred) const
        {
            return std::find_if(begin(), end(), pred);
        }

        template <class TPred> bool containsIf(const TPred &pred) const { return findIf(pred) != end(); }

        template <class TService> bool contains() const { return contains(typeid(TService)); }

        bool contains(TypeId serviceTypeId) const;

        template <class TService, class TImplementation> bool containsExact() const
        {
            return containsExact(typeid(TService), typeid(TImplementation));
        }

        bool containsExact(TypeId serviceTypeId, TypeId implementationTypeId) const;

        Iterator insert(ConstIterator pos, ServiceDescriptor descriptor);

        ServiceCollection &add(ServiceDescriptor descriptor);

        ServiceCollection &add(const ServiceCollection &collection);

        Iterator remove(Iterator pos);
        Iterator remove(ConstIterator pos);

        Iterator removeRange(Iterator begin, Iterator end);
        Iterator removeRange(ConstIterator begin, ConstIterator end);

        template <class TPred> size_t removeIf(const TPred &pred) { return std::erase_if(_serviceDescriptors, pred); }

        template <class TService> ServiceCollection &removeAll() { return removeAll(typeid(TService)); }

        ServiceCollection &removeAll(TypeId serviceTypeId);

        template <class TService, class TImplementation = TService> ServiceCollection &remove()
        {
            return remove(typeid(TService), typeid(TImplementation));
        }

        ServiceCollection &remove(TypeId serviceTypeId, TypeId implementationTypeId);

        void pop();

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

        template <class TService> ServiceCollection &addSingleton(TService *service)
        {
            return add(ServiceDescriber::describeSingleton<TService, TService>(service));
        }

        template <class TService, class TImplementation> ServiceCollection &addSingleton(TImplementation *service)
        {
            return add(ServiceDescriber::describeSingleton<TService, TImplementation>(service));
        }

        template <class TService, class FactoryFcn> ServiceCollection &add(ServiceLifeTime lifeTime, FactoryFcn factory)
        {
            return add(ServiceDescriber::describeFrom<TService, FactoryFcn>(lifeTime, std::move(factory)));
        }

        template <class TService, class FactoryFcn> ServiceCollection &addSingleton(FactoryFcn factory)
        {
            return add(ServiceDescriber::describeSingletonFrom<TService, FactoryFcn>(std::move(factory)));
        }
        template <class TService, class FactoryFcn> ServiceCollection &addScoped(FactoryFcn factory)
        {
            return add(ServiceDescriber::describeScopedFrom<TService, FactoryFcn>(std::move(factory)));
        }
        template <class TService, class FactoryFcn> ServiceCollection &addTransient(FactoryFcn factory)
        {
            return add(ServiceDescriber::describeTransientFrom<TService, FactoryFcn>(std::move(factory)));
        }

        template <class FactoryFcn> ServiceCollection &add(ServiceLifeTime lifeTime, FactoryFcn factory)
        {
            return add(ServiceDescriber::describeFrom(lifeTime, std::move(factory)));
        }

        template <class FactoryFcn> ServiceCollection &addSingleton(FactoryFcn factory)
        {
            return add(ServiceDescriber::describeSingletonFrom(std::move(factory)));
        }
        template <class FactoryFcn> ServiceCollection &addScoped(FactoryFcn factory)
        {
            return add(ServiceDescriber::describeScopedFrom(std::move(factory)));
        }
        template <class FactoryFcn> ServiceCollection &addTransient(FactoryFcn factory)
        {
            return add(ServiceDescriber::describeTransientFrom(std::move(factory)));
        }
    };
} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/ServiceCollection.hpp"
#endif