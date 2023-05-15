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
    struct EXPORT InjectorException : public std::runtime_error
    {
        InjectorException(const std::string &error);
    };

    struct EXPORT NullPointnerException : public InjectorException
    {
        NullPointnerException(const std::string &why);
    };

    struct EXPORT ForbiddenServiceFactoryException : public InjectorException
    {
        ForbiddenServiceFactoryException(TypeId typeId, const std::string &reason);
    };

    struct EXPORT CannotMoveOutServiceException : public InjectorException
    {
        CannotMoveOutServiceException(TypeId typeId, const std::string &reason);
    };

    struct EXPORT InvalidServiceException : public InjectorException
    {
        InvalidServiceException(TypeId typeId, const std::string &reason);
    };

    struct EXPORT ServiceNotFoundException : public InjectorException
    {
        ServiceNotFoundException(TypeId typeIndex, const std::string &reason);
    };

    struct EXPORT ServiceAlreadyRegisteredException : public InjectorException
    {
        ServiceAlreadyRegisteredException(TypeId typeIndex);
    };

    struct EXPORT ServiceBaseTypeMissmatchException : public InjectorException
    {
        ServiceBaseTypeMissmatchException(TypeId typeIndex, TypeId interface);
    };

    struct EXPORT ServiceLifeTimeMissmatchException : public InjectorException
    {
        ServiceLifeTimeMissmatchException(TypeId typeIndex, TypeId interface);
    };

    struct EXPORT CircularDependencyException : public InjectorException
    {
        CircularDependencyException(TypeId typeIndex);
    };
} // namespace sb::di

#ifdef SEVEN_BIT_DI_ADD_IMPL
#include "SevenBit/DI/Details/Impl/Exceptions.hpp"
#endif