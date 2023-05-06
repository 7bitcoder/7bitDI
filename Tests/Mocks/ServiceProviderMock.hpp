#pragma once

#include <gmock/gmock.h>

#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/IServiceProvider.hpp"

struct ServiceProviderMock : public sb::di::IServiceProvider
{
    MOCK_METHOD((sb::di::IServiceProvider::Ptr), createScope, (), (override));
    MOCK_METHOD((const sb::di::IServiceInstance *), tryGetInstance, (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((const sb::di::IServiceInstance &), getInstance, (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((std::vector<const sb::di::IServiceInstance *>), getInstances, (sb::di::TypeId serviceTypeId),
                (override));
    MOCK_METHOD((sb::di::IServiceInstance::Ptr), tryCreateInstance, (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((sb::di::IServiceInstance::Ptr), createInstance, (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((std::vector<sb::di::IServiceInstance::Ptr>), createInstances, (sb::di::TypeId serviceTypeId),
                (override));
};