#pragma once

#include <string>

#include <SevenBit/DI/LibraryConfig.hpp>

#include <SevenBit/DI/Exceptions.hpp>

namespace sb::di
{
    INLINE InjectorException::InjectorException(const std::string &error) : std::runtime_error{error} {}

    INLINE NullPointerException::NullPointerException(const std::string &why) : InjectorException{why} {}

    INLINE InvalidServiceException::InvalidServiceException()
        : InjectorException{std::string{"Service is in not valid state."}}
    {
    }

    INLINE InvalidServiceException::InvalidServiceException(const TypeId typeId)
        : InjectorException{std::string{"Service: '"} + typeId.name() + "' is in not valid state."}
    {
    }

    INLINE CannotReleaseServiceException::CannotReleaseServiceException(const TypeId typeId, const std::string &reason)
        : InjectorException{std::string{"Cannot release ownership of service: '"} + typeId.name() +
                            "', reason: " + reason + "."}
    {
    }

    INLINE CannotMoveOutServiceException::CannotMoveOutServiceException(const TypeId typeId, const std::string &reason)
        : InjectorException{std::string{"Cannot move out service: '"} + typeId.name() + "', reason: " + reason + "."}
    {
    }

    INLINE ServiceNotFoundException::ServiceNotFoundException(const TypeId typeIndex, const std::string &reason)
        : InjectorException{std::string{"Service: '"} + typeIndex.name() + "' was not found, reason: " + reason + "."}
    {
    }

    INLINE CircularDependencyException::CircularDependencyException(const TypeId typeIndex)
        : InjectorException{std::string{"Circular dependency detected while creating service: '"} + typeIndex.name() +
                            "'."}
    {
    }

    INLINE ServiceAlreadyRegisteredException::ServiceAlreadyRegisteredException(const TypeId typeIndex)
        : InjectorException{std::string{"Service: '"} + typeIndex.name() + "' was already registered."}
    {
    }

    INLINE ServiceAliasMismatchException::ServiceAliasMismatchException(const TypeId typeIndex, const TypeId interface,
                                                                        const bool shoudBeAlias)
        : InjectorException{std::string{"Service: '"} + typeIndex.name() +
                            (shoudBeAlias ? "' should be" : "' should not be") +
                            " alias as other services implementing this interface '" + interface.name() +
                            "' that are already registered."}
    {
    }

    INLINE ServiceLifeTimeMismatchException::ServiceLifeTimeMismatchException(const TypeId typeIndex,
                                                                              const TypeId interface)
        : InjectorException{std::string{"Service: '"} + typeIndex.name() +
                            "' should have same scope as other services implementing this interface '" +
                            interface.name() + "' that are already registered."}
    {
    }
} // namespace sb::di
