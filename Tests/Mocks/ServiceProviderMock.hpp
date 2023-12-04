#pragma once

#include <gmock/gmock.h>

#include "SevenBit/DI/ServiceProvider.hpp"

struct ServiceProviderMock : public sb::di::ServiceProvider
{
    MOCK_METHOD((std::unique_ptr<sb::di::ServiceProvider>), createScope, (), (override));
    MOCK_METHOD((const sb::di::IServiceInstance *), tryGetInstance, (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((const sb::di::IServiceInstance &), getInstance, (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((const sb::di::OneOrList<sb::di::IServiceInstance::Ptr> *), tryGetInstances,
                (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((sb::di::IServiceInstance::Ptr), tryCreateInstance, (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((sb::di::IServiceInstance::Ptr), createInstance, (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((sb::di::IServiceInstance::Ptr), tryCreateInstanceInPlace, (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((sb::di::IServiceInstance::Ptr), createInstanceInPlace, (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((std::optional<sb::di::OneOrList<sb::di::IServiceInstance::Ptr>>), tryCreateInstances,
                (sb::di::TypeId serviceTypeId), (override));
};
