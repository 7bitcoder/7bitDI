#include <gtest/gtest.h>
#include <iostream>
#include <memory>

#include "Classes/BasicTest.hpp"
#include "Mocks/ServiceProviderMock.hpp"
#include "SevenBit/Details/ServiceFcnFactory.hpp"
#include "SevenBit/IServiceProvider.hpp"
#include "SevenBit/ServiceLifeTime.hpp"

class ServiceFcnFactoryTest : public ::testing::Test
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
    // auto fcn = [&](sb::IServiceProvider &) { return test; };

    // sb::details::ServiceFcnFactory<TestClass1 *, decltype(fcn)> factory{sb::ServiceLifeTime::transient(), fcn};
}

TEST_F(ServiceFcnFactoryTest, ShouldReturnProperTypeId)
{
    auto fcn = [&](sb::IServiceProvider &) { return std::make_unique<TestClass1>(); };

    sb::details::ServiceFcnFactory factory{fcn};

    EXPECT_EQ(factory.getServiceTypeId(), typeid(TestClass1));
}

TEST_F(ServiceFcnFactoryTest, ShouldCreateService)
{
    ServiceProviderMock mock;
    auto fcn = [&](sb::IServiceProvider &) { return std::make_unique<TestClass1>(); };
    sb::details::ServiceFcnFactory factory{fcn};

    auto instance = factory.createInstance(mock);

    EXPECT_TRUE(instance);
}

TEST_F(ServiceFcnFactoryTest, ShouldCloneFactory)
{
    ServiceProviderMock mock;
    auto fcn = [&](sb::IServiceProvider &) { return std::make_unique<TestClass1>(); };
    sb::details::ServiceFcnFactory factory{fcn};

    auto cloned = factory.clone();

    EXPECT_TRUE(cloned);
    EXPECT_TRUE(cloned->createInstance(mock));
    EXPECT_NE(cloned.get(), &factory);
}