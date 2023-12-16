#include <gtest/gtest.h>

#include "../../Helpers/Classes/Basic.hpp"
#include "../../Helpers/Mocks/ServiceProviderMock.hpp"
#include "SevenBit/DI/Details/Factories/ExternalServiceFactory.hpp"
#include "SevenBit/DI/Exceptions.hpp"

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

TEST_F(ExternalServiceFactoryTest, ShouldCreateService)
{
    TestClass1 test;
    ServiceProviderMock mock;
    const sb::di::details::factories::ExternalServiceFactory factory{&test};

    const auto instance = factory.createInstance(mock, false);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_EQ(instance->get(), &test);
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::ExternalService<TestClass1> *>(instance.get()));
    EXPECT_EQ(factory.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(instance->getTypeId(), factory.getServiceTypeId());
}

TEST_F(ExternalServiceFactoryTest, ShouldCreateInPlaceService)
{
    TestClass1 test;
    ServiceProviderMock mock;
    const sb::di::details::factories::ExternalServiceFactory factory{&test};

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_EQ(instance->get(), &test);
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::ExternalService<TestClass1> *>(instance.get()));
    EXPECT_EQ(factory.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(instance->getTypeId(), factory.getServiceTypeId());
}

TEST_F(ExternalServiceFactoryTest, ShouldCreateNullService)
{
    ServiceProviderMock mock;
    const sb::di::details::factories::ExternalServiceFactory<TestClass1> factory{nullptr};

    const auto instance = factory.createInstance(mock, false);

    EXPECT_TRUE(instance);
    EXPECT_FALSE(instance->isValid());
    EXPECT_FALSE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::ExternalService<TestClass1> *>(instance.get()));
    EXPECT_EQ(factory.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(instance->getTypeId(), factory.getServiceTypeId());
}

TEST_F(ExternalServiceFactoryTest, ShouldCreateNullInPlaceService)
{
    ServiceProviderMock mock;
    const sb::di::details::factories::ExternalServiceFactory<TestClass1> factory{nullptr};

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_FALSE(instance->isValid());
    EXPECT_FALSE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::ExternalService<TestClass1> *>(instance.get()));
    EXPECT_EQ(factory.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(instance->getTypeId(), factory.getServiceTypeId());
}
