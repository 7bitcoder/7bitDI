#pragma once

#include <exception>
#include <stdexcept>
#include <string>

#include "SevenBit/LibraryConfig.hpp"

#include "SevenBit/TypeId.hpp"

namespace sb
{
    struct InjectorException : public std::runtime_error
    {
        InjectorException(const std::string &error);
    };

    struct NullPointnerException : public InjectorException
    {
        NullPointnerException(const std::string &why);
    };

    struct ForbiddenServiceFactoryException : public InjectorException
    {
        ForbiddenServiceFactoryException(TypeId typeId, const std::string &reason);
    };

    struct CannotMoveOutServiceException : public InjectorException
    {
        CannotMoveOutServiceException(TypeId typeId, const std::string &reason);
    };

    struct InvalidServiceException : public InjectorException
    {
        InvalidServiceException(TypeId typeId, const std::string &reason);
    };

    struct ServiceNotRegisteredException : public InjectorException
    {
        ServiceNotRegisteredException(TypeId typeIndex);
    };

    struct ServiceAlreadyRegisteredException : public InjectorException
    {
        ServiceAlreadyRegisteredException(TypeId typeIndex);
    };

    struct ServiceBaseTypeMissmatchException : public InjectorException
    {
        ServiceBaseTypeMissmatchException(TypeId typeIndex, TypeId interface);
    };

    struct ServiceLifeTimeMissmatchException : public InjectorException
    {
        ServiceLifeTimeMissmatchException(TypeId typeIndex, TypeId interface);
    };

    struct NotTransientException : public InjectorException
    {
        NotTransientException(TypeId typeIndex);
    };

    struct TransientForbidException : public InjectorException
    {
        TransientForbidException(TypeId typeIndex);
    };

    struct CircularDependencyException : public InjectorException
    {
        CircularDependencyException(TypeId typeIndex);
    };
} // namespace sb

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/_Internal/Impl/Exceptions.hpp"
#endif