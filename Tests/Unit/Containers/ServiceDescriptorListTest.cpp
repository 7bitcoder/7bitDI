#include <gtest/gtest.h>

#include "../../Helpers/Classes/Inherit.hpp"
#include <SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp>
#include <SevenBit/DI/Exceptions.hpp>
#include <SevenBit/DI/ServiceDescriber.hpp>

class ServiceDescriptorListTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceDescriptorListTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceDescriptorListTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ServiceDescriptorListTest, ShouldAddServiceDescriptors)
{
    sb::di::details::ServiceDescriptorList list(
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());

    auto act = [&] {
        list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
        list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceDescriptorListTest, ShouldAddServiceAliasDescriptors)
{
    sb::di::details::ServiceDescriptorList list(
        sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass3>());

    auto act = [&] {
        list.add(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass4>());
        list.add(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass5>());
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceDescriptorListTest, ShouldFailAddServiceDescriptorAliasMismatch)
{
    sb::di::details::ServiceDescriptorList list(
        sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass3>());

    list.add(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass4>());

    auto act = [&] { list.add(sb::di::ServiceDescriber::describeScoped<TestInheritClass1, TestInheritClass5>()); };

    EXPECT_THROW(act(), sb::di::ServiceAliasMismatchException);
}

TEST_F(ServiceDescriptorListTest, ShouldFailAddServiceDescriptorAliasMismatchOpposite)
{
    sb::di::details::ServiceDescriptorList list(
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());

    list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    auto act = [&] { list.add(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass5>()); };

    EXPECT_THROW(act(), sb::di::ServiceAliasMismatchException);
}

TEST_F(ServiceDescriptorListTest, ShouldFailAddServiceDescriptorLifeTimeMismatch)
{
    sb::di::details::ServiceDescriptorList list(
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());

    list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    auto act = [&] { list.add(sb::di::ServiceDescriber::describeScoped<TestInheritClass1, TestInheritClass5>()); };

    EXPECT_THROW(act(), sb::di::ServiceLifeTimeMismatchException);
}

TEST_F(ServiceDescriptorListTest, ShouldFailAddServiceDescriptorBaseTypeMismatch)
{
    sb::di::details::ServiceDescriptorList list(
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());

    list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    auto act = [&] { list.add(sb::di::ServiceDescriber::describeScoped<TestInheritClass2, TestInheritClass5>()); };

    EXPECT_THROW(act(), sb::di::InjectorException);
}

TEST_F(ServiceDescriptorListTest, ShouldFailAddServiceDescriptorKeyMismatch)
{
    sb::di::details::ServiceDescriptorList list(
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>(
            std::make_unique<std::string>("key")));

    auto act = [&] { list.add(sb::di::ServiceDescriber::describeScoped<TestInheritClass2, TestInheritClass5>()); };

    EXPECT_THROW(act(), sb::di::InjectorException);
}

TEST_F(ServiceDescriptorListTest, ShouldFailAddServiceDescriptorWrongKeyMismatch)
{
    sb::di::details::ServiceDescriptorList list(
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>(
            std::make_unique<std::string>("key")));

    auto act = [&] {
        list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>(
            std::make_unique<std::string>("wrongKey")));
    };

    EXPECT_THROW(act(), sb::di::InjectorException);
}

TEST_F(ServiceDescriptorListTest, ShouldReturnProperSize)
{
    sb::di::details::ServiceDescriptorList list(
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());

    EXPECT_EQ(list.size(), 1);

    list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
    list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());

    EXPECT_EQ(list.size(), 3);
}

TEST_F(ServiceDescriptorListTest, ShouldReturnEmpty)
{

    sb::di::details::ServiceDescriptorList list(

        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());

    EXPECT_FALSE(list.empty());

    list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
    list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());

    EXPECT_FALSE(list.empty());
}

TEST_F(ServiceDescriptorListTest, ShouldGetLifeTime)
{
    sb::di::details::ServiceDescriptorList list(
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());

    list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
    list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());

    EXPECT_EQ(list.getLifeTime(), sb::di::ServiceLifeTime::singleton());
}

TEST_F(ServiceDescriptorListTest, ShouldGetServiceTypeId)
{
    sb::di::details::ServiceDescriptorList list(
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());

    list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
    list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());

    EXPECT_EQ(list.getServiceTypeId(), typeid(TestInheritClass1));
}

TEST_F(ServiceDescriptorListTest, ShouldGetServiceKey)
{
    const sb::di::details::ServiceDescriptorList list(
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>(
            std::make_unique<std::string>("key")));

    EXPECT_TRUE(list.getServiceKey());
    EXPECT_EQ(*list.getServiceKey(), "key");
}

TEST_F(ServiceDescriptorListTest, ShouldGetFirst)
{
    const auto describedFirst = sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>();
    sb::di::details::ServiceDescriptorList list(sb::di::ServiceDescriptor{describedFirst});

    list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
    list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());

    EXPECT_EQ(list.first(), describedFirst);
}

TEST_F(ServiceDescriptorListTest, ShouldGetLast)
{
    const auto describedLast = sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>();
    sb::di::details::ServiceDescriptorList list(
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());

    list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
    list.add(sb::di::ServiceDescriptor{describedLast});

    EXPECT_EQ(list.last(), describedLast);
}

TEST_F(ServiceDescriptorListTest, ShouldGetNotIsAlias)
{
    sb::di::details::ServiceDescriptorList list(
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());
    list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    EXPECT_FALSE(list.isAlias());
}

TEST_F(ServiceDescriptorListTest, ShouldGetIsAlias)
{
    sb::di::details::ServiceDescriptorList list(
        sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass3>());
    list.add(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass4>());

    EXPECT_TRUE(list.isAlias());
}
