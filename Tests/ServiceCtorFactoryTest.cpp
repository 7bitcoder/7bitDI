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

TEST_F(ServiceCtorFactoryTest, ShouldCreateInPlaceService)
{
    ServiceProviderMock mock;
    const sb::di::details::factories::ServiceFactory<TestClass1> factory;

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_THROW(instance->release(), sb::di::CannotReleaseServiceException);
    EXPECT_TRUE(instance->getForMoveOut());
    EXPECT_EQ(instance->getTypeId(), typeid(TestClass1));
    EXPECT_EQ(instance->getTypeId(), factory.getServiceTypeId());
}

TEST_F(ServiceCtorFactoryTest, ShouldCreateService)
{
    ServiceProviderMock mock;
    const sb::di::details::factories::ServiceFactory<TestClass1> factory;

    const auto instance = factory.createInstance(mock, false);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(instance->getForMoveOut());
    EXPECT_TRUE(instance->moveOutAsUniquePtr<TestClass1>());
    EXPECT_EQ(instance->getTypeId(), typeid(TestClass1));
    EXPECT_EQ(instance->getTypeId(), factory.getServiceTypeId());
}
