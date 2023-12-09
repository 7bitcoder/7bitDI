#include <gtest/gtest.h>

#include "Classes/BasicTest.hpp"
#include "Mocks/ServiceProviderMock.hpp"
#include "SevenBit/DI/Details/Factories/ServiceFactory.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

class ServiceCtorFactoryTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceCtorFactoryTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceCtorFactoryTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ServiceCtorFactoryTest, ShouldReturnProperTypeId)
{
    const sb::di::details::factories::ServiceFactory<TestClass1> factory{};

    EXPECT_EQ(factory.getServiceTypeId(), typeid(TestClass1));
}

TEST_F(ServiceCtorFactoryTest, ShouldCreateService)
{
    ServiceProviderMock mock;
    const sb::di::details::factories::ServiceFactory<TestClass1> factory{};

    const auto instance = factory.createInstance(mock, false);

    EXPECT_TRUE(instance);
}
