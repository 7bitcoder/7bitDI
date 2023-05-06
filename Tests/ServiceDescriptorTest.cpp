#include <gtest/gtest.h>
#include <iostream>
#include <memory>

#include "Classes/BasicTest.hpp"
#include "SevenBit/DI/Details/ServiceCtorFactory.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"

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
    auto factory = std::make_unique<sb::di::details::ServiceCtorFactory<TestClass1>>();
    auto act = [&]() {
        sb::di::ServiceDescriptor descriptor{typeid(TestClass1), sb::di::ServiceLifeTime::singleton(),
                                             std::move(factory)};
    };

    EXPECT_NO_THROW((act()));
}

TEST_F(ServiceDescriptorTest, ShouldFailConstructDescriptor)
{
    auto act = [&]() {
        sb::di::ServiceDescriptor descriptor{typeid(TestClass1), sb::di::ServiceLifeTime::singleton(), nullptr};
    };

    EXPECT_THROW((act()), sb::di::NullPointnerException);
}

TEST_F(ServiceDescriptorTest, ShouldGetProperInfoFromDescriptor)
{
    auto factory = std::make_unique<sb::di::details::ServiceCtorFactory<TestClass1>>();
    auto factoryPtr = factory.get();
    sb::di::ServiceDescriptor descriptor{typeid(TestClass1), sb::di::ServiceLifeTime::singleton(), std::move(factory)};

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
    EXPECT_EQ(&descriptor.getImplementationFactory(), factoryPtr);
}