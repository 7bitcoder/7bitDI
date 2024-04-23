#include <gtest/gtest.h>
#include <memory>

#include "../../Helpers/Classes/Basic.hpp"
#include "../../Helpers/Classes/Inherit.hpp"
#include "../../Helpers/Mocks/ServiceProviderMock.hpp"
#include <SevenBit/DI/Details/Core/ServiceAliasesCreator.hpp>
#include <SevenBit/DI/ServiceDescriber.hpp>

class ServiceAliasesCreatorTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    ServiceAliasesCreatorTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceAliasesCreatorTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ServiceAliasesCreatorTest, ShouldFailCreateAliasForInvalidService)
{
    constexpr sb::di::details::ServiceAliasesCreator creator;

    const auto descriptor = sb::di::ServiceDescriber::describeAlias<TestInheritClass3, TestInheritClass5>();

    const sb::di::ServiceInstance external;

    EXPECT_THROW(creator.tryCreate(descriptor, &external), sb::di::InvalidServiceException);
}

TEST_F(ServiceAliasesCreatorTest, ShouldFailCreateAliasForNullService)
{
    constexpr sb::di::details::ServiceAliasesCreator creator;

    const auto descriptor = sb::di::ServiceDescriber::describeAlias<TestInheritClass3, TestInheritClass5>();

    TestInheritClass5 *test = nullptr;
    const sb::di::ServiceInstance external{std::make_unique<sb::di::details::ExternalService<TestInheritClass5>>(test)};

    EXPECT_THROW(creator.tryCreate(descriptor, &external), sb::di::InvalidServiceException);
}

TEST_F(ServiceAliasesCreatorTest, ShouldFailCreateAliasForWrongDescriptor)
{
    constexpr sb::di::details::ServiceAliasesCreator creator;

    const auto descriptor = sb::di::ServiceDescriber::describeSingleton<TestInheritClass3, TestInheritClass5>();

    TestInheritClass5 *test = nullptr;
    const sb::di::ServiceInstance external{std::make_unique<sb::di::details::ExternalService<TestInheritClass5>>(test)};

    EXPECT_THROW(creator.tryCreate(descriptor, &external), sb::di::InjectorException);
}

TEST_F(ServiceAliasesCreatorTest, ShouldCreate)
{
    constexpr sb::di::details::ServiceAliasesCreator creator;

    TestInheritClass6 test;
    const sb::di::ServiceInstance external{
        std::make_unique<sb::di::details::ExternalService<TestInheritClass6>>(&test)};

    const auto instance =
        creator.tryCreate(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass5>(), &external);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance.isValid());
    EXPECT_TRUE(instance.getAs<void>());
    EXPECT_EQ(instance.tryGetImplementation()->getTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceAliasesCreatorTest, ShouldNotCreateForNullOriginal)
{
    constexpr sb::di::details::ServiceAliasesCreator creator;

    TestInheritClass6 test;
    const sb::di::ServiceInstance external{
        std::make_unique<sb::di::details::ExternalService<TestInheritClass6>>(&test)};

    const auto instance =
        creator.tryCreate(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass5>(), nullptr);

    EXPECT_FALSE(instance);
    EXPECT_FALSE(instance.isValid());
}

TEST_F(ServiceAliasesCreatorTest, ShouldCreateAllAliases)
{
    sb::di::details::ServiceDescriptorList descriptors{
        sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass2>()};

    constexpr sb::di::details::ServiceAliasesCreator creator;

    TestInheritClass3 test3;
    TestInheritClass4 test4;
    TestInheritClass5 test5;
    sb::di::details::ServiceInstanceList externals{
        sb::di::ServiceInstance{std::make_unique<sb::di::details::ExternalService<TestInheritClass3>>(&test3)}};
    externals.add(
        sb::di::ServiceInstance{std::make_unique<sb::di::details::ExternalService<TestInheritClass4>>(&test4)});
    externals.add(
        sb::di::ServiceInstance{std::make_unique<sb::di::details::ExternalService<TestInheritClass5>>(&test5)});

    const auto instances =
        creator.tryCreateAll(descriptors, [&](const sb::di::ServiceDescriptor &) { return &externals; });

    EXPECT_EQ(instances.size(), 3);
    EXPECT_FALSE(instances.isSealed());
    EXPECT_TRUE(instances[0].isValid());
    EXPECT_TRUE(instances[0].getAs<void>());
    EXPECT_EQ(instances[0].tryGetImplementation()->getTypeId(), typeid(TestInheritClass2));
    EXPECT_TRUE(instances[1].isValid());
    EXPECT_TRUE(instances[1].getAs<void>());
    EXPECT_EQ(instances[1].tryGetImplementation()->getTypeId(), typeid(TestInheritClass2));
    EXPECT_TRUE(instances[2].isValid());
    EXPECT_TRUE(instances[2].getAs<void>());
    EXPECT_EQ(instances[2].tryGetImplementation()->getTypeId(), typeid(TestInheritClass2));
}

TEST_F(ServiceAliasesCreatorTest, ShouldNotCreateAllAliasesForEmpty)
{
    sb::di::details::ServiceDescriptorList descriptors;

    constexpr sb::di::details::ServiceAliasesCreator creator;

    sb::di::details::ServiceInstanceList externals;

    const auto instances =
        creator.tryCreateAll(descriptors, [&](const sb::di::ServiceDescriptor &) { return &externals; });

    EXPECT_FALSE(instances);
    EXPECT_TRUE(instances.empty());
}

TEST_F(ServiceAliasesCreatorTest, ShouldNotCreateAllAliasesForNull)
{
    sb::di::details::ServiceDescriptorList descriptors{
        sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass2>()};

    constexpr sb::di::details::ServiceAliasesCreator creator;
    const auto instances = creator.tryCreateAll(
        descriptors,
        [&](const sb::di::ServiceDescriptor &) -> sb::di::details::ServiceInstanceList * { return nullptr; });

    EXPECT_FALSE(instances);
    EXPECT_TRUE(instances.empty());
}

TEST_F(ServiceAliasesCreatorTest, ShouldCreateRestAliases)
{
    sb::di::details::ServiceDescriptorList descriptors{
        sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass2>()};

    constexpr sb::di::details::ServiceAliasesCreator creator;

    TestInheritClass3 test3;
    TestInheritClass4 test4;
    TestInheritClass5 test5;
    sb::di::details::ServiceInstanceList externals{
        sb::di::ServiceInstance{std::make_unique<sb::di::details::ExternalService<TestInheritClass3>>(&test3)}};
    externals.add(
        sb::di::ServiceInstance{std::make_unique<sb::di::details::ExternalService<TestInheritClass4>>(&test4)});
    externals.add(
        sb::di::ServiceInstance{std::make_unique<sb::di::details::ExternalService<TestInheritClass5>>(&test5)});

    sb::di::details::ServiceInstanceList instances{
        sb::di::ServiceInstance{std::make_unique<sb::di::details::ExternalService<TestInheritClass2>>(&test5)}};

    creator.tryCreateRest(descriptors, instances, [&](const sb::di::ServiceDescriptor &) { return &externals; });

    EXPECT_EQ(instances.size(), 3);
    EXPECT_FALSE(instances.isSealed());
    EXPECT_TRUE(instances[0].isValid());
    EXPECT_TRUE(instances[0].getAs<void>());
    EXPECT_EQ(instances[0].tryGetImplementation()->getTypeId(), typeid(TestInheritClass2));
    EXPECT_TRUE(instances[1].isValid());
    EXPECT_TRUE(instances[1].getAs<void>());
    EXPECT_EQ(instances[1].tryGetImplementation()->getTypeId(), typeid(TestInheritClass2));
    EXPECT_TRUE(instances[2].isValid());
    EXPECT_TRUE(instances[2].getAs<void>());
    EXPECT_EQ(instances[2].tryGetImplementation()->getTypeId(), typeid(TestInheritClass2));
}
