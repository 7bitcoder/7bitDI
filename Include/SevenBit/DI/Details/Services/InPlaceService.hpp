#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceInstance.hpp"

namespace sb::di::details::services
{
    template <class T> class InPlaceService final : public ServiceInstance
    {
        T _service;

      public:
        template <class... Args> explicit InPlaceService(Args &&...args) : _service(std::forward<Args>(args)...) {}

        InPlaceService(const InPlaceService &) = delete;
        InPlaceService(InPlaceService &&) = delete;
        InPlaceService &operator=(const InPlaceService &) = delete;
        InPlaceService &operator=(InPlaceService &&) = delete;

        [[nodiscard]] void *get() const override { return const_cast<T *>(&_service); }

        void *getForMoveOut() override { return get(); }

        void *release() override
        {
            throw CannotReleaseServiceException(getTypeId(), "In place service ownership cannot be released");
        }

        [[nodiscard]] TypeId getTypeId() const override { return typeid(T); }

        [[nodiscard]] bool isValid() const override { return true; }
    };
} // namespace sb::di::details::services
