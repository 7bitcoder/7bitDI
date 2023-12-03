#include <gtest/gtest.h>
#include <iostream>
#include <memory>

#include "Classes/BasicTest.hpp"
#include "Mocks/ServiceProviderMock.hpp"
#include "SevenBit/DI/Details/ExternalServiceFcnFactory.hpp"
#include "SevenBit/DI/Details/ServiceFcnFactory.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

class ExternalServiceFcnFactoryTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ExternalServiceFcnFactoryTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ExternalServiceFcnFactoryTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(ExternalServiceFcnFactoryTest, ShouldNotCompileWrongFactoryScheme)
{
    // TestClass1 test;
    // auto fcn = [&](sb::di::ServiceProvider &) { return test; };

    // sb::di::details::ServiceFcnFactory<TestClass1 *, decltype(fcn)> factory{sb::di::ServiceLifeTime::transient(),
    // fcn};
}

TEST_F(ExternalServiceFcnFactoryTest, ShouldReturnProperTypeId)
{
    TestClass1 test;
    auto fcn = [&](sb::di::ServiceProvider &) { return &test; };
    sb::di::details::ExternalServiceFcnFactory factory{fcn};

    EXPECT_EQ(factory.getServiceTypeId(), typeid(TestClass1));
}

TEST_F(ExternalServiceFcnFactoryTest, ShouldCreateService)
{
    ServiceProviderMock mock;
    TestClass1 test;
    auto fcn = [&](sb::di::ServiceProvider &) { return &test; };
    sb::di::details::ExternalServiceFcnFactory factory{fcn};

    auto instance = factory.createInstance(mock, false);

    EXPECT_TRUE(instance);
}
