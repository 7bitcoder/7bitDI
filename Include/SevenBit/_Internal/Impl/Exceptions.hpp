#pragma once

#include <string>

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/Exceptions.hpp"

namespace sb
{
    INLINE InjectorException::InjectorException(const std::string &error) : std::runtime_error{error} {}

    INLINE NullPointnerException::NullPointnerException(const std::string &why) : InjectorException{why} {}

    INLINE ForbiddenServiceFactoryException::ForbiddenServiceFactoryException(TypeId typeId, const std::string &reason)
        : InjectorException{std::string{"Forbidden service: '"} + typeId.name() + "' factory, reason: " + reason + "."}
    {
    }

    INLINE CannotMoveOutServiceException::CannotMoveOutServiceException(TypeId typeId, const std::string &reason)
        : InjectorException{std::string{"Cannot move out service: '"} + typeId.name() + "', reason: " + reason + "."}
    {
    }

    INLINE InvalidServiceException::InvalidServiceException(TypeId typeId, const std::string &reason)
        : InjectorException{std::string{"Service: '"} + typeId.name() + "' is invalid, reason: " + reason + "."}
    {
    }

    INLINE ServiceNotRegisteredException::ServiceNotRegisteredException(TypeId typeIndex)
        : InjectorException{std::string{"Service: '"} + typeIndex.name() + "' was not registered."}
    {
    }

    INLINE NotTransientException::NotTransientException(TypeId typeIndex)
        : InjectorException{std::string{"Service: '"} + typeIndex.name() +
                            "' is not registered as transient and could not be created."}
    {
    }

    INLINE TransientForbidException::TransientForbidException(TypeId typeIndex)
        : InjectorException{std::string{"Service: '"} + typeIndex.name() +
                            "' is registered as transient and cannot be saved in DI."}
    {
    }

    INLINE CircularDependencyException::CircularDependencyException(TypeId typeIndex)
        : InjectorException{std::string{"Circular dependency detected while creating service: '"} + typeIndex.name() +
                            "'."}
    {
    }

    INLINE ServiceAlreadyRegisteredException::ServiceAlreadyRegisteredException(TypeId typeIndex)
        : InjectorException{std::string{"Service: '"} + typeIndex.name() + "' was already registered."}
    {
    }

    INLINE ServiceLifeTimeMissmatchException::ServiceLifeTimeMissmatchException(TypeId typeIndex, TypeId interface)
        : InjectorException{std::string{"Service: '"} + typeIndex.name() +
                            "' should have same scope as other services implementing this interface '" +
                            interface.name() + "' that are already registered."}
    {
    }
} // namespace sb