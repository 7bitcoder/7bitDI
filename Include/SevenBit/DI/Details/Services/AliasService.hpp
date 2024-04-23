#pragma once

#include <memory>

#include <SevenBit/DI/LibraryConfig.hpp>

#include <SevenBit/DI/Exceptions.hpp>
#include <SevenBit/DI/IServiceInstance.hpp>

namespace sb::di::details
{
    class AliasService final : public IServiceInstance
    {
        void *_service = nullptr;
        TypeId _serviceTypeId;

      public:
        using Ptr = std::unique_ptr<AliasService>;

        explicit AliasService(void *service, const TypeId serviceTypeId)
            : _service(service), _serviceTypeId(serviceTypeId)
        {
        }

        AliasService(const AliasService &) = default;
        AliasService(AliasService &&) = default;

        AliasService &operator=(const AliasService &) = default;
        AliasService &operator=(AliasService &&) = default;

        [[nodiscard]] void *get() const override { return _service; }

        void *getForMoveOut() override
        {
            throw CannotMoveOutServiceException{getTypeId(), "Alias service cannot be moved out"};
        }

        void *release() override
        {
            throw CannotReleaseServiceException{getTypeId(), "Alias service ownership cannot be released"};
        }

        [[nodiscard]] TypeId getTypeId() const override { return _serviceTypeId; }

        [[nodiscard]] bool isValid() const override { return _service != nullptr; }
    };
} // namespace sb::di::details
