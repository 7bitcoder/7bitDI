#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/Require.hpp"
#include "SevenBit/DI/Exceptions.hpp"

namespace sb::di::details::utils
{
    INLINE ServiceInstance::Ptr Require::validInstanceAndGet(ServiceInstance::Ptr &&instance)
    {
        validInstance(instance.get());
        return std::move(instance);
    }

    INLINE ServiceInstance &Require::validInstanceAndGet(ServiceInstance *instance)
    {
        validInstance(instance);
        return *instance;
    }

    INLINE void Require::validInstance(const ServiceInstance::Ptr &instance) { validInstance(instance.get()); }

    INLINE void Require::validInstance(const ServiceInstance *instance)
    {
        notNull(instance);
        if (!Check::instanceValidity(instance))
        {
            throw InvalidServiceException{instance->getTypeId()};
        }
    }
} // namespace sb::di::details::utils
