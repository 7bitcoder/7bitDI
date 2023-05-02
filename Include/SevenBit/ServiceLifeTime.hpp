#pragma once

#include "SevenBit/LibraryConfig.hpp"

namespace sb
{
    struct ServiceLifeTime
    {
      public:
        enum Type
        {
            Singleton,
            Scoped,
            Transient
        };

      private:
        Type _type;

      public:
        static ServiceLifeTime singleton();
        static ServiceLifeTime scoped();
        static ServiceLifeTime transient();

        ServiceLifeTime(Type type);

        ServiceLifeTime(const ServiceLifeTime &) = default;
        ServiceLifeTime &operator=(const ServiceLifeTime &) = default;

        bool isSingleton() const;
        bool isScoped() const;
        bool isTransient() const;
        bool is(Type type) const;

        bool operator>(const ServiceLifeTime &scope) const;
        bool operator<(const ServiceLifeTime &scope) const;
        bool operator>=(const ServiceLifeTime &scope) const;
        bool operator<=(const ServiceLifeTime &scope) const;
        bool operator!=(const ServiceLifeTime &scope) const;
        bool operator==(const ServiceLifeTime &scope) const;
    };

} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/ServiceLifeTime.hpp"
#endif