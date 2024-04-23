#pragma once

#include <SevenBit/DI/LibraryConfig.hpp>

#include <SevenBit/DI/Details/Utils/RequireInstance.hpp>
#include <SevenBit/DI/Details/Utils/Require.hpp>

namespace sb::di::details
{
    INLINE ServiceInstance *RequireInstance::validAndGet(ServiceInstance *instance)
    {
        valid(instance);
        return instance;
    }

    INLINE ServiceInstance &&RequireInstance::validAndGet(ServiceInstance &&instance)
    {
        valid(instance);
        return std::move(instance);
    }

    INLINE void RequireInstance::valid(const ServiceInstance *instance)
    {
        Require::notNull(instance);
        valid(*instance);
    }

    INLINE void RequireInstance::valid(const ServiceInstance &instance)
    {
        if (!instance.isValid())
        {
            if (instance.tryGetImplementation())
            {
                throw InvalidServiceException{instance.getImplementation().getTypeId()};
            }
            throw InvalidServiceException{};
        }
    }
} // namespace sb::di::details
