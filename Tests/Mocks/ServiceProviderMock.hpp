#pragma once

#include "ServiceInstanceProviderMock.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

struct ServiceProviderMock : public sb::di::ServiceProvider
{
    ServiceProviderMock() : sb::di::ServiceProvider(prepare()) {}

    [[nodiscard]] static sb::di::IServiceInstanceProvider::Ptr prepare()
    {
        auto instance = std::make_unique<ServiceInstanceProviderMock>();
        EXPECT_CALL(*instance, setServiceProvider).WillOnce(testing::Return());

        return instance;
    }

    ServiceInstanceProviderMock &getMock()
    {
        return dynamic_cast<ServiceInstanceProviderMock &>(getInstanceProvider());
    }
};
