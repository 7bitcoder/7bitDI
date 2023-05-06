#pragma once

#include <memory>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/TypeId.hpp"

namespace sb::di
{
    struct IServiceInstance
    {
        using Ptr = std::unique_ptr<IServiceInstance>;

        virtual void *get() const = 0;

        template <class T> T *getAs() const { return static_cast<T *>(get()); };

        virtual void *moveOut() = 0;

        template <class T> T *moveOutAs() { return static_cast<T *>(moveOut()); };

        virtual TypeId getTypeId() const = 0;

        virtual bool isValid() const = 0;

        operator bool() const { return isValid(); }

        virtual ~IServiceInstance() = default;
    };
} // namespace sb::di