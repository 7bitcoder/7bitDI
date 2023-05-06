#include <gtest/gtest.h>
#include <iostream>
#include <memory>

#include "Classes/BasicTest.hpp"
#include "Mocks/ServiceProviderMock.hpp"
#include "SevenBit/DI/Details/ServiceFcnFactory.hpp"
#include "SevenBit/DI/IServiceProvider.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"

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
    // auto fcn = [&](sb::di::IServiceProvider &) { return test; };

    // sb::di::details::ServiceFcnFactory<TestClass1 *, decltype(fcn)> factory{sb::di::ServiceLifeTime::transient(),
    // fcn};
}

TEST_F(ServiceFcnFactoryTest, ShouldReturnProperTypeId)
{
    auto fcn = [&](sb::di::IServiceProvider &) { return std::make_unique<TestClass1>(); };

    sb::di::details::ServiceFcnFactory factory{fcn};

    EXPECT_EQ(factory.getServiceTypeId(), typeid(TestClass1));
}

TEST_F(ServiceFcnFactoryTest, ShouldCreateService)
{
    ServiceProviderMock mock;
    auto fcn = [&](sb::di::IServiceProvider &) { return std::make_unique<TestClass1>(); };
    sb::di::details::ServiceFcnFactory factory{fcn};

    auto instance = factory.createInstance(mock);

    EXPECT_TRUE(instance);
}

TEST_F(ServiceFcnFactoryTest, ShouldCloneFactory)
{
    ServiceProviderMock mock;
    auto fcn = [&](sb::di::IServiceProvider &) { return std::make_unique<TestClass1>(); };
    sb::di::details::ServiceFcnFactory factory{fcn};

    auto cloned = factory.clone();

    EXPECT_TRUE(cloned);
    EXPECT_TRUE(cloned->createInstance(mock));
    EXPECT_NE(cloned.get(), &factory);
}