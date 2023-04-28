#pragma once

#include <memory>

#include "SevenBit/_Internal/IServiceHolder.hpp"
#include "SevenBit/_Internal/ServiceLifeTime.hpp"

namespace sb
{
    class ServiceProvider;

    struct IServiceCreator
    {
        using Ptr = std::unique_ptr<IServiceCreator>;

        virtual const ServiceLifeTime &getServiceScope() const = 0;

        virtual TypeId getServiceTypeId() const = 0;

        virtual TypeId getServiceInterfaceTypeId() const = 0;

        virtual IServiceHolder::Ptr create(ServiceProvider &serviceProvider) const = 0;

        virtual ~IServiceCreator() {}
    };

} // namespace sb