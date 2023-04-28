#pragma once

#include <memory>

#include "SevenBit/_Internal/IServiceHolder.hpp"
#include "SevenBit/_Internal/ServiceLifeTime.hpp"

namespace sb
{
    class ServiceProvider;

    struct IServiceAccessor
    {
        using Ptr = std::unique_ptr<IServiceAccessor>;

        virtual void *get(ServiceProvider &serviceProvider) const = 0;

        virtual ~IServiceAccessor() {}
    };

} // namespace sb