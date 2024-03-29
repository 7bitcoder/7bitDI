#pragma once

#include <memory>
#include <optional>
#include <string>
#include <string_view>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/TypeId.hpp"

namespace sb::di::details
{
    class ServiceId
    {
        TypeId _typeId;
        std::string_view _key{};

      public:
        explicit ServiceId(const TypeId typeId) : _typeId(typeId) {}
        ServiceId(const TypeId typeId, const std::string_view key) : _typeId(typeId), _key(key) {}
        ServiceId(const TypeId typeId, const std::string *key) : _typeId(typeId), _key(key ? *key : std::string_view{})
        {
        }

        ServiceId(const ServiceId &) = default;
        ServiceId(ServiceId &&) noexcept = default;
        ServiceId &operator=(const ServiceId &) = default;
        ServiceId &operator=(ServiceId &&) noexcept = default;

        [[nodiscard]] const TypeId &getTypeId() const { return _typeId; }

        [[nodiscard]] const std::string_view &getKey() const { return _key; }

        friend bool operator==(const ServiceId &l, const ServiceId &r)
        {
            return l.getTypeId() == r.getTypeId() && l.getKey() == r.getKey();
        }

        friend bool operator!=(const ServiceId &l, const ServiceId &r) { return !(l == r); }
    };

} // namespace sb::di::details

template <> struct std::hash<sb::di::details::ServiceId>
{
    std::size_t operator()(const sb::di::details::ServiceId &s) const noexcept
    {
        const std::size_t h1 = std::hash<sb::di::TypeId>{}(s.getTypeId());
        const std::size_t h2 = std::hash<std::string_view>{}(s.getKey());
        return h1 ^ (h2 << 1);
    }
};
