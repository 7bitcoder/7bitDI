#pragma once

#include <memory>
#include <typeindex>

#include "SevenBit/_Internal/TypeId.hpp"

namespace sb
{
    struct IServiceHolder
    {
        using Ptr = std::unique_ptr<IServiceHolder>;

        virtual void *getService() = 0;

        virtual TypeId getServiceTypeId() const = 0;

        virtual TypeId getServiceInterfaceTypeId() const = 0;

        virtual bool isValid() const = 0;

        virtual ~IServiceHolder() = default;
    };
} // namespace sb