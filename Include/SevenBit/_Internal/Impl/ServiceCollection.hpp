#pragma once

#include "SevenBit/LibraryConfig.hpp"
#include "SevenBit/ServiceCollection.hpp"
#include "SevenBit/_Internal/Exceptions.hpp"

namespace sb
{
    INLINE ServiceProvider ServiceCollection::buildServiceProvider() { return ServiceProvider{this}; }

    INLINE void ServiceCollection::merge(ServiceCollection &&serviceCollection)
    {
        _serviceCreatorsMap.merge(std::move(serviceCollection._serviceCreatorsMap));
    }

    INLINE void ServiceCollection::add(IServiceCreator::Ptr creator)
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

    INLINE const ServiceCreators *ServiceCollection::getCreators(TypeId typeId) const
    {
        auto pair = _serviceCreatorsMap.find(typeId);
        return pair != _serviceCreatorsMap.end() ? &pair->second : nullptr;
    }

    INLINE const IServiceCreator *ServiceCollection::getMainCreator(TypeId typeId) const
    {
        if (auto creators = getCreators(typeId))
        {
            return &creators->getMainCreator();
        }
        return nullptr;
    }

    INLINE bool ServiceCollection::isAlreadyResistered(IServiceCreator &newCreator) const
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

    INLINE auto ServiceCollection::begin() const { return _serviceCreatorsMap.cbegin(); }

    INLINE auto ServiceCollection::end() const { return _serviceCreatorsMap.cend(); }
} // namespace sb
