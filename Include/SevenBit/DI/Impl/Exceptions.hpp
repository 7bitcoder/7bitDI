#pragma once

#include <string>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Exceptions.hpp"

namespace sb::di
{
    INLINE InjectorException::InjectorException(const std::string &error) : std::runtime_error{error} {}

    INLINE NullPointerException::NullPointerException(const std::string &why) : InjectorException{why} {}

    INLINE InvalidServiceException::InvalidServiceException(TypeId typeId)
        : InjectorException{std::string{"Service: '"} + typeId.name() + "' is in not valid state."}
    {
    }

    INLINE ForbiddenServiceFactoryException::ForbiddenServiceFactoryException(TypeId typeId, const std::string &reason)
        : InjectorException{std::string{"Forbidden instanceValidity: '"} + typeId.name() +
                            "' factory, reason: " + reason + "."}
    {
    }

    INLINE CannotReleaseServiceException::CannotReleaseServiceException(TypeId typeId, const std::string &reason)
        : InjectorException{std::string{"Cannot release ownership of instanceValidity: '"} + typeId.name() +
                            "', reason: " + reason + "."}
    {
    }

    INLINE CannotMoveOutServiceException::CannotMoveOutServiceException(TypeId typeId, const std::string &reason)
        : InjectorException{std::string{"Cannot move out instanceValidity: '"} + typeId.name() +
                            "', reason: " + reason + "."}
    {
    }

    INLINE ServiceNotFoundException::ServiceNotFoundException(TypeId typeIndex, const std::string &reason)
        : InjectorException{std::string{"Service: '"} + typeIndex.name() + "' was not found, reason: " + reason + "."}
    {
    }

    INLINE CircularDependencyException::CircularDependencyException(TypeId typeIndex)
        : InjectorException{std::string{"Circular dependency detected while creating instanceValidity: '"} +
                            typeIndex.name() + "'."}
    {
    }

    INLINE ServiceAlreadyRegisteredException::ServiceAlreadyRegisteredException(TypeId typeIndex)
        : InjectorException{std::string{"Service: '"} + typeIndex.name() + "' was already registered."}
    {
    }

    INLINE ServiceBaseTypeMismatchException::ServiceBaseTypeMismatchException(TypeId typeIndex, TypeId interface)
        : InjectorException{std::string{"Service: '"} + typeIndex.name() + "' should implement this base type '" +
                            interface.name() + "' ."}
    {
    }

    INLINE ServiceLifeTimeMismatchException::ServiceLifeTimeMismatchException(TypeId typeIndex, TypeId interface)
        : InjectorException{std::string{"Service: '"} + typeIndex.name() +
                            "' should have same scope as other services implementing this interface '" +
                            interface.name() + "' that are already registered."}
    {
    }
} // namespace sb::di
