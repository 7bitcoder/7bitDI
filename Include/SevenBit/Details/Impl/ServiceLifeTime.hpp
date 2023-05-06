#pragma once

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/ServiceLifeTime.hpp"

namespace sb
{
    INLINE ServiceLifeTime ServiceLifeTime::singleton() { return {Type::Singleton}; }
    INLINE ServiceLifeTime ServiceLifeTime::scoped() { return {Type::Scoped}; }
    INLINE ServiceLifeTime ServiceLifeTime::transient() { return {Type::Transient}; }

    INLINE ServiceLifeTime::ServiceLifeTime(Type type) : _type(type) {}

    INLINE bool ServiceLifeTime::isSingleton() const { return is(Type::Singleton); }
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