#pragma once

#include <memory>

#include "SevenBit/_Internal/ServiceCreators.hpp"
#include "SevenBit/_Internal/TypeId.hpp"

namespace sb
{
    struct IServiceCreatorsProvider
    {
        using Ptr = std::unique_ptr<IServiceCreatorsProvider>;

        virtual const ServiceCreators *getCreators(TypeId typeId) const = 0;

        virtual const IServiceCreator *getMainCreator(TypeId typeId) const = 0;

        virtual ~IServiceCreatorsProvider() = default;
    };
} // namespace sb
