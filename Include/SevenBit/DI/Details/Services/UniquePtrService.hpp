#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/IServiceInstance.hpp"

namespace sb::di::details
{
    template <class T> class UniquePtrService final : public IServiceInstance
    {
        std::unique_ptr<T> _service;

      public:
        using Ptr = std::unique_ptr<UniquePtrService>;

        explicit UniquePtrService(std::unique_ptr<T> service) : _service(std::move(service)) {}
        template <class... Args>
        explicit UniquePtrService(Args &&...args) : _service(std::make_unique<T>(std::forward<Args>(args)...))
        {
        }

        UniquePtrService(const UniquePtrService &) = delete;
        UniquePtrService(UniquePtrService &&) = default;
        UniquePtrService &operator=(const UniquePtrService &) = delete;
        UniquePtrService &operator=(UniquePtrService &&) = default;

        [[nodiscard]] void *get() const override { return _service.get(); }

        void *getForMoveOut() override { return get(); }

        void *release() override { return _service.release(); }

        [[nodiscard]] TypeId getTypeId() const override { return typeid(T); }

        [[nodiscard]] bool isValid() const override { return bool{_service}; }
    };
} // namespace sb::di::details
