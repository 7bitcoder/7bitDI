#pragma once

#include <string>

#include "SevenBit/LibraryConfig.hpp"
#include "SevenBit/_Internal/Exceptions.hpp"

namespace sb
{
    INLINE NullReferenceException::NullReferenceException() : std::runtime_error{"Null reference"} {}

    INLINE ServiceHolderInvalidException::ServiceHolderInvalidException()
        : std::runtime_error{std::string{"Service holder is invalid or null."}}
    {
    }

    INLINE ServiceCreatorInvalidException::ServiceCreatorInvalidException()
        : std::runtime_error{std::string{"Service creator is invalid or null."}}
    {
    }

    INLINE ServiceNotRegisteredException::ServiceNotRegisteredException(TypeId typeIndex)
        : std::runtime_error{std::string{"Service: '"} + typeIndex.name() + "' was not registered."}
    {
    }

    INLINE ConstructionException::ConstructionException(TypeId typeIndex)
        : std::runtime_error{std::string{"Could not construct service: '"} + typeIndex.name() + "'."}
    {
    }

    INLINE NotTransientException::NotTransientException(TypeId typeIndex)
        : std::runtime_error{std::string{"Service: '"} + typeIndex.name() +
                             "' is not registered as transient and could not be created."}
    {
    }

    INLINE TransientForbidException::TransientForbidException(TypeId typeIndex)
        : std::runtime_error{std::string{"Service: '"} + typeIndex.name() +
                             "' is registered as transient and cannot be saved in DI."}
    {
    }

    INLINE CircularDependencyException::CircularDependencyException(TypeId typeIndex)
        : std::runtime_error{std::string{"Circular dependency detected while creating service: '"} + typeIndex.name() +
                             "'."}
    {
    }

    INLINE ServiceCreatorProviderNotSet::ServiceCreatorProviderNotSet()
        : std::runtime_error{"Service creator provider not set."}
    {
    }

    INLINE ServiceShouldBeProvidedExternally::ServiceShouldBeProvidedExternally(TypeId typeIndex)
        : std::runtime_error{std::string{"Service: '"} + typeIndex.name() + "' should be provided externally."}
    {
    }

    INLINE ServiceTypeAlreadyRegisteredException::ServiceTypeAlreadyRegisteredException(TypeId typeIndex)
        : std::runtime_error{std::string{"Service: '"} + typeIndex.name() + "' was already registered."}
    {
    }

    INLINE ServiceScopeMissmatchException::ServiceScopeMissmatchException(TypeId typeIndex, TypeId interface)
        : std::runtime_error{std::string{"Service: '"} + typeIndex.name() +
                             "' should have same scope as other services implementing this interface '" +
                             interface.name() + "' that are already registered."}
    {
    }
} // namespace sb