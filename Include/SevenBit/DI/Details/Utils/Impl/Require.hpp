#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/Require.hpp"
#include "SevenBit/DI/Exceptions.hpp"

namespace sb::di::details::utils
{
    INLINE IServiceInstance::Ptr Require::validInstanceAndGet(IServiceInstance::Ptr &&instance)
    {
        validInstance(instance.get());
        return std::move(instance);
    }

    INLINE IServiceInstance &Require::validInstanceAndGet(IServiceInstance *instance)
    {
        validInstance(instance);
        return *instance;
    }

    INLINE void Require::validInstance(const IServiceInstance::Ptr &instance) { validInstance(instance.get()); }

    INLINE void Require::validInstance(const IServiceInstance *instance)
    {
        notNull(instance);
        if (!Check::instanceValidity(instance))
        {
            throw InvalidServiceException{instance->getTypeId()};
        }
    }
} // namespace sb::di::details::utils
