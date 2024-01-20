#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di
{
    class EXPORT ServiceLifeTime
    {
      public:
        /**
         * @brief 3 types of available service lifetimes
         */
        enum Type
        {
            Singleton,
            Scoped,
            Transient,

            Count
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
        explicit ServiceLifeTime(Type type);

        ServiceLifeTime(ServiceLifeTime &&) = default;
        ServiceLifeTime(const ServiceLifeTime &) = default;

        ServiceLifeTime &operator=(ServiceLifeTime &&) = default;
        ServiceLifeTime &operator=(const ServiceLifeTime &) = default;

        /**
         * @brief checks if lifetime is singleton
         */
        [[nodiscard]] bool isSingleton() const;
        /**
         * @brief checks if lifetime is scoped
         */
        [[nodiscard]] bool isScoped() const;
        /**
         * @brief checks if lifetime is transient
         */
        [[nodiscard]] bool isTransient() const;

        /**
         * @brief checks if lifetime is given type
         */
        [[nodiscard]] bool is(Type type) const;

        bool operator!=(const ServiceLifeTime &scope) const;
        bool operator==(const ServiceLifeTime &scope) const;
    };

} // namespace sb::di

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Impl/ServiceLifeTime.hpp"
#endif
