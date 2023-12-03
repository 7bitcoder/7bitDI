#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceInstance.hpp"

namespace sb::di::details
{
    template <class T> class ServiceOwner final : public IServiceInstance
    {
      private:
        T _service;

      public:
        template <class... Args> explicit ServiceOwner(Args &&...args) : _service(std::forward<Args>(args)...) {}

        ServiceOwner(const ServiceOwner &) = delete;
        ServiceOwner(ServiceOwner &&) = delete;
        ServiceOwner &operator=(const ServiceOwner &) = delete;
        ServiceOwner &operator=(ServiceOwner &&) = delete;

        void *get() const final { return const_cast<T *>(&_service); }

        void *release() { throw CannotMoveOutServiceException(getTypeId(), "Service cannot be moved out"); }

        TypeId getTypeId() const final { return typeid(T); }

        bool isValid() const final { return true; }

        operator bool() const { return isValid(); }
    };
} // namespace sb::di::details
