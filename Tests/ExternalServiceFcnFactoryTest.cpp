#include <gtest/gtest.h>
#include <iostream>
#include <memory>

#include "Classes/BasicTest.hpp"
#include "Mocks/ServiceProviderMock.hpp"
#include "SevenBit/Details/ExternalServiceFcnFactory.hpp"
#include "SevenBit/Details/ServiceFcnFactory.hpp"
#include "SevenBit/IServiceProvider.hpp"
#include "SevenBit/ServiceLifeTime.hpp"

class ExternalServiceFcnFactoryTest : public ::testing::Test
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
    // auto fcn = [&](sb::IServiceProvider &) { return test; };

    // sb::details::ServiceFcnFactory<TestClass1 *, decltype(fcn)> factory{sb::ServiceLifeTime::transient(), fcn};
}

TEST_F(ExternalServiceFcnFactoryTest, ShouldReturnProperTypeId)
{
    TestClass1 test;
    auto fcn = [&](sb::IServiceProvider &) { return &test; };
    sb::details::ExternalServiceFcnFactory factory{fcn};

    EXPECT_EQ(factory.getServiceTypeId(), typeid(TestClass1));
}

TEST_F(ExternalServiceFcnFactoryTest, ShouldCreateService)
{
    ServiceProviderMock mock;
    TestClass1 test;
    auto fcn = [&](sb::IServiceProvider &) { return &test; };
    sb::details::ExternalServiceFcnFactory factory{fcn};

    auto instance = factory.createInstance(mock);

    EXPECT_TRUE(instance);
}

TEST_F(ExternalServiceFcnFactoryTest, ShouldCloneFactory)
{
    ServiceProviderMock mock;
    TestClass1 test;
    auto fcn = [&](sb::IServiceProvider &) { return &test; };
    sb::details::ExternalServiceFcnFactory factory{fcn};

    auto cloned = factory.clone();

    EXPECT_TRUE(cloned);
    EXPECT_TRUE(cloned->createInstance(mock));
    EXPECT_NE(cloned.get(), &factory);
}