#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"

namespace sb::di::details::services
{
    template <class T> class ExternalService final : public IServiceInstance
    {
        T *_service = nullptr;

      public:
        using Ptr = std::unique_ptr<ExternalService<T>>;

        explicit ExternalService(T *service) : _service(service) {}

        ExternalService(const ExternalService &) = default;
        ExternalService(ExternalService &&) = default;

        ExternalService &operator=(const ExternalService &) = default;
        ExternalService &operator=(ExternalService &&) = default;

        [[nodiscard]] void *get() const override { return _service; }

        void *getForMoveOut() override
        {
            throw CannotMoveOutServiceException{getTypeId(), "External service cannot be moved out"};
        }

        void *release() override
        {
            throw CannotReleaseServiceException{getTypeId(), "External service ownership cannot be released"};
        }

        [[nodiscard]] TypeId getTypeId() const override { return typeid(T); }

        [[nodiscard]] bool isValid() const override { return _service != nullptr; }
    };
} // namespace sb::di::details::services
