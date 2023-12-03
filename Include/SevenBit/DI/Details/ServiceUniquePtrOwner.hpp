#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/IServiceInstance.hpp"

namespace sb::di::details
{
    template <class T> class ServiceUniquePtrOwner final : public IServiceInstance
    {
      private:
        std::unique_ptr<T> _service;

      public:
        using Ptr = std::unique_ptr<ServiceUniquePtrOwner<T>>;

        ServiceUniquePtrOwner(std::unique_ptr<T> service) : _service(std::move(service)) {}

        ServiceUniquePtrOwner(const ServiceUniquePtrOwner &) = delete;
        ServiceUniquePtrOwner(ServiceUniquePtrOwner &&) = default;
        ServiceUniquePtrOwner &operator=(const ServiceUniquePtrOwner &) = delete;
        ServiceUniquePtrOwner &operator=(ServiceUniquePtrOwner &&) = default;

        void *get() const final { return _service.get(); }

        void *release() { return _service.release(); }

        TypeId getTypeId() const final { return typeid(T); }

        bool isValid() const final { return bool{_service}; }

        operator bool() const { return isValid(); }
    };
} // namespace sb::di::details
