#pragma once

#include <memory>

#include "SevenBit/_Internal/IServiceHolder.hpp"
#include "SevenBit/_Internal/IServiceInstance.hpp"

namespace sb
{
    template <class T> class ServiceOwner2 final : public IServiceInstance
    {
      private:
        std::unique_ptr<T> _service;

      public:
        using Ptr = std::unique_ptr<ServiceOwner2<T>>;

        ServiceOwner2(std::unique_ptr<T> service) : _service(std::move(service)) {}

        ServiceOwner2(const ServiceOwner2 &) = delete;
        ServiceOwner2(ServiceOwner2 &&) = default;
        ServiceOwner2 &operator=(const ServiceOwner2 &) = delete;
        ServiceOwner2 &operator=(ServiceOwner2 &&) = default;

        void *get() final { return _service.get(); }

        void *moveOut() { return _service.release(); }

        TypeId getTypeId() const final { return typeid(T); }

        bool isValid() const final { return bool{_service}; }

        operator bool() const { return isValid(); }
    };
} // namespace sb