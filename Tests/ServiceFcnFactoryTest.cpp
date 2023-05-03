#include <gtest/gtest.h>
#include <iostream>
#include <memory>

#include "Classes/BasicTest.hpp"
#include "Mocks/ServiceProviderMock.hpp"
#include "SevenBit/IServiceProvider.hpp"
#include "SevenBit/ServiceLifeTime.hpp"
#include "SevenBit/_Internal/ServiceFcnFactory.hpp"

class ServiceFcnFactoryTest : public ::testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceFcnFactoryTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceFcnFactoryTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(ServiceFcnFactoryTest, ShouldNotCompileWrongFactoryScheme)
{
    // TestClass1 test;
    // auto fcn = [&](sb::IServiceProvider &) { return test; };

    // sb::internal::ServiceFcnFactory<TestClass1 *, decltype(fcn)> factory{sb::ServiceLifeTime::transient(), fcn};
}

TEST_F(ServiceFcnFactoryTest, ShouldFailtToConstructTransientFactory)
{
    TestClass1 test;
    auto fcn = [&](sb::IServiceProvider &) { return &test; };

    auto act = [&]() {
        sb::internal::ServiceFcnFactory<TestClass1 *, decltype(fcn)> factory{sb::ServiceLifeTime::transient(), fcn};
    };

    EXPECT_THROW((act()), sb::ForbiddenServiceFactoryException);
}

TEST_F(ServiceFcnFactoryTest, ShouldReturnProperTypeIdForPointner)
{
    TestClass1 test;
    auto fcn = [&](sb::IServiceProvider &) { return &test; };
    sb::internal::ServiceFcnFactory<TestClass1 *, decltype(fcn)> factory{sb::ServiceLifeTime::singleton(), fcn};

    EXPECT_EQ(factory.getServiceTypeId(), typeid(TestClass1));
}

TEST_F(ServiceFcnFactoryTest, ShouldReturnProperTypeIdForUniquePointner)
{
    auto fcn = [&](sb::IServiceProvider &) { return std::make_unique<TestClass1>(); };

    sb::internal::ServiceFcnFactory<std::unique_ptr<TestClass1>, decltype(fcn)> factory{
        sb::ServiceLifeTime::singleton(), fcn};

    EXPECT_EQ(factory.getServiceTypeId(), typeid(TestClass1));
}

TEST_F(ServiceFcnFactoryTest, ShouldCreateExternService)
{
    TestClass1 test;
    ServiceProviderMock mock;
    auto fcn = [&](sb::IServiceProvider &) { return &test; };
    sb::internal::ServiceFcnFactory<TestClass1 *, decltype(fcn)> factory{sb::ServiceLifeTime::singleton(), fcn};

    auto instance = factory.createInstance(mock);

    EXPECT_TRUE(instance);
    EXPECT_EQ(instance->get(), &test);
}

TEST_F(ServiceFcnFactoryTest, ShouldCreateService)
{
    ServiceProviderMock mock;
    auto fcn = [&](sb::IServiceProvider &) { return std::make_unique<TestClass1>(); };
    sb::internal::ServiceFcnFactory<std::unique_ptr<TestClass1>, decltype(fcn)> factory{
        sb::ServiceLifeTime::singleton(), fcn};

    auto instance = factory.createInstance(mock);

    EXPECT_TRUE(instance);
}

TEST_F(ServiceFcnFactoryTest, ShouldCloneFactory)
{
    TestClass1 test;
    ServiceProviderMock mock;
    auto fcn = [&](sb::IServiceProvider &) { return &test; };
    sb::internal::ServiceFcnFactory<TestClass1 *, decltype(fcn)> factory{sb::ServiceLifeTime::singleton(), fcn};

    auto cloned = factory.clone();

    EXPECT_TRUE(cloned);
    EXPECT_NE(cloned.get(), &factory);
}