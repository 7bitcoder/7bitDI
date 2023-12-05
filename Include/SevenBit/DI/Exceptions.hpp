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
        explicit InjectorException(const std::string &error);
    };

    struct EXPORT NullPointerException : public InjectorException
    {
        explicit NullPointerException(const std::string &why);
    };

    struct EXPORT InvalidServiceException : public InjectorException
    {
        explicit InvalidServiceException(TypeId typeId);
    };

    struct EXPORT ForbiddenServiceFactoryException : public InjectorException
    {
        ForbiddenServiceFactoryException(TypeId typeId, const std::string &reason);
    };

    struct EXPORT CannotReleaseServiceException : public InjectorException
    {
        CannotReleaseServiceException(TypeId typeId, const std::string &reason);
    };

    struct EXPORT CannotMoveOutServiceException : public InjectorException
    {
        CannotMoveOutServiceException(TypeId typeId, const std::string &reason);
    };

    struct EXPORT ServiceNotFoundException : public InjectorException
    {
        ServiceNotFoundException(TypeId typeIndex, const std::string &reason);
    };

    struct EXPORT ServiceAlreadyRegisteredException : public InjectorException
    {
        explicit ServiceAlreadyRegisteredException(TypeId typeIndex);
    };

    struct EXPORT ServiceBaseTypeMismatchException : public InjectorException
    {
        ServiceBaseTypeMismatchException(TypeId typeIndex, TypeId interface);
    };

    struct EXPORT ServiceLifeTimeMismatchException : public InjectorException
    {
        ServiceLifeTimeMismatchException(TypeId typeIndex, TypeId interface);
    };

    struct EXPORT CircularDependencyException : public InjectorException
    {
        explicit CircularDependencyException(TypeId typeIndex);
    };
} // namespace sb::di

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Impl/Exceptions.hpp"
#endif
