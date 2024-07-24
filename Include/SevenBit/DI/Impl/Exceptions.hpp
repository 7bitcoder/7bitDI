#pragma once

#include <string>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/String.hpp"
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
        : InjectorException{
              details::String::join("Service:", details::String::quote(typeId.name()), "is in not valid state")}
    {
    }

    INLINE CannotReleaseServiceException::CannotReleaseServiceException(const TypeId typeId, const std::string &reason)
        : InjectorException{details::String::join(
              "Cannot release ownership of service:", details::String::quote(typeId.name()), ", reason:", reason)}
    {
    }

    INLINE CannotMoveOutServiceException::CannotMoveOutServiceException(const TypeId typeId, const std::string &reason)
        : InjectorException{details::String::join("Cannot move out service:", details::String::quote(typeId.name()),
                                                  ", reason:", reason)}
    {
    }

    INLINE ServiceNotFoundException::ServiceNotFoundException(const TypeId typeId, const std::string &reason)
        : InjectorException{details::String::join("Service:", details::String::quote(typeId.name()),
                                                  "was not found, reason:", reason)}
    {
    }

    INLINE CircularDependencyException::CircularDependencyException(const TypeId typeId)
        : InjectorException{details::String::join("Circular dependency detected while creating service:",
                                                  details::String::quote(typeId.name()))}
    {
    }

    INLINE ServiceAlreadyRegisteredException::ServiceAlreadyRegisteredException(const TypeId typeId)
        : InjectorException{
              details::String::join("Service:", details::String::quote(typeId.name()), "was already registered")}
    {
    }

    INLINE ServiceAliasMismatchException::ServiceAliasMismatchException(const TypeId typeId, const TypeId interface,
                                                                        const bool shouldBeAlias)
        : InjectorException{details::String::join(
              "Service:", details::String::quote(typeId.name()), "should", (shouldBeAlias ? "be" : "not be"),
              "alias as other services implementing this interface", details::String::quote(interface.name()),
              "that are already registered")}
    {
    }

    INLINE ServiceLifeTimeMismatchException::ServiceLifeTimeMismatchException(const TypeId typeId,
                                                                              const TypeId interface)
        : InjectorException{
              details::String::join("Service:", details::String::quote(typeId.name()),
                                    "should have same scope as other services implementing this interface",
                                    details::String::quote(interface.name()), "that are already registered")}
    {
    }
} // namespace sb::di
