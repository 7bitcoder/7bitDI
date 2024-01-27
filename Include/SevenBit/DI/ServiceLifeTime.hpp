#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/Require.hpp"

namespace sb::di
{
    class ServiceLifeTime
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
        constexpr static ServiceLifeTime singleton() { return ServiceLifeTime{Singleton}; }
        /**
         * @brief creates scoped service lifetime
         */
        constexpr static ServiceLifeTime scoped() { return ServiceLifeTime{Scoped}; }
        /**
         * @brief creates transient service lifetime
         */
        constexpr static ServiceLifeTime transient() { return ServiceLifeTime{Transient}; }
        constexpr static ServiceLifeTime alias() { return ServiceLifeTime{Alias}; }

        /**
         * @brief Construct a new Service Life Time object with specified type
         */
        constexpr explicit ServiceLifeTime(const Type type) : _type(details::utils::Require::validEnumAndGet(type)) {}

        constexpr ServiceLifeTime(ServiceLifeTime &&) = default;
        constexpr ServiceLifeTime(const ServiceLifeTime &) = default;

        constexpr ServiceLifeTime &operator=(ServiceLifeTime &&) = default;
        constexpr ServiceLifeTime &operator=(const ServiceLifeTime &) = default;

        /**
         * @brief checks if lifetime is given type
         */
        [[nodiscard]] constexpr bool is(const Type type) const { return *this == type; }
        /**
         * @brief checks if lifetime is singleton
         */
        [[nodiscard]] constexpr bool isSingleton() const { return is(Singleton); }
        /**
         * @brief checks if lifetime is scoped
         */
        [[nodiscard]] constexpr bool isScoped() const { return is(Scoped); }
        /**
         * @brief checks if lifetime is transient
         */
        [[nodiscard]] constexpr bool isTransient() const { return is(Transient); }
        /**
         * @brief checks if lifetime is alias
         */
        [[nodiscard]] constexpr bool isAlias() const { return is(Alias); }

        template <class... TServiceLifeTime> [[nodiscard]] constexpr bool isAny(TServiceLifeTime... types) const
        {
            return ((*this == types) || ...);
        }

        template <class... TServiceLifeTime> [[nodiscard]] constexpr bool isNot(TServiceLifeTime... types) const
        {
            return ((*this != types) && ...);
        }

        constexpr bool operator!=(const ServiceLifeTime &lifeTime) const { return _type != lifeTime._type; }
        constexpr bool operator==(const ServiceLifeTime &lifeTime) const { return _type == lifeTime._type; }

        constexpr bool operator!=(const Type type) const { return _type != type; }
        constexpr bool operator==(const Type type) const { return _type == type; }
    };
} // namespace sb::di
