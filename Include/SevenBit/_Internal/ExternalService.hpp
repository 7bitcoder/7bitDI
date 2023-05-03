#pragma once

#include <memory>

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/Exceptions.hpp"
#include "SevenBit/IServiceInstance.hpp"

namespace sb::internal
{
    template <class T> class ExternalService final : public IServiceInstance
    {
      private:
        T *_service = nullptr;

      public:
        using Ptr = std::unique_ptr<ExternalService<T>>;

        ExternalService(T *service) : _service(service) {}

        ExternalService(const ExternalService &) = default;
        ExternalService(ExternalService &&) = default;

        ExternalService &operator=(const ExternalService &) = default;
        ExternalService &operator=(ExternalService &&) = default;

        void *get() final { return _service; }

        void *moveOut() { throw CannotMoveOutServiceException{getTypeId(), "External service cannot be moved out."}; }

        TypeId getTypeId() const final { return typeid(T); }

        bool isValid() const final { return _service != nullptr; }

        operator bool() const { return isValid(); }
    };
} // namespace sb::internal