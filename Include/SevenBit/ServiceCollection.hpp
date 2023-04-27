#pragma once

#include <cstddef>
#include <exception>
#include <forward_list>
#include <list>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "SevenBit/ServiceProvider.hpp"
#include "SevenBit/_Internal/Exceptions.hpp"
#include "SevenBit/_Internal/IServiceCreator.hpp"
#include "SevenBit/_Internal/IServiceCreatorsProvider.hpp"
#include "SevenBit/_Internal/IServiceHolder.hpp"
#include "SevenBit/_Internal/ServiceConstructor.hpp"
#include "SevenBit/_Internal/ServiceCreators.hpp"
#include "SevenBit/_Internal/ServiceFactory.hpp"
#include "SevenBit/_Internal/ServiceOwner.hpp"
#include "SevenBit/_Internal/ServiceScope.hpp"
#include "SevenBit/_Internal/ServicesContainer.hpp"
#include "SevenBit/_Internal/TypeId.hpp"
#include "SevenBit/_Internal/Utils.hpp"

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

        ServiceProvider buildServiceProvider() { return ServiceProvider{this}; }

        template <class I, class T = I> void addSingleton() { add<I, T>(ServiceScope::singeleton()); }
        template <class I, class T = I> void addScoped() { add<I, T>(ServiceScope::scoped()); }
        template <class I, class T = I> void addTransient() { add<I, T>(ServiceScope::transient()); }
        template <class I, class T = I> void add(ServiceScope scope)
        {
            add(std::make_unique<ServiceConstructor<I, T>>(scope));
        }

        template <class I, class T = I, class FactoryFcn> void addSingleton(FactoryFcn factory)
        {
            add<I, T>(ServiceScope::singeleton(), std::move(factory));
        }
        template <class I, class T = I, class FactoryFcn> void addScoped(FactoryFcn factory)
        {
            add<I, T>(ServiceScope::scoped(), std::move(factory));
        }
        template <class I, class T = I, class FactoryFcn> void addTransient(FactoryFcn factory)
        {
            add<I, T>(ServiceScope::transient(), std::move(factory));
        }
        template <class I, class T = I, class FactoryFcn> void add(ServiceScope scope, FactoryFcn factory)
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

        void merge(ServiceCollection &&serviceCollection)
        {
            _serviceCreatorsMap.merge(std::move(serviceCollection._serviceCreatorsMap));
        }

      private:
        void add(IServiceCreator::Ptr creator)
        {
            if (!creator)
            {
                throw ServiceCreatorInvalidException{};
            }
            auto serviceTypeId = creator->getServiceTypeId();
            auto serviceInterfaceTypeId = creator->getServiceInterfaceTypeId();
            auto serviceScope = creator->getServiceScope();

            if (isAlreadyResistered(*creator))
            {
                throw new ServiceTypeAlreadyRegisteredException{serviceTypeId};
            }

            auto &creators = _serviceCreatorsMap[serviceInterfaceTypeId];
            if (creators.size() && creators.getServicesScope() != serviceScope)
            {
                throw ServiceScopeMissmatchException{serviceTypeId, serviceInterfaceTypeId};
            }
            creators.add(std::move(creator));
        }

        const ServiceCreators *getCreators(TypeId typeId) const
        {
            auto pair = _serviceCreatorsMap.find(typeId);
            return pair != _serviceCreatorsMap.end() ? &pair->second : nullptr;
        }

        const IServiceCreator *getMainCreator(TypeId typeId) const
        {
            if (auto creators = getCreators(typeId))
            {
                return &creators->getMainCreator();
            }
            return nullptr;
        }

        bool isAlreadyResistered(IServiceCreator &newCreator) const
        {
            auto it = _serviceCreatorsMap.find(newCreator.getServiceInterfaceTypeId());

            if (it == _serviceCreatorsMap.end())
            {
                return false;
            }

            auto &[_, creators] = *it;
            for (auto &creator : creators)
            {
                if (creator && newCreator.getServiceTypeId() == creator->getServiceTypeId())
                {
                    return true;
                }
            }
            return false;
        }

        auto begin() const { return _serviceCreatorsMap.cbegin(); }

        auto end() const { return _serviceCreatorsMap.cend(); }
    };
} // namespace sb
