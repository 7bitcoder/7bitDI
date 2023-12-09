#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/TypeId.hpp"

namespace sb::di
{
    /**
     * @brief Interface for all service instances
     */
    struct IServiceInstance
    {
        using Ptr = std::unique_ptr<IServiceInstance>;

        [[nodiscard]] virtual void *get() const = 0;

        [[nodiscard]] virtual void *getForMoveOut() = 0;

        [[nodiscard]] virtual void *release() = 0;

        [[nodiscard]] virtual TypeId getTypeId() const = 0;

        [[nodiscard]] virtual bool isValid() const = 0;

        explicit operator bool() const { return isValid(); }

        template <class T> [[nodiscard]] T *getAs() const { return static_cast<T *>(get()); }

        template <class T> T *releaseAs() { return static_cast<T *>(release()); }

        template <class T> std::unique_ptr<T> moveOutAsUniquePtr() { return std::unique_ptr<T>{releaseAs<T>()}; }

        template <class T> T &&moveOutAs() { return std::move(*static_cast<T *>(getForMoveOut())); }

        virtual ~IServiceInstance() = default;
    };
} // namespace sb::di
