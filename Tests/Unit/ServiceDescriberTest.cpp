#include <gtest/gtest.h>
#include <memory>

#include "../Helpers/Classes/Basic.hpp"
#include "../Helpers/Classes/Inherit.hpp"
#include <SevenBit/DI/Exceptions.hpp>
#include <SevenBit/DI/ServiceDescriber.hpp>
#include <SevenBit/DI/ServiceProvider.hpp>

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
    EXPECT_FALSE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonCtorInterfaceService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>();

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_FALSE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeScopedCtorService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeScoped<TestClass1>();

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
    EXPECT_FALSE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeScopedCtorInterfaceService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeScoped<TestInheritClass1, TestInheritClass5>();

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_FALSE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeTransientCtorService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeTransient<TestClass1>();

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
    EXPECT_FALSE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeTransientCtorInterfaceService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeTransient<TestInheritClass1, TestInheritClass5>();

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_FALSE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonExternalService)
{
    TestClass1 test;
    const auto descriptor = sb::di::ServiceDescriber::describeSingleton<TestClass1>(&test);

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
    EXPECT_FALSE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonExternalInterfaceService)
{
    TestInheritClass5 test;
    const auto descriptor = sb::di::ServiceDescriber::describeSingleton<TestInheritClass1>(&test);

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_FALSE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonFcnService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeSingletonFrom(
        [](sb::di::ServiceProvider &sp, TestClass2 *cl2, std::unique_ptr<TestClass3> cl3,
           std::vector<TestClass4 *> cls4,
           std::vector<std::unique_ptr<TestClass5>> cls5) { return std::make_unique<TestClass1>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
    EXPECT_FALSE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonEmptyFcnService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeSingletonFrom([] { return TestClass1(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
    EXPECT_FALSE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonFcnInterfaceService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeSingletonFrom<TestInheritClass1>(
        [](sb::di::ServiceProvider &sp, TestClass2 *cl2, std::unique_ptr<TestClass3> cl3,
           std::vector<TestClass4 *> cls4,
           std::vector<std::unique_ptr<TestClass5>> cls5) { return std::make_unique<TestInheritClass5>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_FALSE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeSingletonEmptyFcnInterfaceService)
{
    const auto descriptor =
        sb::di::ServiceDescriber::describeSingletonFrom<TestInheritClass1>([] { return TestInheritClass5(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_FALSE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeScopedFcnService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeScopedFrom(
        [](sb::di::ServiceProvider &sp, TestClass2 *cl2, std::unique_ptr<TestClass3> cl3,
           std::vector<TestClass4 *> cls4,
           std::vector<std::unique_ptr<TestClass5>> cls5) { return std::make_unique<TestClass1>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
    EXPECT_FALSE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeScopedEmptyFcnService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeScopedFrom([] { return TestClass1(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
    EXPECT_FALSE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeScopedFcnInterfaceService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeScopedFrom<TestInheritClass1>(
        [](sb::di::ServiceProvider &sp, TestClass2 *cl2, std::unique_ptr<TestClass3> cl3,
           std::vector<TestClass4 *> cls4,
           std::vector<std::unique_ptr<TestClass5>> cls5) { return std::make_unique<TestInheritClass5>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_FALSE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeScopedEmptyFcnInterfaceService)
{
    const auto descriptor =
        sb::di::ServiceDescriber::describeScopedFrom<TestInheritClass1>([] { return TestInheritClass5(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_FALSE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeTransientFcnService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeTransientFrom(
        [](sb::di::ServiceProvider &sp, TestClass2 *cl2, std::unique_ptr<TestClass3> cl3,
           std::vector<TestClass4 *> cls4,
           std::vector<std::unique_ptr<TestClass5>> cls5) { return std::make_unique<TestClass1>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
    EXPECT_FALSE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeEmptyTransientFcnService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeTransientFrom([] { return TestClass1(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
    EXPECT_FALSE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeTransientFcnInterfaceService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeTransientFrom<TestInheritClass1>(
        [](sb::di::ServiceProvider &sp, TestClass2 *cl2, std::unique_ptr<TestClass3> cl3,
           const std::vector<TestClass4 *> &cls4,
           const std::vector<std::unique_ptr<TestClass5>> &cls5) { return std::make_unique<TestInheritClass5>(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_FALSE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeTransientEmptyFcnInterfaceService)
{
    const auto descriptor =
        sb::di::ServiceDescriber::describeTransientFrom<TestInheritClass1>([] { return TestInheritClass5(); });

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_FALSE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeAliasService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass5>();

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_FALSE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_TRUE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeKeyedSingletonCtorService)
{
    const auto descriptor =
        sb::di::ServiceDescriber::describeSingleton<TestClass1>(std::make_unique<std::string>("TestClass1Key"));

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
    EXPECT_TRUE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_EQ(*descriptor.getServiceKey(), "TestClass1Key");
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeKeyedSingletonCtorInterfaceService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>(
        std::make_unique<std::string>("TestInheritClass1Key"));

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_TRUE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_EQ(*descriptor.getServiceKey(), "TestInheritClass1Key");
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeKeyedScopedCtorService)
{
    const auto descriptor =
        sb::di::ServiceDescriber::describeScoped<TestClass1>(std::make_unique<std::string>("TestClass1Key"));

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
    EXPECT_TRUE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_EQ(*descriptor.getServiceKey(), "TestClass1Key");
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeKeyedScopedCtorInterfaceService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeScoped<TestInheritClass1, TestInheritClass5>(
        std::make_unique<std::string>("TestInheritClass1Key"));

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_TRUE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_EQ(*descriptor.getServiceKey(), "TestInheritClass1Key");
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeKeyedTransientCtorService)
{
    const auto descriptor =
        sb::di::ServiceDescriber::describeTransient<TestClass1>(std::make_unique<std::string>("TestClass1Key"));

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
    EXPECT_TRUE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_EQ(*descriptor.getServiceKey(), "TestClass1Key");
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeKeyedTransientCtorInterfaceService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeTransient<TestInheritClass1, TestInheritClass5>(
        std::make_unique<std::string>("TestInheritClass1Key"));

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_TRUE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_EQ(*descriptor.getServiceKey(), "TestInheritClass1Key");
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeKeyedSingletonExternalService)
{
    TestClass1 test;
    const auto descriptor =
        sb::di::ServiceDescriber::describeSingleton<TestClass1>(&test, std::make_unique<std::string>("TestClass1Key"));

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
    EXPECT_TRUE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_EQ(*descriptor.getServiceKey(), "TestClass1Key");
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeKeyedSingletonExternalInterfaceService)
{
    TestInheritClass5 test;
    const auto descriptor = sb::di::ServiceDescriber::describeSingleton<TestInheritClass1>(
        &test, std::make_unique<std::string>("TestInheritClass1Key"));

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_TRUE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_EQ(*descriptor.getServiceKey(), "TestInheritClass1Key");
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeKeyedSingletonFcnService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeSingletonFrom(
        [](sb::di::ServiceProvider &sp, TestClass2 *cl2, std::unique_ptr<TestClass3> cl3,
           std::vector<TestClass4 *> cls4,
           std::vector<std::unique_ptr<TestClass5>> cls5) { return std::make_unique<TestClass1>(); },
        std::make_unique<std::string>("TestClass1Key"));

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
    EXPECT_TRUE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_EQ(*descriptor.getServiceKey(), "TestClass1Key");
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeKeyedSingletonEmptyFcnService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeSingletonFrom(
        [] { return TestClass1(); }, std::make_unique<std::string>("TestClass1Key"));

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
    EXPECT_TRUE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_EQ(*descriptor.getServiceKey(), "TestClass1Key");
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeKeyedSingletonFcnInterfaceService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeSingletonFrom<TestInheritClass1>(
        [](sb::di::ServiceProvider &sp, TestClass2 *cl2, std::unique_ptr<TestClass3> cl3,
           std::vector<TestClass4 *> cls4,
           std::vector<std::unique_ptr<TestClass5>> cls5) { return std::make_unique<TestInheritClass5>(); },
        std::make_unique<std::string>("TestInheritClass1Key"));

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_TRUE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_EQ(*descriptor.getServiceKey(), "TestInheritClass1Key");
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeKeyedSingletonEmptyFcnInterfaceService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeSingletonFrom<TestInheritClass1>(
        [] { return TestInheritClass5(); }, std::make_unique<std::string>("TestInheritClass1Key"));

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_TRUE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_EQ(*descriptor.getServiceKey(), "TestInheritClass1Key");
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeKeyedScopedFcnService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeScopedFrom(
        [](sb::di::ServiceProvider &sp, TestClass2 *cl2, std::unique_ptr<TestClass3> cl3,
           std::vector<TestClass4 *> cls4,
           std::vector<std::unique_ptr<TestClass5>> cls5) { return std::make_unique<TestClass1>(); },
        std::make_unique<std::string>("TestClass1Key"));

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
    EXPECT_TRUE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_EQ(*descriptor.getServiceKey(), "TestClass1Key");
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeKeyedScopedEmptyFcnService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeScopedFrom(
        [] { return TestClass1(); }, std::make_unique<std::string>("TestClass1Key"));

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
    EXPECT_TRUE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_EQ(*descriptor.getServiceKey(), "TestClass1Key");
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeKeyedScopedFcnInterfaceService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeScopedFrom<TestInheritClass1>(
        [](sb::di::ServiceProvider &sp, TestClass2 *cl2, std::unique_ptr<TestClass3> cl3,
           std::vector<TestClass4 *> cls4,
           std::vector<std::unique_ptr<TestClass5>> cls5) { return std::make_unique<TestInheritClass5>(); },
        std::make_unique<std::string>("TestInheritClass1Key"));

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_TRUE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_EQ(*descriptor.getServiceKey(), "TestInheritClass1Key");
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeKeyedScopedEmptyFcnInterfaceService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeScopedFrom<TestInheritClass1>(
        [] { return TestInheritClass5(); }, std::make_unique<std::string>("TestInheritClass1Key"));

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_TRUE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_EQ(*descriptor.getServiceKey(), "TestInheritClass1Key");
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeKeyedTransientFcnService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeTransientFrom(
        [](sb::di::ServiceProvider &sp, TestClass2 *cl2, std::unique_ptr<TestClass3> cl3,
           std::vector<TestClass4 *> cls4,
           std::vector<std::unique_ptr<TestClass5>> cls5) { return std::make_unique<TestClass1>(); },
        std::make_unique<std::string>("TestClass1Key"));

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
    EXPECT_TRUE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_EQ(*descriptor.getServiceKey(), "TestClass1Key");
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeEmptyKeyedTransientFcnService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeTransientFrom(
        [] { return TestClass1(); }, std::make_unique<std::string>("TestClass1Key"));

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestClass1));
    EXPECT_TRUE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_EQ(*descriptor.getServiceKey(), "TestClass1Key");
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeKeyedTransientFcnInterfaceService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeTransientFrom<TestInheritClass1>(
        [](sb::di::ServiceProvider &sp, TestClass2 *cl2, std::unique_ptr<TestClass3> cl3,
           const std::vector<TestClass4 *> &cls4,
           const std::vector<std::unique_ptr<TestClass5>> &cls5) { return std::make_unique<TestInheritClass5>(); },
        std::make_unique<std::string>("TestInheritClass1Key"));

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_TRUE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_EQ(*descriptor.getServiceKey(), "TestInheritClass1Key");
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeKeyedTransientEmptyFcnInterfaceService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeTransientFrom<TestInheritClass1>(
        [] { return TestInheritClass5(); }, std::make_unique<std::string>("TestInheritClass1Key"));

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_TRUE(descriptor.getServiceKey());
    EXPECT_FALSE(descriptor.getImplementationKey());
    EXPECT_EQ(*descriptor.getServiceKey(), "TestInheritClass1Key");
    EXPECT_FALSE(descriptor.isAlias());
}

TEST_F(ServiceDescriberTest, ShouldDescribeKeyedAliasService)
{
    const auto descriptor = sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass5>(
        std::make_unique<std::string>("TestInheritClass1Key"), std::make_unique<std::string>("TestInheritClass5Key"));

    EXPECT_EQ(descriptor.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(descriptor.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(descriptor.getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_TRUE(descriptor.getServiceKey());
    EXPECT_TRUE(descriptor.getImplementationKey());
    EXPECT_EQ(*descriptor.getServiceKey(), "TestInheritClass1Key");
    EXPECT_EQ(*descriptor.getImplementationKey(), "TestInheritClass5Key");
    EXPECT_TRUE(descriptor.isAlias());
}
