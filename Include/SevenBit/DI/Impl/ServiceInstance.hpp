#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/Cast.hpp"
#include "SevenBit/DI/Details/Utils/Require.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceInstance.hpp"

namespace sb::di
{
    INLINE ServiceInstance::ServiceInstance(IServiceInstance::Ptr implementation, const ptrdiff_t castOffset)
        : _implementation(std::move(implementation)), _castOffset(castOffset)
    {
        details::utils::Require::notNull(_implementation);
        if (!_implementation->isValid())
        {
            throw InvalidServiceException{_implementation->getTypeId()};
        }
    }

    INLINE IServiceInstance &ServiceInstance::getImplementation()
    {
        return *details::utils::Require::notNullAndGet(_implementation.get());
    }

    INLINE const IServiceInstance &ServiceInstance::getImplementation() const
    {
        return *details::utils::Require::notNullAndGet(_implementation.get());
    }

    INLINE void ServiceInstance::addCastOffset(const ptrdiff_t castOffset) { _castOffset += castOffset; }

    INLINE bool ServiceInstance::isValid() const { return _implementation && _implementation->isValid(); }

    INLINE ServiceInstance::operator bool() const { return isValid(); }

    INLINE void ServiceInstance::clear()
    {
        if (_implementation)
        {
            _implementation.reset();
        }
        _castOffset = 0;
    }

    INLINE void *ServiceInstance::applyOffset(void *ptr) const
    {
        return details::utils::Cast::applyCastOffset(ptr, _castOffset);
    }

} // namespace sb::di
