#pragma once

#include <exception>
#include <stdexcept>
#include <string>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/TypeId.hpp"

namespace sb::di
{
    /**
     * @brief Base class for all injector exceptions
     */
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

    struct ServiceNotFoundException : public InjectorException
    {
        ServiceNotFoundException(TypeId typeIndex, const std::string &reason);
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

    struct CircularDependencyException : public InjectorException
    {
        CircularDependencyException(TypeId typeIndex);
    };
} // namespace sb::di

#ifdef SEVEN_BIT_INJECTOR_ADD_IMPL
#include "SevenBit/DI/Details/Impl/Exceptions.hpp"
#endif