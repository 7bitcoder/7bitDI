#pragma once

#include <gmock/gmock.h>

#include "SevenBit/DI/IServiceInstanceProvider.hpp"

struct ServiceInstanceProviderMock : public sb::di::IServiceInstanceProvider
{
    MOCK_METHOD((std::unique_ptr<sb::di::IServiceInstanceProvider>), createScope, (), (const override));
    MOCK_METHOD((void), init, (sb::di::ServiceProvider &), (override));
    MOCK_METHOD((const sb::di::ServiceInstance *), tryGetInstance, (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((const sb::di::ServiceInstance &), getInstance, (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((const sb::di::OneOrList<sb::di::ServiceInstance> *), tryGetInstances, (sb::di::TypeId serviceTypeId),
                (override));
    MOCK_METHOD((sb::di::ServiceInstance), tryCreateInstance, (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((sb::di::ServiceInstance), createInstance, (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((sb::di::ServiceInstance), tryCreateInstanceInPlace, (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((sb::di::ServiceInstance), createInstanceInPlace, (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((sb::di::OneOrList<sb::di::ServiceInstance>), tryCreateInstances, (sb::di::TypeId serviceTypeId),
                (override));

    MOCK_METHOD((const sb::di::ServiceInstance *), tryGetKeyedInstance,
                (sb::di::TypeId serviceTypeId, std::string_view serviceKey), (override));
    MOCK_METHOD((const sb::di::ServiceInstance &), getKeyedInstance,
                (sb::di::TypeId serviceTypeId, std::string_view serviceKey), (override));
    MOCK_METHOD((const sb::di::OneOrList<sb::di::ServiceInstance> *), tryGetKeyedInstances,
                (sb::di::TypeId serviceTypeId, std::string_view serviceKey), (override));
    MOCK_METHOD((sb::di::ServiceInstance), tryCreateKeyedInstance,
                (sb::di::TypeId serviceTypeId, std::string_view serviceKey), (override));
    MOCK_METHOD((sb::di::ServiceInstance), createKeyedInstance,
                (sb::di::TypeId serviceTypeId, std::string_view serviceKey), (override));
    MOCK_METHOD((sb::di::ServiceInstance), tryCreateKeyedInstanceInPlace,
                (sb::di::TypeId serviceTypeId, std::string_view serviceKey), (override));
    MOCK_METHOD((sb::di::ServiceInstance), createKeyedInstanceInPlace,
                (sb::di::TypeId serviceTypeId, std::string_view serviceKey), (override));
    MOCK_METHOD((sb::di::OneOrList<sb::di::ServiceInstance>), tryCreateKeyedInstances,
                (sb::di::TypeId serviceTypeId, std::string_view serviceKey), (override));
};
