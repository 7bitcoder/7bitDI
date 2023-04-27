#pragma once

#include <memory>

#include "SevenBit/_Internal/IServiceHolder.hpp"

namespace sb
{
    template <class I, class T> class ServiceOwner final : public IServiceHolder
    {
      private:
        std::unique_ptr<T> _service;

      public:
        using Ptr = std::unique_ptr<ServiceOwner<I, T>>;

        ServiceOwner(std::unique_ptr<T> service) : _service(std::move(service))
        {
            static_assert(std::is_base_of_v<I, T>, "Type T must inherit from I");
        }

        ServiceOwner(const ServiceOwner &) = delete;
        ServiceOwner &operator=(const ServiceOwner &) = delete;

        void *getService() final { return _service.get(); }

        TypeId getServiceTypeId() const final { return typeid(T); }

        TypeId getServiceInterfaceTypeId() const final { return typeid(I); }

        std::unique_ptr<T> moveOutService() { return std::move(_service); }

        bool isValid() const final { return bool{_service}; }

        operator bool() const { return isValid(); }
    };
} // namespace sb