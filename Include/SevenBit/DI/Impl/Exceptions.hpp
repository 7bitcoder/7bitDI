#pragma once

#include <string>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/Format.hpp"
#include "SevenBit/DI/Exceptions.hpp"

namespace sb::di
{
    INLINE InjectorException::InjectorException(const std::string &error) : std::runtime_error{error} {}

    INLINE NullPointerException::NullPointerException(const std::string &why) : InjectorException{why} {}

    INLINE InvalidServiceException::InvalidServiceException()
        : InjectorException{std::string{"Service is not in valid state"}}
    {
    }

    INLINE InvalidServiceException::InvalidServiceException(const TypeId typeId)
        : InjectorException{details::Format::fmt("Service: '{}' is in not valid state.", typeId.name())}
    {
    }

    INLINE CannotReleaseServiceException::CannotReleaseServiceException(const TypeId typeId, const std::string &reason)
        : InjectorException{
              details::Format::fmt("Cannot release ownership of service: '{}', reason: {}.", typeId.name(), reason)}
    {
    }

    INLINE CannotMoveOutServiceException::CannotMoveOutServiceException(const TypeId typeId, const std::string &reason)
        : InjectorException{details::Format::fmt("Cannot move out service: '{}', reason: {}.", typeId.name(), reason)}
    {
    }

    INLINE ServiceNotFoundException::ServiceNotFoundException(const TypeId typeId, const std::string &reason)
        : InjectorException{details::Format::fmt("Service: '{}' was not found, reason: {}.", typeId.name(), reason)}
    {
    }

    INLINE CircularDependencyException::CircularDependencyException(const TypeId typeId)
        : InjectorException{
              details::Format::fmt("Circular dependency detected while creating service: '{}'.", typeId.name())}
    {
    }

    INLINE ServiceAlreadyRegisteredException::ServiceAlreadyRegisteredException(const TypeId typeId)
        : InjectorException{details::Format::fmt("Service: '{}' was already registered.", typeId.name())}
    {
    }

    INLINE ServiceAliasMismatchException::ServiceAliasMismatchException(const TypeId typeId, const TypeId interface,
                                                                        const bool shouldBeAlias)
        : InjectorException{details::Format::fmt("Service: '{}' should {} alias as other services implementing this "
                                                 "interface '{}' that are already registered.",
                                                 typeId.name(), (shouldBeAlias ? "be" : "not be"), interface.name())}
    {
    }

    INLINE ServiceLifeTimeMismatchException::ServiceLifeTimeMismatchException(const TypeId typeId,
                                                                              const TypeId interface)
        : InjectorException{details::Format::fmt(
              "Service: '{}' should have same scope as other services implementing this interface '{}'.", typeId.name(),
              interface.name())}
    {
    }
} // namespace sb::di
