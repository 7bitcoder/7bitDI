#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/Cast.hpp"
#include "SevenBit/DI/Details/Utils/RequireBase.hpp"
#include "SevenBit/DI/ServiceInstance.hpp"

namespace sb::di
{
    INLINE ServiceInstance::ServiceInstance(IServiceInstance::Ptr implementation, const ptrdiff_t castOffset)
        : _implementation(std::move(implementation)), _castOffset(castOffset)
    {
    }

    INLINE IServiceInstance *ServiceInstance::tryGetImplementation() { return _implementation.get(); }

    INLINE const IServiceInstance *ServiceInstance::tryGetImplementation() const { return _implementation.get(); }

    INLINE IServiceInstance &ServiceInstance::getImplementation()
    {
        return *details::RequireBase::notNullAndGet(_implementation.get());
    }

    INLINE const IServiceInstance &ServiceInstance::getImplementation() const
    {
        return *details::RequireBase::notNullAndGet(_implementation.get());
    }

    INLINE void ServiceInstance::addCastOffset(const ptrdiff_t castOffset) { _castOffset += castOffset; }

    INLINE void ServiceInstance::setCastOffset(const ptrdiff_t castOffset) { _castOffset = castOffset; }

    INLINE ptrdiff_t ServiceInstance::getCastOffset() const { return _castOffset; }

    INLINE bool ServiceInstance::isValid() const { return _implementation && _implementation->isValid(); }

    INLINE ServiceInstance::operator bool() const { return isValid(); }

    INLINE void ServiceInstance::clear()
    {
        _implementation.reset();
        _castOffset = 0;
    }

    INLINE void *ServiceInstance::applyOffset(void *ptr) const
    {
        return details::Cast::applyCastOffset(ptr, _castOffset);
    }

} // namespace sb::di
