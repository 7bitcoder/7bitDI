#include <gtest/gtest.h>

#include "../../Helpers/Classes/Basic.hpp"
#include "../../Helpers/Classes/Inherit.hpp"
#include <SevenBit/DI/Details/Utils/RequireDescriptor.hpp>
#include <SevenBit/DI/Exceptions.hpp>
#include <SevenBit/DI/ServiceDescriber.hpp>

class RequireDescriptorTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    RequireDescriptorTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~RequireDescriptorTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(RequireDescriptorTest, ShoulRequireTransientDescriptors)
{
    EXPECT_THROW(
        sb::di::details::RequireDescriptor::transient(sb::di::ServiceDescriber::describeSingleton<TestClass1>()),
        sb::di::InjectorException);

    EXPECT_THROW(sb::di::details::RequireDescriptor::transient(sb::di::ServiceDescriber::describeScoped<TestClass1>()),
                 sb::di::InjectorException);

    EXPECT_NO_THROW(
        sb::di::details::RequireDescriptor::transient(sb::di::ServiceDescriber::describeTransient<TestClass1>()));
}

TEST_F(RequireDescriptorTest, ShoulRequireNonTransientDescriptors)
{
    EXPECT_NO_THROW(
        sb::di::details::RequireDescriptor::nonTransient(sb::di::ServiceDescriber::describeSingleton<TestClass1>()));

    EXPECT_NO_THROW(
        sb::di::details::RequireDescriptor::nonTransient(sb::di::ServiceDescriber::describeScoped<TestClass1>()));

    EXPECT_THROW(
        sb::di::details::RequireDescriptor::nonTransient(sb::di::ServiceDescriber::describeTransient<TestClass1>()),
        sb::di::InjectorException);
}

TEST_F(RequireDescriptorTest, ShoulRequireAliasDescriptor)
{
    EXPECT_THROW(sb::di::details::RequireDescriptor::alias(sb::di::ServiceDescriber::describeSingleton<TestClass1>()),
                 sb::di::InjectorException);

    EXPECT_NO_THROW(sb::di::details::RequireDescriptor::alias(
        sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass2>()));
}

TEST_F(RequireDescriptorTest, ShoulRequireNonAliasDescriptor)
{
    EXPECT_NO_THROW(
        sb::di::details::RequireDescriptor::nonAlias(sb::di::ServiceDescriber::describeSingleton<TestClass1>()));

    EXPECT_THROW(sb::di::details::RequireDescriptor::nonAlias(
                     sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass2>()),
                 sb::di::InjectorException);
}
