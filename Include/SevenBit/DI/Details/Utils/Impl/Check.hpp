#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/Check.hpp"
#include "SevenBit/DI/Exceptions.hpp"

namespace sb::di::details::utils
{
    INLINE IServiceInstance::Ptr &&Check::serviceAndMove(IServiceInstance::Ptr &&service)
    {
        Check::service(service.get());
        return std::move(service);
    }

    INLINE IServiceInstance::Ptr Check::serviceAndGet(IServiceInstance::Ptr service)
    {
        Check::service(service.get());
        return std::move(service);
    }

    INLINE IServiceInstance *Check::serviceAndGet(IServiceInstance *service)
    {
        Check::service(service);
        return service;
    }

    INLINE void Check::service(const IServiceInstance::Ptr &service) { Check::service(service.get()); }

    INLINE void Check::service(const IServiceInstance *service)
    {
        Check::notNull(service);
        if (!service->isValid())
        {
            throw InvalidServiceException{service->getTypeId(), ""};
        }
    }
} // namespace sb::di::details::utils
