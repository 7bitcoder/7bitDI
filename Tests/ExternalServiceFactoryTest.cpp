#include <gtest/gtest.h>

#include "Classes/BasicTest.hpp"
#include "Mocks/ServiceProviderMock.hpp"
#include "SevenBit/DI/Details/Factories/ExternalServiceFactory.hpp"

class ExternalServiceFactoryTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ExternalServiceFactoryTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ExternalServiceFactoryTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ExternalServiceFactoryTest, ShouldReturnProperTypeId)
{
    TestClass1 test;
    const sb::di::details::factories::ExternalServiceFactory factory{&test};

    EXPECT_EQ(factory.getServiceTypeId(), typeid(TestClass1));
}

TEST_F(ExternalServiceFactoryTest, ShouldCreateService)
{
    TestClass1 test;
    ServiceProviderMock mock;
    const sb::di::details::factories::ExternalServiceFactory factory{&test};

    const auto instance = factory.createInstance(mock, false);

    EXPECT_TRUE(instance);
    EXPECT_EQ(instance->get(), &test);
}
