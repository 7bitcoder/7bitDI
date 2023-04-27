#pragma once

namespace sb
{
    struct ServiceScope
    {
      public:
        enum Type
        {
            Singeleton,
            Scoped,
            Transient
        };

      private:
        Type type;

      public:
        static ServiceScope singeleton() { return {Type::Singeleton}; }
        static ServiceScope scoped() { return {Type::Scoped}; }
        static ServiceScope transient() { return {Type::Transient}; }

        ServiceScope(Type type) : type(type) {}

        ServiceScope(const ServiceScope &) = default;
        ServiceScope &operator=(const ServiceScope &) = default;

        bool isSingeleton() const { return type == Type::Singeleton; }
        bool isScoped() const { return type == Type::Scoped; }
        bool isTransient() const { return type == Type::Transient; }

        bool operator==(const ServiceScope &scope) const { return type == scope.type; }
        bool operator!=(const ServiceScope &scope) const { return type != scope.type; }
    };

} // namespace sb