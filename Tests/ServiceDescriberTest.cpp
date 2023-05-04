#include <gtest/gtest.h>
#include <iostream>
#include <memory>

#include "Classes/BasicInherit.hpp"
#include "Classes/BasicTest.hpp"
#include "SevenBit/Exceptions.hpp"
#include "SevenBit/ServiceDescriber.hpp"

class ServiceDescriberTest : public ::testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceDescriberTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceDescriberTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonCtorService)
{
    auto descriptor = sb::ServiceDescriber::describeSingleton<TestClass1>();

    EXPECT_EQ(descriptor.getLifeTime(), sb::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonCtorInterfaceService)
{
    auto descriptor = sb::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>();

    EXPECT_EQ(descriptor.getLifeTime(), sb::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeScopedCtorService)
{
    auto descriptor = sb::ServiceDescriber::describeScoped<TestClass1>();

    EXPECT_EQ(descriptor.getLifeTime(), sb::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeScopedCtorInterfaceService)
{
    auto descriptor = sb::ServiceDescriber::describeScoped<TestInheritClass1, TestInheritClass5>();

    EXPECT_EQ(descriptor.getLifeTime(), sb::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeTransientCtorService)
{
    auto descriptor = sb::ServiceDescriber::describeTransient<TestClass1>();

    EXPECT_EQ(descriptor.getLifeTime(), sb::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeTransientCtorInterfaceService)
{
    auto descriptor = sb::ServiceDescriber::describeTransient<TestInheritClass1, TestInheritClass5>();

    EXPECT_EQ(descriptor.getLifeTime(), sb::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonFcnService)
{
    auto descriptor =
        sb::ServiceDescriber::describeSingletonFrom([](auto &) { return std::make_unique<TestClass1>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonEmptyFcnService)
{
    auto descriptor = sb::ServiceDescriber::describeSingletonFrom([]() { return std::make_unique<TestClass1>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonFcnInterfaceService)
{
    auto descriptor = sb::ServiceDescriber::describeSingletonFrom<TestInheritClass1>(
        [](auto &) { return std::make_unique<TestInheritClass5>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonEmptyFcnInterfaceService)
{
    auto descriptor = sb::ServiceDescriber::describeSingletonFrom<TestInheritClass1>(
        []() { return std::make_unique<TestInheritClass5>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeScopedFcnService)
{
    auto descriptor = sb::ServiceDescriber::describeScopedFrom([](auto &) { return std::make_unique<TestClass1>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeScopedEmptyFcnService)
{
    auto descriptor = sb::ServiceDescriber::describeScopedFrom([]() { return std::make_unique<TestClass1>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeScopedFcnInterfaceService)
{
    auto descriptor = sb::ServiceDescriber::describeScopedFrom<TestInheritClass1>(
        [](auto &) { return std::make_unique<TestInheritClass5>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeScopedEmptyFcnInterfaceService)
{
    auto descriptor = sb::ServiceDescriber::describeScopedFrom<TestInheritClass1>(
        []() { return std::make_unique<TestInheritClass5>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeTransientFcnService)
{
    auto descriptor =
        sb::ServiceDescriber::describeTransientFrom([](auto &) { return std::make_unique<TestClass1>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeEmptyTransientFcnService)
{
    auto descriptor = sb::ServiceDescriber::describeTransientFrom([]() { return std::make_unique<TestClass1>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeTransientFcnInterfaceService)
{
    auto descriptor = sb::ServiceDescriber::describeTransientFrom<TestInheritClass1>(
        [](auto &) { return std::make_unique<TestInheritClass5>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeTransientEmptyFcnInterfaceService)
{
    auto descriptor = sb::ServiceDescriber::describeTransientFrom<TestInheritClass1>(
        []() { return std::make_unique<TestInheritClass5>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonExternalService)
{
    TestClass1 test;
    auto descriptor = sb::ServiceDescriber::describeSingleton<TestClass1>(&test);

    EXPECT_EQ(descriptor.getLifeTime(), sb::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonExternalInterfaceService)
{
    TestInheritClass5 test;
    auto descriptor = sb::ServiceDescriber::describeSingleton<TestInheritClass1>(&test);

    EXPECT_EQ(descriptor.getLifeTime(), sb::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}