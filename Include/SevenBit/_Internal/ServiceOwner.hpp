#pragma once

#include <memory>

#include "SevenBit/_Internal/IServiceInstance.hpp"

namespace sb
{
    template <class T> class ServiceOwner final : public IServiceInstance
    {
      private:
        std::unique_ptr<T> _service;

      public:
        using Ptr = std::unique_ptr<ServiceOwner<T>>;

        ServiceOwner(std::unique_ptr<T> service) : _service(std::move(service)) {}

        ServiceOwner(const ServiceOwner &) = delete;
        ServiceOwner(ServiceOwner &&) = default;
        ServiceOwner &operator=(const ServiceOwner &) = delete;
        ServiceOwner &operator=(ServiceOwner &&) = default;

        void *get() final { return _service.get(); }

        void *moveOut() { return _service.release(); }

        TypeId getTypeId() const final { return typeid(T); }

        bool isValid() const final { return bool{_service}; }

        operator bool() const { return isValid(); }
    };
} // namespace sb