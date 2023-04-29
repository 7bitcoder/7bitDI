#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <vector>

#include "SevenBit/ServiceProvider.hpp"
#include "SevenBit/_Internal/IServiceCreatorsProvider.hpp"
#include "SevenBit/_Internal/ServiceConstructor.hpp"
#include "SevenBit/_Internal/ServiceCreators.hpp"
#include "SevenBit/_Internal/ServiceDescriber.hpp"
#include "SevenBit/_Internal/ServiceDescriptor.hpp"
#include "SevenBit/_Internal/ServiceFactory.hpp"
#include "SevenBit/_Internal/ServiceLifeTime.hpp"
#include "SevenBit/_Internal/TypeId.hpp"

namespace sb
{
    class ServiceCollection2
    {
      private:
        std::vector<ServiceDescriptor> _serviceDescriptors;

      public:
        using Iterator = std::vector<ServiceDescriptor>::iterator;
        using ConstIterator = std::vector<ServiceDescriptor>::const_iterator;
        using ReverseIterator = std::vector<ServiceDescriptor>::reverse_iterator;
        using ConstReverseIterator = std::vector<ServiceDescriptor>::const_reverse_iterator;

        using Ptr = std::unique_ptr<ServiceCollection2>;

        template <class TImplementation>
        using Factory = std::function<std::unique_ptr<TImplementation>(ServiceProvider &)>;
        template <class TImplementation> using ExternalProvider = std::function<TImplementation *(ServiceProvider &)>;

        ServiceCollection2() = default;
        ServiceCollection2(const ServiceCollection2 &) = default;
        ServiceCollection2(ServiceCollection2 &&) = default;

        ServiceCollection2 &operator=(const ServiceCollection2 &) = default;
        ServiceCollection2 &operator=(ServiceCollection2 &&) = default;

        ServiceProvider buildServiceProvider();

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

        Iterator insert(ConstIterator pos, ServiceDescriptor descriptor)
        {
            return _serviceDescriptors.insert(pos, std::move(descriptor));
        }

        ServiceCollection2 &add(ServiceDescriptor descriptor)
        {
            _serviceDescriptors.push_back(std::move(descriptor));
            return *this;
        }

        ServiceCollection2 &add(const ServiceCollection2 &collection)
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

        template <class TService> ServiceCollection2 &removeAll() { return removeAll(typeid(TService)); }

        ServiceCollection2 &removeAll(TypeId serviceTypeId)
        {
            removeIf([&](auto &descriptor) { descriptor.getServiceTypeId() == serviceTypeId; });
            return *this;
        }

        void pop() { _serviceDescriptors.pop_back(); }

        template <class TService, class TImplementation = TService> ServiceCollection2 &addSingleton()
        {
            return add<TService, TImplementation>(ServiceLifeTime::singeleton());
        }
        template <class TService, class TImplementation = TService> ServiceCollection2 &addScoped()
        {
            return add<TService, TImplementation>(ServiceLifeTime::scoped());
        }
        template <class TService, class TImplementation = TService> ServiceCollection2 &addTransient()
        {
            return add<TService, TImplementation>(ServiceLifeTime::transient());
        }
        template <class TService, class TImplementation = TService> ServiceCollection2 &add(ServiceLifeTime scope)
        {
            return add(ServiceDescriber::describe<TService, TImplementation>(scope));
        }

        template <class TService, class TImplementation = TService>
        ServiceCollection2 &addSingleton(Factory<TImplementation> factory)
        {
            return add<TService, TImplementation>(ServiceLifeTime::singeleton(), std::move(factory));
        }
        template <class TService, class TImplementation = TService>
        ServiceCollection2 &addScoped(Factory<TImplementation> factory)
        {
            return add<TService, TImplementation>(ServiceLifeTime::scoped(), std::move(factory));
        }
        template <class TService, class TImplementation = TService>
        ServiceCollection2 &addTransient(Factory<TImplementation> factory)
        {
            return add<TService, TImplementation>(ServiceLifeTime::transient(), std::move(factory));
        }
        template <class TService, class TImplementation = TService>
        ServiceCollection2 &add(ServiceLifeTime scope, Factory<TImplementation> factory)
        {
            return add(ServiceDescriber::describe<TService>(scope, std::move(factory)));
        }

        template <class TService, class TImplementation = TService>
        ServiceCollection2 &addSingeleton(TImplementation &service)
        {
            return add<TService, TImplementation>(&service);
        }
        template <class TService, class TImplementation = TService>
        ServiceCollection2 &addSingeleton(TImplementation *service)
        {
            return add(ServiceDescriber::describe<TService>(service));
        }

        template <class TService, class TImplementation = TService>
        ServiceCollection2 &addSingeleton(ExternalProvider<TImplementation> provider)
        {
            return add(ServiceDescriber::describe<TService>(ServiceLifeTime::singeleton(), std::move(provider)));
        }
        template <class TService, class TImplementation = TService>
        ServiceCollection2 &addScoped(ExternalProvider<TImplementation> provider)
        {
            return add(ServiceDescriber::describe<TService>(ServiceLifeTime::scoped(), std::move(provider)));
        }
    };
} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/ServiceCollection.hpp"
#endif