#pragma once

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils.hpp"
#include "SevenBit/DI/Exceptions.hpp"

namespace sb::di::details::utils
{
    INLINE IServiceInstance::Ptr &&Assert::serviceAndMove(IServiceInstance::Ptr &&service)
    {
        Assert::service(service.get());
        return std::move(service);
    }

    INLINE IServiceInstance::Ptr Assert::serviceAndGet(IServiceInstance::Ptr service)
    {
        Assert::service(service.get());
        return std::move(service);
    }

    INLINE IServiceInstance *Assert::serviceAndGet(IServiceInstance *service)
    {
        Assert::service(service);
        return service;
    }

    INLINE void Assert::service(const IServiceInstance::Ptr &service) { Assert::service(service.get()); }

    INLINE void Assert::service(const IServiceInstance *service)
    {
        Assert::ptr(service);
        if (!service->isValid())
        {
            throw InvalidServiceException{service->getTypeId(), ""};
        }
    }
} // namespace sb::di::details::utils
