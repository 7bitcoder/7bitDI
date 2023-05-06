#include <gtest/gtest.h>
#include <iostream>

#include "Classes/BasicTest.hpp"
#include "Mocks/ServiceProviderMock.hpp"
#include "SevenBit/Details/ExternalServiceFactory.hpp"
#include "SevenBit/IServiceProvider.hpp"

class ExternalServiceFactoryTest : public ::testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ExternalServiceFactoryTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ExternalServiceFactoryTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(ExternalServiceFactoryTest, ShouldReturnProperTypeId)
{
    TestClass1 test;
    sb::details::ExternalServiceFactory<TestClass1> factory{&test};

    EXPECT_EQ(factory.getServiceTypeId(), typeid(TestClass1));
}

TEST_F(ExternalServiceFactoryTest, ShouldCreateService)
{
    TestClass1 test;
    ServiceProviderMock mock;
    sb::details::ExternalServiceFactory<TestClass1> factory{&test};

    auto instance = factory.createInstance(mock);

    EXPECT_TRUE(instance);
    EXPECT_EQ(instance->get(), &test);
}

TEST_F(ExternalServiceFactoryTest, ShouldCloneFactory)
{
    TestClass1 test;

    sb::details::ExternalServiceFactory<TestClass1> factory{&test};

    auto cloned = factory.clone();

    EXPECT_TRUE(cloned);
    EXPECT_NE(cloned.get(), &factory);
}