#include <gtest/gtest.h>

#include "../../Helpers/Classes/Basic.hpp"
#include "../../Helpers/Classes/Inherit.hpp"
#include "SevenBit/DI/Details/Services/ExternalService.hpp"
#include "SevenBit/DI/Details/Services/InPlaceService.hpp"
#include "SevenBit/DI/Details/Services/UniquePtrService.hpp"
#include "SevenBit/DI/Details/Utils/ExtRequire.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceDescriber.hpp"

class ExtRequireTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ExtRequireTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ExtRequireTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ExtRequireTest, ShoulRequireValidInstance)
{
    TestClass1 test;
    EXPECT_THROW(sb::di::details::ExtRequire::validInstance(nullptr), sb::di::NullPointerException);
    EXPECT_THROW(sb::di::details::ExtRequire::validInstance(sb::di::ServiceInstance{}),
                 sb::di::InvalidServiceException);
    EXPECT_THROW(sb::di::details::ExtRequire::validInstance(
                     sb::di::ServiceInstance{std::make_unique<sb::di::details::ExternalService<TestClass1>>(nullptr)}),
                 sb::di::InvalidServiceException);
    EXPECT_NO_THROW(sb::di::details::ExtRequire::validInstance(
        sb::di::ServiceInstance{std::make_unique<sb::di::details::ExternalService<TestClass1>>(&test)}));
    EXPECT_THROW(sb::di::details::ExtRequire::validInstance(
                     sb::di::ServiceInstance{std::make_unique<sb::di::details::UniquePtrService<TestClass1>>(nullptr)}),
                 sb::di::InvalidServiceException);
    EXPECT_NO_THROW(sb::di::details::ExtRequire::validInstance(sb::di::ServiceInstance{
        std::make_unique<sb::di::details::UniquePtrService<TestClass1>>(std::make_unique<TestClass1>())}));
    EXPECT_NO_THROW(sb::di::details::ExtRequire::validInstance(
        sb::di::ServiceInstance{std::make_unique<sb::di::details::InPlaceService<TestClass1>>()}));
}

TEST_F(ExtRequireTest, ShoulRequireTransientDescriptors)
{
    EXPECT_THROW(sb::di::details::ExtRequire::transientDescriptors(
                     sb::di::details::ServiceDescriptorList{sb::di::ServiceDescriber::describeSingleton<TestClass1>()}),
                 sb::di::InjectorException);

    EXPECT_THROW(sb::di::details::ExtRequire::transientDescriptors(
                     sb::di::details::ServiceDescriptorList{sb::di::ServiceDescriber::describeScoped<TestClass1>()}),
                 sb::di::InjectorException);

    EXPECT_NO_THROW(sb::di::details::ExtRequire::transientDescriptors(
        sb::di::details::ServiceDescriptorList{sb::di::ServiceDescriber::describeTransient<TestClass1>()}));
}

TEST_F(ExtRequireTest, ShoulRequireNonTransientDescriptors)
{
    EXPECT_NO_THROW(sb::di::details::ExtRequire::nonTransientDescriptors(
        sb::di::details::ServiceDescriptorList{sb::di::ServiceDescriber::describeSingleton<TestClass1>()}));

    EXPECT_NO_THROW(sb::di::details::ExtRequire::nonTransientDescriptors(
        sb::di::details::ServiceDescriptorList{sb::di::ServiceDescriber::describeScoped<TestClass1>()}));

    EXPECT_THROW(sb::di::details::ExtRequire::nonTransientDescriptors(
                     sb::di::details::ServiceDescriptorList{sb::di::ServiceDescriber::describeTransient<TestClass1>()}),
                 sb::di::InjectorException);
}

TEST_F(ExtRequireTest, ShoulRequireAliasDescriptor)
{
    EXPECT_THROW(
        sb::di::details::ExtRequire::aliasDescriptor(sb::di::ServiceDescriber::describeSingleton<TestClass1>()),
        sb::di::InjectorException);

    EXPECT_NO_THROW(sb::di::details::ExtRequire::aliasDescriptor(
        sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass2>()));
}

TEST_F(ExtRequireTest, ShoulRequireNonAliasDescriptor)
{
    EXPECT_NO_THROW(
        sb::di::details::ExtRequire::nonAliasDescriptor(sb::di::ServiceDescriber::describeSingleton<TestClass1>()));

    EXPECT_THROW(sb::di::details::ExtRequire::nonAliasDescriptor(
                     sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass2>()),
                 sb::di::InjectorException);
}
