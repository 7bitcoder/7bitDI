#pragma once

#include <stdexcept>
#include <string>
#include <string_view>

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
        CannotReleaseServiceException(TypeId typeId, std::string_view reason);
    };

    struct EXPORT CannotMoveOutServiceException : InjectorException
    {
        CannotMoveOutServiceException(TypeId typeId, std::string_view reason);
    };

    struct EXPORT ServiceNotFoundException : InjectorException
    {
        ServiceNotFoundException(TypeId typeId, std::string_view reason);
    };

    struct EXPORT ServiceAlreadyRegisteredException : InjectorException
    {
        explicit ServiceAlreadyRegisteredException(TypeId typeId);
    };

    struct EXPORT ServiceLifeTimeMismatchException : InjectorException
    {
        ServiceLifeTimeMismatchException(TypeId typeId, TypeId interface);
    };

    struct EXPORT ServiceAliasMismatchException : InjectorException
    {
        ServiceAliasMismatchException(TypeId typeId, TypeId interface, bool shouldBeAlias);
    };

    struct EXPORT CircularDependencyException : InjectorException
    {
        explicit CircularDependencyException(TypeId typeId);
    };
} // namespace sb::di

#ifdef _7BIT_DI_ADD_IMPL
#include "SevenBit/DI/Impl/Exceptions.hpp"
#endif
