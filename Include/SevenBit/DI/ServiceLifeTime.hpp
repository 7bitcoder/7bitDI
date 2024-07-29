#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/Require.hpp"

namespace sb::di
{
    class ServiceLifeTime
    {
      public:
        /**
         * @brief 3 types of available service lifetimes
         */
        enum class Type
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
        constexpr static ServiceLifeTime singleton() { return ServiceLifeTime{Type::Singleton}; }
        /**
         * @brief creates scoped service lifetime
         */
        constexpr static ServiceLifeTime scoped() { return ServiceLifeTime{Type::Scoped}; }
        /**
         * @brief creates transient service lifetime
         */
        constexpr static ServiceLifeTime transient() { return ServiceLifeTime{Type::Transient}; }

        /**
         * @brief Construct a new Service Life Time object with specified type
         */
        constexpr explicit ServiceLifeTime(const Type type) : _type(details::Require::validEnumAndGet(type)) {}

        constexpr ServiceLifeTime(ServiceLifeTime &&) = default;
        constexpr ServiceLifeTime(const ServiceLifeTime &) = default;

        constexpr ServiceLifeTime &operator=(ServiceLifeTime &&) = default;
        constexpr ServiceLifeTime &operator=(const ServiceLifeTime &) = default;

        /**
         * @brief checks if lifetime is given type
         */
        [[nodiscard]] constexpr bool is(const Type type) const { return _type == type; }
        /**
         * @brief checks if lifetime is singleton
         */
        [[nodiscard]] constexpr bool isSingleton() const { return is(Type::Singleton); }
        /**
         * @brief checks if lifetime is scoped
         */
        [[nodiscard]] constexpr bool isScoped() const { return is(Type::Scoped); }
        /**
         * @brief checks if lifetime is transient
         */
        [[nodiscard]] constexpr bool isTransient() const { return is(Type::Transient); }

        constexpr bool operator!=(const ServiceLifeTime &lifeTime) const { return _type != lifeTime._type; }
        constexpr bool operator==(const ServiceLifeTime &lifeTime) const { return _type == lifeTime._type; }

        [[nodiscard]] std::string_view toString() const
        {
            switch (_type)
            {
            case Type::Singleton:
                return "Singleton";
            case Type::Scoped:
                return "Scoped";
            case Type::Transient:
                return "Transient";
            default:
                return "Unknown";
            }
        }
    };
} // namespace sb::di
