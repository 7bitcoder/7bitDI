#include <gtest/gtest.h>
#include <iostream>
#include <memory>

#include "Classes/BasicInherit.hpp"
#include "Classes/BasicTest.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceDescriber.hpp"

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
    auto descriptor = sb::di::ServiceDescriber::describeSingleton<TestClass1>();

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonCtorInterfaceService)
{
    auto descriptor = sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>();

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeScopedCtorService)
{
    auto descriptor = sb::di::ServiceDescriber::describeScoped<TestClass1>();

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeScopedCtorInterfaceService)
{
    auto descriptor = sb::di::ServiceDescriber::describeScoped<TestInheritClass1, TestInheritClass5>();

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeTransientCtorService)
{
    auto descriptor = sb::di::ServiceDescriber::describeTransient<TestClass1>();

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeTransientCtorInterfaceService)
{
    auto descriptor = sb::di::ServiceDescriber::describeTransient<TestInheritClass1, TestInheritClass5>();

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonFcnService)
{
    auto descriptor =
        sb::di::ServiceDescriber::describeSingletonFrom([](auto &) { return std::make_unique<TestClass1>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonEmptyFcnService)
{
    auto descriptor = sb::di::ServiceDescriber::describeSingletonFrom([]() { return std::make_unique<TestClass1>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonFcnInterfaceService)
{
    auto descriptor = sb::di::ServiceDescriber::describeSingletonFrom<TestInheritClass1>(
        [](auto &) { return std::make_unique<TestInheritClass5>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonEmptyFcnInterfaceService)
{
    auto descriptor = sb::di::ServiceDescriber::describeSingletonFrom<TestInheritClass1>(
        []() { return std::make_unique<TestInheritClass5>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeScopedFcnService)
{
    auto descriptor =
        sb::di::ServiceDescriber::describeScopedFrom([](auto &) { return std::make_unique<TestClass1>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeScopedEmptyFcnService)
{
    auto descriptor = sb::di::ServiceDescriber::describeScopedFrom([]() { return std::make_unique<TestClass1>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeScopedFcnInterfaceService)
{
    auto descriptor = sb::di::ServiceDescriber::describeScopedFrom<TestInheritClass1>(
        [](auto &) { return std::make_unique<TestInheritClass5>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeScopedEmptyFcnInterfaceService)
{
    auto descriptor = sb::di::ServiceDescriber::describeScopedFrom<TestInheritClass1>(
        []() { return std::make_unique<TestInheritClass5>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeTransientFcnService)
{
    auto descriptor =
        sb::di::ServiceDescriber::describeTransientFrom([](auto &) { return std::make_unique<TestClass1>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeEmptyTransientFcnService)
{
    auto descriptor = sb::di::ServiceDescriber::describeTransientFrom([]() { return std::make_unique<TestClass1>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeTransientFcnInterfaceService)
{
    auto descriptor = sb::di::ServiceDescriber::describeTransientFrom<TestInheritClass1>(
        [](auto &) { return std::make_unique<TestInheritClass5>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeTransientEmptyFcnInterfaceService)
{
    auto descriptor = sb::di::ServiceDescriber::describeTransientFrom<TestInheritClass1>(
        []() { return std::make_unique<TestInheritClass5>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonExternalService)
{
    TestClass1 test;
    auto descriptor = sb::di::ServiceDescriber::describeSingleton<TestClass1>(&test);

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonExternalInterfaceService)
{
    TestInheritClass5 test;
    auto descriptor = sb::di::ServiceDescriber::describeSingleton<TestInheritClass1>(&test);

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}