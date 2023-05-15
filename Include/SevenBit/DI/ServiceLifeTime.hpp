#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di
{

    /**
     * @brief Class used to describe service lifetime
     */
    class EXPORT ServiceLifeTime
    {
      public:
        /**
         * @brief 3 types of availabe service lifetimes
         */
        enum Type
        {
            Singleton,
            Scoped,
            Transient
        };

      private:
        Type _type;

      public:
        /**
         * @brief creates singleton service lifetime
         */
        static ServiceLifeTime singleton();
        /**
         * @brief creates scoped service lifetime
         */
        static ServiceLifeTime scoped();
        /**
         * @brief creates transient service lifetime
         */
        static ServiceLifeTime transient();

        /**
         * @brief Construct a new Service Life Time object with specified type
         */
        ServiceLifeTime(Type type);

        ServiceLifeTime(const ServiceLifeTime &) = default;
        ServiceLifeTime &operator=(const ServiceLifeTime &) = default;

        /**
         * @brief checks if lifetime is singleton
         */
        bool isSingleton() const;
        /**
         * @brief checks if lifetime is scoped
         */
        bool isScoped() const;
        /**
         * @brief checks if lifetime is transient
         */
        bool isTransient() const;
        /**
         * @brief checks if lifetime is given type
         */
        bool is(Type type) const;

        bool operator>(const ServiceLifeTime &scope) const;
        bool operator<(const ServiceLifeTime &scope) const;
        bool operator>=(const ServiceLifeTime &scope) const;
        bool operator<=(const ServiceLifeTime &scope) const;
        bool operator!=(const ServiceLifeTime &scope) const;
        bool operator==(const ServiceLifeTime &scope) const;
    };

} // namespace sb::di

#ifdef SEVEN_BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Impl/ServiceLifeTime.hpp"
#endif