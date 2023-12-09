#include <gtest/gtest.h>
#include <memory>

#include "Classes/BasicTest.hpp"
#include "Mocks/ServiceProviderMock.hpp"
#include "SevenBit/DI/Details/Factories/ExternalServiceFcnFactory.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

class ExternalServiceFcnFactoryTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ExternalServiceFcnFactoryTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ExternalServiceFcnFactoryTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ExternalServiceFcnFactoryTest, ShouldNotCompileWrongFactoryScheme)
{
    // TestClass1 test;
    // auto fcn = [&](sb::di::ServiceProvider &) { return test; };

    // sb::di::details::ServiceFcnFactory<TestClass1 *, decltype(fcn)> factory{sb::di::ServiceLifeTime::transient(),
    // fcn};
}

TEST_F(ExternalServiceFcnFactoryTest, ShouldCreateService)
{
    ServiceProviderMock mock;
    TestClass1 test;
    auto fcn = [&] { return &test; };
    const sb::di::details::factories::ExternalServiceFcnFactory factory{fcn};

    const auto instance = factory.createInstance(mock, false);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_EQ(instance->get(), &test);
    EXPECT_THROW(instance->release(), sb::di::CannotReleaseServiceException);
    EXPECT_THROW(instance->getForMoveOut(), sb::di::CannotMoveOutServiceException);
    EXPECT_EQ(instance->getTypeId(), typeid(TestClass1));
    EXPECT_EQ(instance->getTypeId(), factory.getServiceTypeId());
}

TEST_F(ExternalServiceFcnFactoryTest, ShouldCreateNullService)
{
    ServiceProviderMock mock;
    auto fcn = [&]() -> TestClass1 * { return nullptr; };
    const sb::di::details::factories::ExternalServiceFcnFactory factory{fcn};

    const auto instance = factory.createInstance(mock, false);

    EXPECT_TRUE(instance);
    EXPECT_FALSE(instance->isValid());
    EXPECT_FALSE(instance->get());
    EXPECT_THROW(instance->release(), sb::di::CannotReleaseServiceException);
    EXPECT_THROW(instance->getForMoveOut(), sb::di::CannotMoveOutServiceException);
    EXPECT_EQ(instance->getTypeId(), typeid(TestClass1));
    EXPECT_EQ(instance->getTypeId(), factory.getServiceTypeId());
}
