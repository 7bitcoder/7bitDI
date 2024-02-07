#pragma once

#include <stdexcept>
#include <string>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/TypeId.hpp"

namespace sb::di
{
    /**
     * @brief Base class for all injector exceptions
     */
    struct EXPORT InjectorException : std::runtime_error
    {
        explicit InjectorException(const std::string &error);
    };

    struct EXPORT NullPointerException : InjectorException
    {
        explicit NullPointerException(const std::string &why);
    };

    struct EXPORT InvalidServiceException : InjectorException
    {
        explicit InvalidServiceException();
        explicit InvalidServiceException(TypeId typeId);
    };

    struct EXPORT CannotReleaseServiceException : InjectorException
    {
        CannotReleaseServiceException(TypeId typeId, const std::string &reason);
    };

    struct EXPORT CannotMoveOutServiceException : InjectorException
    {
        CannotMoveOutServiceException(TypeId typeId, const std::string &reason);
    };

    struct EXPORT ServiceNotFoundException : InjectorException
    {
        ServiceNotFoundException(TypeId typeIndex, const std::string &reason);
    };

    struct EXPORT ServiceAlreadyRegisteredException : InjectorException
    {
        explicit ServiceAlreadyRegisteredException(TypeId typeIndex);
    };

    struct EXPORT ServiceBaseTypeMismatchException : InjectorException
    {
        ServiceBaseTypeMismatchException(TypeId typeIndex, TypeId interface);
    };

    struct EXPORT ServiceLifeTimeMismatchException : InjectorException
    {
        ServiceLifeTimeMismatchException(TypeId typeIndex, TypeId interface);
    };

    struct EXPORT ServiceAliasMismatchException : InjectorException
    {
        ServiceAliasMismatchException(TypeId typeIndex, TypeId interface, bool shoudBeAlias);
    };

    struct EXPORT CircularDependencyException : InjectorException
    {
        explicit CircularDependencyException(TypeId typeIndex);
    };
} // namespace sb::di

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Impl/Exceptions.hpp"
#endif
