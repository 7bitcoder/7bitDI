#pragma once

#include <stdexcept>
#include <string>
#include <typeindex>

#include "SevenBit/_Internal/IServiceHolder.hpp"

namespace sb
{
    struct NullReferenceException : public std::runtime_error
    {
        NullReferenceException() : std::runtime_error{"Null reference"} {}
    };

    struct ServiceHolderInvalidException : public std::runtime_error
    {
        ServiceHolderInvalidException() : std::runtime_error{std::string{"Service holder is invalid or null."}} {}
    };

    struct ServiceCreatorInvalidException : public std::runtime_error
    {
        ServiceCreatorInvalidException() : std::runtime_error{std::string{"Service creator is invalid or null."}} {}
    };

    struct ServiceNotRegisteredException : public std::runtime_error
    {
        ServiceNotRegisteredException(TypeId typeIndex)
            : std::runtime_error{std::string{"Service: '"} + typeIndex.name() + "' was not registered."}
        {
        }
    };

    struct ConstructionException : public std::runtime_error
    {
        ConstructionException(TypeId typeIndex)
            : std::runtime_error{std::string{"Could not construct service: '"} + typeIndex.name() + "'."}
        {
        }
    };

    struct NotTransientException : public std::runtime_error
    {
        NotTransientException(TypeId typeIndex)
            : std::runtime_error{std::string{"Service: '"} + typeIndex.name() +
                                 "' is not registered as transient and could not be created."}
        {
        }
    };

    struct TransientForbidException : public std::runtime_error
    {
        TransientForbidException(TypeId typeIndex)
            : std::runtime_error{std::string{"Service: '"} + typeIndex.name() +
                                 "' is registered as transient and cannot be saved in DI."}
        {
        }
    };

    struct CircularDependencyException : public std::runtime_error
    {
        CircularDependencyException(TypeId typeIndex)
            : std::runtime_error{std::string{"Circular dependency detected while creating service: '"} +
                                 typeIndex.name() + "'."}
        {
        }
    };

    struct ServiceCreatorProviderNotSet : public std::runtime_error
    {
        ServiceCreatorProviderNotSet() : std::runtime_error{"Service creator provider not set."} {}
    };

    struct ServiceShouldBeProvidedExternally : public std::runtime_error
    {
        ServiceShouldBeProvidedExternally(TypeId typeIndex)
            : std::runtime_error{std::string{"Service: '"} + typeIndex.name() + "' should be provided externally."}
        {
        }
    };

    struct ServiceTypeAlreadyRegisteredException : public std::runtime_error
    {
        ServiceTypeAlreadyRegisteredException(TypeId typeIndex)
            : std::runtime_error{std::string{"Service: '"} + typeIndex.name() + "' was already registered."}
        {
        }
    };

    struct ServiceScopeMissmatchException : public std::runtime_error
    {
        ServiceScopeMissmatchException(TypeId typeIndex, TypeId interface)
            : std::runtime_error{std::string{"Service: '"} + typeIndex.name() +
                                 "' should have same scope as other services implementing this interface '" +
                                 interface.name() + "' that are already registered."}
        {
        }
    };
} // namespace sb