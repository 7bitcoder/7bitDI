#pragma once

#include <gmock/gmock.h>

#include "SevenBit/IServiceProvider.hpp"

struct ServiceProviderMock : public sb::IServiceProvider
{
    MOCK_METHOD((sb::IServiceProvider::Ptr), createScope, (), (override));
    MOCK_METHOD((void *), getService, (sb::TypeId serviceTypeId), (override));
    MOCK_METHOD((void *), getRequiredService, (sb::TypeId serviceTypeId), (override));
    MOCK_METHOD((std::vector<void *>), getServices, (sb::TypeId serviceTypeId), (override));
    MOCK_METHOD((void *), createService, (sb::TypeId serviceTypeId), (override));
    MOCK_METHOD((void *), createRequiredService, (sb::TypeId serviceTypeId), (override));
    MOCK_METHOD((std::vector<void *>), createServices, (sb::TypeId serviceTypeId), (override));
};