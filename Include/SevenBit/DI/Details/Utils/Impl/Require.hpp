#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/Require.hpp"

namespace sb::di::details::utils
{
    INLINE ServiceInstance *Require::validInstanceAndGet(ServiceInstance *instance)
    {
        validInstance(instance);
        return instance;
    }

    INLINE ServiceInstance &&Require::validInstanceAndGet(ServiceInstance &&instance)
    {
        validInstance(instance);
        return std::move(instance);
    }

    INLINE void Require::validInstance(const ServiceInstance *instance)
    {
        notNull(instance);
        validInstance(*instance);
    }

    INLINE void Require::validInstance(const ServiceInstance &instance)
    {
        if (!instance.isValid())
        {
            if (instance.tryGetImplementation())
            {
                throw InvalidServiceException{instance.tryGetImplementation()->getTypeId()};
            }
            throw InvalidServiceException{};
        }
    }
} // namespace sb::di::details::utils
