#pragma once

#include <cstddef>
#include <exception>
#include <forward_list>
#include <list>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "SevenBit/_Internal/Exceptions.hpp"
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
