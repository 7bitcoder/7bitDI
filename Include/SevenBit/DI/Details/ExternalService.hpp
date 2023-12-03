#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"

namespace sb::di::details
{
    template <class T> class ExternalService final : public IServiceInstance
    {
      private:
        T *_service = nullptr;

      public:
        using Ptr = std::unique_ptr<ExternalService<T>>;

        explicit ExternalService(T *service) : _service(service) {}

        ExternalService(const ExternalService &) = default;
        ExternalService(ExternalService &&) = default;

        ExternalService &operator=(const ExternalService &) = default;
        ExternalService &operator=(ExternalService &&) = default;

        void *get() const final { return _service; }

        void *release() { throw CannotMoveOutServiceException{getTypeId(), "External service cannot be moved out."}; }

        TypeId getTypeId() const final { return typeid(T); }

        bool isValid() const final { return _service != nullptr; }

        operator bool() const { return isValid(); }
    };
} // namespace sb::di::details
