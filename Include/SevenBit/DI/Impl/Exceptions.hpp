#pragma once

#include <string>

#include "SevenBit/DI/LibraryConfig.hpp"

#include "SevenBit/DI/Details/Utils/String.hpp"
#include "SevenBit/DI/Exceptions.hpp"

namespace sb::di
{
    INLINE InjectorException::InjectorException(const std::string &error) : std::runtime_error{error} {}

    INLINE NullPointerException::NullPointerException(const std::string &why) : InjectorException{why} {}

    INLINE ServiceRegisterException::ServiceRegisterException(const TypeId typeId, const std::string &reason)
        : InjectorException{details::String::fmt("Cannot register service '{}', reason: {}.", typeId.name(), reason)}
    {
    }

    INLINE ServiceNotFoundException::ServiceNotFoundException(const TypeId typeId, const std::string &reason)
        : InjectorException{details::String::fmt("Service '{}' was not found, reason: {}.", typeId.name(), reason)}
    {
    }

    INLINE CannotMoveOutServiceException::CannotMoveOutServiceException(const TypeId typeId, const std::string &reason)
        : InjectorException{details::String::fmt("Cannot move out service '{}', reason: {}.", typeId.name(), reason)}
    {
    }

    INLINE CannotReleaseServiceException::CannotReleaseServiceException(const TypeId typeId, const std::string &reason)
        : InjectorException{
              details::String::fmt("Cannot release ownership of service '{}', reason: {}.", typeId.name(), reason)}
    {
    }

    INLINE InvalidServiceException::InvalidServiceException()
        : InjectorException{std::string{"Service is not in valid state."}}
    {
    }

    INLINE InvalidServiceException::InvalidServiceException(const TypeId typeId)
        : InjectorException{details::String::fmt("Service: '{}' is not in valid state.", typeId.name())}
    {
    }

    INLINE CircularDependencyException::CircularDependencyException(const TypeId typeId)
        : InjectorException{
              details::String::fmt("Circular dependency detected while creating service: '{}'.", typeId.name())}
    {
    }
} // namespace sb::di
