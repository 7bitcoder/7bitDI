#include <gtest/gtest.h>

#include "../../Helpers/Classes/Basic.hpp"
#include "../../Helpers/Mocks/ServiceProviderMock.hpp"
#include "SevenBit/DI/Details/Factories/VoidServiceFactory.hpp"
#include "SevenBit/DI/Exceptions.hpp"

class VoidServiceFactoryTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    VoidServiceFactoryTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~VoidServiceFactoryTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(VoidServiceFactoryTest, ShouldCreateVoidService)
{
    TestClass1 test;
    ServiceProviderMock mock;
    const sb::di::details::factories::VoidServiceFactory<TestClass1> factory{};

    const auto instance = factory.createInstance(mock, false);

    EXPECT_FALSE(instance);
}

TEST_F(VoidServiceFactoryTest, ShouldCreateVoidInPlaceService)
{
    TestClass1 test;
    ServiceProviderMock mock;
    const sb::di::details::factories::VoidServiceFactory<TestClass1> factory{};

    const auto instance = factory.createInstance(mock, true);

    EXPECT_FALSE(instance);
}
