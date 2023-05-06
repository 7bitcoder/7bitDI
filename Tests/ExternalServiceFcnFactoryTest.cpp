#include <gtest/gtest.h>
#include <iostream>
#include <memory>

#include "Classes/BasicTest.hpp"
#include "Mocks/ServiceProviderMock.hpp"
#include "SevenBit/DI/Details/ExternalServiceFcnFactory.hpp"
#include "SevenBit/DI/Details/ServiceFcnFactory.hpp"
#include "SevenBit/DI/IServiceProvider.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"

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
    // auto fcn = [&](sb::di::IServiceProvider &) { return test; };

    // sb::di::details::ServiceFcnFactory<TestClass1 *, decltype(fcn)> factory{sb::di::ServiceLifeTime::transient(),
    // fcn};
}

TEST_F(ExternalServiceFcnFactoryTest, ShouldReturnProperTypeId)
{
    TestClass1 test;
    auto fcn = [&](sb::di::IServiceProvider &) { return &test; };
    sb::di::details::ExternalServiceFcnFactory factory{fcn};

    EXPECT_EQ(factory.getServiceTypeId(), typeid(TestClass1));
}

TEST_F(ExternalServiceFcnFactoryTest, ShouldCreateService)
{
    ServiceProviderMock mock;
    TestClass1 test;
    auto fcn = [&](sb::di::IServiceProvider &) { return &test; };
    sb::di::details::ExternalServiceFcnFactory factory{fcn};

    auto instance = factory.createInstance(mock);

    EXPECT_TRUE(instance);
}

TEST_F(ExternalServiceFcnFactoryTest, ShouldCloneFactory)
{
    ServiceProviderMock mock;
    TestClass1 test;
    auto fcn = [&](sb::di::IServiceProvider &) { return &test; };
    sb::di::details::ExternalServiceFcnFactory factory{fcn};

    auto cloned = factory.clone();

    EXPECT_TRUE(cloned);
    EXPECT_TRUE(cloned->createInstance(mock));
    EXPECT_NE(cloned.get(), &factory);
}