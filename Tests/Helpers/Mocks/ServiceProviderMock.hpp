#pragma once

#include "ServiceInstanceProviderMock.hpp"
#include <SevenBit/DI/ServiceProvider.hpp>

struct ServiceProviderMock : sb::di::ServiceProvider
{
    ServiceProviderMock() : ServiceProvider(prepare()) {}

    [[nodiscard]] static sb::di::IServiceInstanceProvider::Ptr prepare()
    {
        auto instance = std::make_unique<ServiceInstanceProviderMock>();
        EXPECT_CALL(*instance, init).WillOnce(testing::Return());

        return instance;
    }

    ServiceInstanceProviderMock &getMock()
    {
        return dynamic_cast<ServiceInstanceProviderMock &>(getInstanceProvider());
    }
};
