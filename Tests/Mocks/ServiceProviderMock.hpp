#pragma once

#include <gmock/gmock.h>

#include "SevenBit/IServiceInstance.hpp"
#include "SevenBit/IServiceProvider.hpp"

struct ServiceProviderMock : public sb::IServiceProvider
{
    MOCK_METHOD((sb::IServiceProvider::Ptr), createScope, (), (override));
    MOCK_METHOD((const sb::IServiceInstance *), tryGetInstance, (sb::TypeId serviceTypeId), (override));
    MOCK_METHOD((const sb::IServiceInstance &), getInstance, (sb::TypeId serviceTypeId), (override));
    MOCK_METHOD((std::vector<const sb::IServiceInstance *>), getInstances, (sb::TypeId serviceTypeId), (override));
    MOCK_METHOD((sb::IServiceInstance::Ptr), tryCreateInstance, (sb::TypeId serviceTypeId), (override));
    MOCK_METHOD((sb::IServiceInstance::Ptr), createInstance, (sb::TypeId serviceTypeId), (override));
    MOCK_METHOD((std::vector<sb::IServiceInstance::Ptr>), createInstances, (sb::TypeId serviceTypeId), (override));
};