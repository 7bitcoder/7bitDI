#pragma once

#include <memory>

#include "SevenBit/_Internal/TypeId.hpp"

namespace sb
{
    struct IServiceInstance
    {
        using Ptr = std::unique_ptr<IServiceInstance>;

        virtual void *get() = 0;

        virtual void *moveOut() = 0;

        virtual TypeId getTypeId() const = 0;

        virtual bool isValid() const = 0;

        virtual ~IServiceInstance() = default;
    };
} // namespace sb