#include <gtest/gtest.h>
#include <iostream>

#include "Classes/BasicTest.hpp"
#include "Mocks/ServiceProviderMock.hpp"
#include "SevenBit/DI/Details/ExternalServiceFactory.hpp"
#include "SevenBit/DI/Details/ServiceCtorFactory.hpp"
#include "SevenBit/DI/IServiceProvider.hpp"

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
    sb::di::details::ServiceCtorFactory<TestClass1> factory{};

    EXPECT_EQ(factory.getServiceTypeId(), typeid(TestClass1));
}

TEST_F(ServiceCtorFactoryTest, ShouldCreateService)
{
    ServiceProviderMock mock;
    sb::di::details::ServiceCtorFactory<TestClass1> factory{};

    auto instance = factory.createInstance(mock);

    EXPECT_TRUE(instance);
}

TEST_F(ServiceCtorFactoryTest, ShouldCloneFactory)
{
    ServiceProviderMock mock;
    sb::di::details::ServiceCtorFactory<TestClass1> factory{};

    auto cloned = factory.clone();

    EXPECT_TRUE(cloned);
    EXPECT_TRUE(cloned->createInstance(mock));
    EXPECT_NE(cloned.get(), &factory);
}