#include <gtest/gtest.h>
#include <iostream>

#include "Classes/BasicTest.hpp"
#include "Mocks/ServiceProviderMock.hpp"
#include "SevenBit/DI/Details/Factories/ExternalServiceFactory.hpp"
#include "SevenBit/DI/Details/Factories/ServiceCtorFactory.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

class ServiceCtorFactoryTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceCtorFactoryTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceCtorFactoryTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(ServiceCtorFactoryTest, ShouldReturnProperTypeId)
{
    sb::di::details::factories::ServiceCtorFactory<TestClass1> factory{};

    EXPECT_EQ(factory.getServiceTypeId(), typeid(TestClass1));
}

TEST_F(ServiceCtorFactoryTest, ShouldCreateService)
{
    ServiceProviderMock mock;
    sb::di::details::factories::ServiceCtorFactory<TestClass1> factory{};

    auto instance = factory.createInstance(mock, false);

    EXPECT_TRUE(instance);
}
