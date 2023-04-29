#pragma once

#include "SevenBit/LibraryConfig.hpp"
#include "SevenBit/_Internal/ServiceLifeTime.hpp"

namespace sb
{
    INLINE ServiceLifeTime ServiceLifeTime::singeleton() { return {Type::Singeleton}; }
    INLINE ServiceLifeTime ServiceLifeTime::scoped() { return {Type::Scoped}; }
    INLINE ServiceLifeTime ServiceLifeTime::transient() { return {Type::Transient}; }

    INLINE ServiceLifeTime::ServiceLifeTime(Type type) : _type(type) {}

    INLINE bool ServiceLifeTime::isSingeleton() const { return is(Type::Singeleton); }
    INLINE bool ServiceLifeTime::isScoped() const { return is(Type::Scoped); }
    INLINE bool ServiceLifeTime::isTransient() const { return is(Type::Transient); }
    INLINE bool ServiceLifeTime::is(Type type) const { return _type == type; }

    INLINE bool ServiceLifeTime::operator==(const ServiceLifeTime &scope) const { return _type == scope._type; }
    INLINE bool ServiceLifeTime::operator!=(const ServiceLifeTime &scope) const { return _type != scope._type; }
    INLINE bool ServiceLifeTime::operator<(const ServiceLifeTime &scope) const { return _type < scope._type; }
    INLINE bool ServiceLifeTime::operator>(const ServiceLifeTime &scope) const { return _type > scope._type; }
    INLINE bool ServiceLifeTime::operator<=(const ServiceLifeTime &scope) const { return _type <= scope._type; }
    INLINE bool ServiceLifeTime::operator>=(const ServiceLifeTime &scope) const { return _type >= scope._type; }
} // namespace sb