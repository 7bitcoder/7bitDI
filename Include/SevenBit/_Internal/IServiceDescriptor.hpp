#pragma once
#include <memory>

#include "SevenBit/_Internal/Exceptions.hpp"
#include "SevenBit/_Internal/IServiceCreator.hpp"
#include "SevenBit/_Internal/ServiceLifeTime.hpp"
#include "SevenBit/_Internal/TypeId.hpp"

namespace sb
{
    struct IServiceDescriptor
    {
        using Ptr = std::unique_ptr<IServiceDescriptor>;

        virtual const ServiceLifeTime &getLifeTime() const = 0;

        virtual TypeId getServiceTypeId() const = 0;

        virtual TypeId getImplementationTypeId() const = 0;

        virtual IServiceCreator &getImplementationCreator() const = 0;

        virtual void *getImplementationInstance() const = 0;

        virtual ~IServiceDescriptor() = default;
    };

} // namespace sb
