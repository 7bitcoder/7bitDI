#include <gtest/gtest.h>
#include <iostream>
#include <memory>

#include "Classes/BasicTest.hpp"
#include "Mocks/ServiceProviderMock.hpp"
#include "SevenBit/DI/Details/ServiceFcnFactory.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

class ServiceFcnFactoryTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceFcnFactoryTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceFcnFactoryTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(ServiceFcnFactoryTest, ShouldNotCompileWrongFactoryScheme)
{
    // TestClass1 test;
    // auto fcn = [&](auto f) { return std::make_unique<TestClass1>(); };

    // sb::di::details::ServiceFcnFactory factory{std::move(fcn)};
}

TEST_F(ServiceFcnFactoryTest, ShouldReturnProperTypeId)
{
    auto fcn = [&]() { return std::make_unique<TestClass1>(); };

    sb::di::details::ServiceFcnFactory factory{std::move(fcn)};

    EXPECT_EQ(factory.getServiceTypeId(), typeid(TestClass1));
}

TEST_F(ServiceFcnFactoryTest, ShouldCreateService)
{
    ServiceProviderMock mock;
    auto fcn = [&]() { return std::make_unique<TestClass1>(); };
    sb::di::details::ServiceFcnFactory factory{std::move(fcn)};

    auto instance = factory.createInstance(mock, false);

    EXPECT_TRUE(instance);
}
