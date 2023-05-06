#include <gtest/gtest.h>
#include <iostream>
#include <memory>

#include "Classes/BasicTest.hpp"
#include "SevenBit/Details/ServiceCtorFactory.hpp"
#include "SevenBit/Exceptions.hpp"
#include "SevenBit/ServiceDescriptor.hpp"
#include "SevenBit/ServiceLifeTime.hpp"

class ServiceDescriptorTest : public ::testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceDescriptorTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceDescriptorTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(ServiceDescriptorTest, ShouldConstructDescriptor)
{
    auto factory = std::make_unique<sb::details::ServiceCtorFactory<TestClass1>>();
    auto act = [&]() {
        sb::ServiceDescriptor descriptor{typeid(TestClass1), sb::ServiceLifeTime::singleton(), std::move(factory)};
    };

    EXPECT_NO_THROW((act()));
}

TEST_F(ServiceDescriptorTest, ShouldFailConstructDescriptor)
{
    auto act = [&]() {
        sb::ServiceDescriptor descriptor{typeid(TestClass1), sb::ServiceLifeTime::singleton(), nullptr};
    };

    EXPECT_THROW((act()), sb::NullPointnerException);
}

TEST_F(ServiceDescriptorTest, ShouldGetProperInfoFromDescriptor)
{
    auto factory = std::make_unique<sb::details::ServiceCtorFactory<TestClass1>>();
    auto factoryPtr = factory.get();
    sb::ServiceDescriptor descriptor{typeid(TestClass1), sb::ServiceLifeTime::singleton(), std::move(factory)};

    EXPECT_EQ(descriptor.getLifeTime(), sb::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
    EXPECT_EQ(&descriptor.getImplementationFactory(), factoryPtr);
}