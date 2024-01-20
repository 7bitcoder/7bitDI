#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/Require.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"

namespace sb::di
{
    INLINE ServiceLifeTime ServiceLifeTime::singleton() { return ServiceLifeTime{Singleton}; }
    INLINE ServiceLifeTime ServiceLifeTime::scoped() { return ServiceLifeTime{Scoped}; }
    INLINE ServiceLifeTime ServiceLifeTime::transient() { return ServiceLifeTime{Transient}; }

    INLINE ServiceLifeTime::ServiceLifeTime(const Type type) : _type(details::utils::Require::validEnumAndGet(type)) {}

    INLINE bool ServiceLifeTime::is(const Type type) const { return _type == type; }
    INLINE bool ServiceLifeTime::isSingleton() const { return is(Singleton); }
    INLINE bool ServiceLifeTime::isScoped() const { return is(Scoped); }
    INLINE bool ServiceLifeTime::isTransient() const { return is(Transient); }
    INLINE bool ServiceLifeTime::isAlias() const { return is(Alias); }

    INLINE bool ServiceLifeTime::operator==(const ServiceLifeTime &scope) const { return _type == scope._type; }
    INLINE bool ServiceLifeTime::operator!=(const ServiceLifeTime &scope) const { return _type != scope._type; }
} // namespace sb::di
