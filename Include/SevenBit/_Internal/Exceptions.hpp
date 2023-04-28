#pragma once

#include <stdexcept>
#include <string>

#include "SevenBit/_Internal/TypeId.hpp"

namespace sb
{
    struct NullReferenceException : public std::runtime_error
    {
        NullReferenceException();
    };

    struct ServiceHolderInvalidException : public std::runtime_error
    {
        ServiceHolderInvalidException();
    };

    struct ServiceCreatorInvalidException : public std::runtime_error
    {
        ServiceCreatorInvalidException();
    };

    struct ServiceNotRegisteredException : public std::runtime_error
    {
        ServiceNotRegisteredException(TypeId typeIndex);
    };

    struct ConstructionException : public std::runtime_error
    {
        ConstructionException(TypeId typeIndex);
    };

    struct NotTransientException : public std::runtime_error
    {
        NotTransientException(TypeId typeIndex);
    };

    struct TransientForbidException : public std::runtime_error
    {
        TransientForbidException(TypeId typeIndex);
    };

    struct CircularDependencyException : public std::runtime_error
    {
        CircularDependencyException(TypeId typeIndex);
    };

    struct ServiceCreatorProviderNotSet : public std::runtime_error
    {
        ServiceCreatorProviderNotSet();
    };

    struct ServiceShouldBeProvidedExternally : public std::runtime_error
    {
        ServiceShouldBeProvidedExternally(TypeId typeIndex);
    };

    struct ServiceTypeAlreadyRegisteredException : public std::runtime_error
    {
        ServiceTypeAlreadyRegisteredException(TypeId typeIndex);
    };

    struct ServiceScopeMissmatchException : public std::runtime_error
    {
        ServiceScopeMissmatchException(TypeId typeIndex, TypeId interface);
    };
} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/Exceptions.hpp"
#endif