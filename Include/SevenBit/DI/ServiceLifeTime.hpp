#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

namespace sb::di
{
    class EXPORT ServiceLifeTime
    {
      public:
        /**
         * @brief 4 types of available service lifetimes
         */
        enum Type
        {
            Singleton,
            Scoped,
            Transient,
            Alias,

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
        static ServiceLifeTime alias();

        /**
         * @brief Construct a new Service Life Time object with specified type
         */
        constexpr explicit ServiceLifeTime(Type type);

        ServiceLifeTime(ServiceLifeTime &&) = default;
        ServiceLifeTime(const ServiceLifeTime &) = default;

        ServiceLifeTime &operator=(ServiceLifeTime &&) = default;
        ServiceLifeTime &operator=(const ServiceLifeTime &) = default;

        [[nodiscard]] Type getType() const;

        /**
         * @brief checks if lifetime is given type
         */
        [[nodiscard]] bool is(Type type) const;
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
         * @brief checks if lifetime is alias
         */
        [[nodiscard]] bool isAlias() const;

        template <class... TServiceLifeTime> [[nodiscard]] bool isAny(TServiceLifeTime... types) const
        {
            return ((*this == types) || ...);
        }

        template <class... TServiceLifeTime> [[nodiscard]] bool isNot(TServiceLifeTime... types) const
        {
            return ((*this != types) && ...);
        }

        bool operator!=(const ServiceLifeTime &scope) const;
        bool operator==(const ServiceLifeTime &scope) const;
    };

} // namespace sb::di

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Impl/ServiceLifeTime.hpp"
#endif
