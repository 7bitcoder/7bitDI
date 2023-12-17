#include <gtest/gtest.h>
#include <memory>

#include "../Helpers/Classes/Basic.hpp"
#include "../Helpers/Classes/Complex.hpp"
#include "SevenBit/DI/Details/Factories/ServiceFactory.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"

class ServiceDescriptorTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceDescriptorTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceDescriptorTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ServiceDescriptorTest, ShouldConstructDescriptor)
{
    auto factory = std::make_unique<sb::di::details::factories::ServiceFactory<TestClass1>>();
    auto act = [&] {
        sb::di::ServiceDescriptor descriptor{typeid(TestClass1), sb::di::ServiceLifeTime::singleton(),
                                             std::move(factory)};
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceDescriptorTest, ShouldFailConstructDescriptor)
{
    auto act = [&] {
        sb::di::ServiceDescriptor descriptor{typeid(TestClass1), sb::di::ServiceLifeTime::singleton(), nullptr};
    };

    EXPECT_THROW(act(), sb::di::NullPointerException);
}

TEST_F(ServiceDescriptorTest, ShouldGetProperInfoFromDescriptor)
{
    auto factory = std::make_unique<sb::di::details::factories::ServiceFactory<TestClass1>>();
    const auto factoryPtr = factory.get();
    const sb::di::ServiceDescriptor descriptor{typeid(TestClass1), sb::di::ServiceLifeTime::singleton(),
                                               std::move(factory)};

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
    EXPECT_EQ(&descriptor.getImplementationFactory(), factoryPtr);
}

TEST_F(ServiceDescriptorTest, ShouldCompareDescriptors)
{
    auto factory = std::make_unique<sb::di::details::factories::ServiceFactory<TestClass1>>();
    const sb::di::ServiceDescriptor descriptor{typeid(TestClass1), sb::di::ServiceLifeTime::singleton(),
                                               std::move(factory)};

    auto factory2 = std::make_unique<sb::di::details::factories::ServiceFactory<TestClass1>>();
    const sb::di::ServiceDescriptor descriptor2{typeid(TestClass1), sb::di::ServiceLifeTime::singleton(),
                                                std::move(factory2)};

    const auto &descriptorCopy = descriptor;
    EXPECT_EQ(descriptor, descriptorCopy);
    EXPECT_NE(descriptor, descriptor2);
}
