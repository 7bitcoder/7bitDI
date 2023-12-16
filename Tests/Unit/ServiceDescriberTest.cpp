#include <gtest/gtest.h>
#include <memory>

#include "../Helpers/Classes/Basic.hpp"
#include "../Helpers/Classes/Inherit.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceDescriber.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

class ServiceDescriberTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceDescriberTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceDescriberTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonCtorService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeSingleton<TestClass1>();

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonCtorInterfaceService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>();

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeScopedCtorService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeScoped<TestClass1>();

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeScopedCtorInterfaceService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeScoped<TestInheritClass1, TestInheritClass5>();

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeTransientCtorService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeTransient<TestClass1>();

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeTransientCtorInterfaceService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeTransient<TestInheritClass1, TestInheritClass5>();

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonExternalService)
{
    TestClass1 test;
    const auto descriptor = sb::di::ServiceDescriber::describeSingleton<TestClass1>(&test);

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonExternalInterfaceService)
{
    TestInheritClass5 test;
    const auto descriptor = sb::di::ServiceDescriber::describeSingleton<TestInheritClass1>(&test);

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonFcnService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeSingletonFrom(
        [](sb::di::ServiceProvider &) { return std::make_unique<TestClass1>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonEmptyFcnService)
{
    const auto descriptor =
        sb::di::ServiceDescriber::describeSingletonFrom([] { return std::make_unique<TestClass1>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonFcnInterfaceService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeSingletonFrom<TestInheritClass1>(
        [](sb::di::ServiceProvider &) { return std::make_unique<TestInheritClass5>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonEmptyFcnInterfaceService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeSingletonFrom<TestInheritClass1>(
        [] { return std::make_unique<TestInheritClass5>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeScopedFcnService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeScopedFrom(
        [](sb::di::ServiceProvider &) { return std::make_unique<TestClass1>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeScopedEmptyFcnService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeScopedFrom([] { return std::make_unique<TestClass1>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeScopedFcnInterfaceService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeScopedFrom<TestInheritClass1>(
        [](sb::di::ServiceProvider &) { return std::make_unique<TestInheritClass5>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeScopedEmptyFcnInterfaceService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeScopedFrom<TestInheritClass1>(
        [] { return std::make_unique<TestInheritClass5>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeTransientFcnService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeTransientFrom(
        [](sb::di::ServiceProvider &) { return std::make_unique<TestClass1>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeEmptyTransientFcnService)
{
    const auto descriptor =
        sb::di::ServiceDescriber::describeTransientFrom([] { return std::make_unique<TestClass1>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceDescriberTest, ShouldDescribeTransientFcnInterfaceService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeTransientFrom<TestInheritClass1>(
        [](sb::di::ServiceProvider &) { return std::make_unique<TestInheritClass5>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceDescriberTest, ShouldDescribeTransientEmptyFcnInterfaceService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeTransientFrom<TestInheritClass1>(
        [] { return std::make_unique<TestInheritClass5>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
}
