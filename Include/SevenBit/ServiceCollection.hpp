#pragma once

#include <memory>
#include <unordered_map>

#include "SevenBit/ServiceProvider.hpp"
#include "SevenBit/_Internal/IServiceCreatorsProvider.hpp"
#include "SevenBit/_Internal/ServiceConstructor.hpp"
#include "SevenBit/_Internal/ServiceCreators.hpp"
#include "SevenBit/_Internal/ServiceFactory.hpp"
#include "SevenBit/_Internal/ServiceLifeTime.hpp"
#include "SevenBit/_Internal/TypeId.hpp"

namespace sb
{
    class ServiceCollection : private IServiceCreatorsProvider
    {
      private:
        std::unordered_map<TypeId, ServiceCreators> _serviceCreatorsMap;

      public:
        using Ptr = std::unique_ptr<ServiceCollection>;

        ServiceCollection() = default;
        ServiceCollection(const ServiceCollection &) = delete;
        ServiceCollection(ServiceCollection &&) = default;

        ServiceCollection &operator=(const ServiceCollection &) = delete;
        ServiceCollection &operator=(ServiceCollection &&) = default;

        ServiceProvider buildServiceProvider();

        template <class I, class T = I> void addSingleton() { add<I, T>(ServiceLifeTime::singeleton()); }
        template <class I, class T = I> void addScoped() { add<I, T>(ServiceLifeTime::scoped()); }
        template <class I, class T = I> void addTransient() { add<I, T>(ServiceLifeTime::transient()); }
        template <class I, class T = I> void add(ServiceLifeTime scope)
        {
            add(std::make_unique<ServiceConstructor<I, T>>(scope));
        }

        template <class I, class T = I, class FactoryFcn> void addSingleton(FactoryFcn factory)
        {
            add<I, T>(ServiceLifeTime::singeleton(), std::move(factory));
        }
        template <class I, class T = I, class FactoryFcn> void addScoped(FactoryFcn factory)
        {
            add<I, T>(ServiceLifeTime::scoped(), std::move(factory));
        }
        template <class I, class T = I, class FactoryFcn> void addTransient(FactoryFcn factory)
        {
            add<I, T>(ServiceLifeTime::transient(), std::move(factory));
        }
        template <class I, class T = I, class FactoryFcn> void add(ServiceLifeTime scope, FactoryFcn factory)
        {
            add(std::make_unique<ServiceFactory<I, T, FactoryFcn>>(scope, std::move(factory)));
        }

        template <class I> bool contains() const { return _serviceCreatorsMap.contains(typeid(I)); }

        template <class I, class T> bool contains() const
        {
            if (auto it = _serviceCreatorsMap.find(typeid(I)); it != _serviceCreatorsMap.end())
            {
                return it->second.contains<I, T>();
            }
            return false;
        }

        template <class I> bool removeAll() { return _serviceCreatorsMap.erase(typeid(I)) > 0; }

        template <class I, class T = I> bool remove()
        {
            auto it = _serviceCreatorsMap.find(typeid(I));
            if (it != _serviceCreatorsMap.end())
            {
                auto &creators = it->second;
                auto removed = creators.remove<T>() > 0;
                if (creators.size() == 0)
                {
                    _serviceCreatorsMap.erase(it);
                }
                return removed;
            }

            return false;
        }

        void merge(ServiceCollection &&serviceCollection);

      private:
        void add(IServiceCreator::Ptr creator);

        const ServiceCreators *getCreators(TypeId typeId) const;

        const IServiceCreator *getMainCreator(TypeId typeId) const;

        bool isAlreadyResistered(IServiceCreator &newCreator) const;

        auto begin() const;

        auto end() const;
    };
} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/ServiceCollection.hpp"
#endif