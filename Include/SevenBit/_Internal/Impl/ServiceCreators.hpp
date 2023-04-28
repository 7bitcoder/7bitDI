#pragma once

#include "SevenBit/LibraryConfig.hpp"
#include "SevenBit/_Internal/Exceptions.hpp"
#include "SevenBit/_Internal/ServiceCreators.hpp"

namespace sb
{
    INLINE ServiceCreators::ServiceCreators() { _serviceCreators.reserve(1); }

    INLINE ServiceLifeTime ServiceCreators::getServicesScope() const { return getMainCreator().getServiceScope(); }

    INLINE TypeId ServiceCreators::getInterfaceTypeId() const { return getMainCreator().getServiceInterfaceTypeId(); }

    INLINE void ServiceCreators::add(IServiceCreator::Ptr creator)
    {
        if (!creator)
        {
            throw ServiceCreatorInvalidException{};
        }
        _serviceCreators.emplace_back(std::move(creator));
    }

    INLINE size_t ServiceCreators::remove(TypeId typeId)
    {
        return std::erase_if(_serviceCreators, [=](auto &creator) { return creator->getServiceTypeId() == typeId; });
    }

    INLINE bool ServiceCreators::contains(TypeId typeId)
    {
        for (auto &creator : _serviceCreators)
        {
            if (creator->getServiceTypeId() == typeId)
            {
                return true;
            }
        }
        return false;
    }

    INLINE void ServiceCreators::seal() { _serviceCreators.shrink_to_fit(); }

    INLINE size_t ServiceCreators::size() const { return _serviceCreators.size(); }

    INLINE IServiceCreator &ServiceCreators::getMainCreator() const { return *_serviceCreators.back(); }

} // namespace sb