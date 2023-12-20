#pragma once

#include <gmock/gmock.h>

#include "SevenBit/DI/IServiceInstanceProvider.hpp"

struct ServiceInstanceProviderMock : public sb::di::IServiceInstanceProvider
{
    MOCK_METHOD((std::unique_ptr<sb::di::IServiceInstanceProvider>), createScope, (), (const override));
    MOCK_METHOD((const sb::di::ServiceProviderOptions &), getOptions, (), (const override));
    MOCK_METHOD((void), init, (sb::di::ServiceProvider &), (override));
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
