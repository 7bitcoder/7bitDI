#pragma once

#include <gmock/gmock.h>

#include "SevenBit/DI/IServiceInstance.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

struct ServiceProviderMock : public sb::di::ServiceProvider
{
    MOCK_METHOD((std::unique_ptr<sb::di::ServiceProvider>), createScope, (), (override));
    MOCK_METHOD((const sb::di::IServiceInstance *), getInstance, (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((std::vector<const sb::di::IServiceInstance *>), getInstances, (sb::di::TypeId serviceTypeId),
                (override));
    MOCK_METHOD((sb::di::IServiceInstance::Ptr), createInstance, (sb::di::TypeId serviceTypeId), (override));
    MOCK_METHOD((std::vector<sb::di::IServiceInstance::Ptr>), createInstances, (sb::di::TypeId serviceTypeId),
                (override));
};
