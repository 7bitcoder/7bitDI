#include <gtest/gtest.h>

#include "Classes/BasicInherit.hpp"
#include "SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Details/Factories/ServiceFactory.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceDescriber.hpp"

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
    sb::di::details::containers::ServiceDescriptorList list(
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());

    auto act = [&] {
        list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
        list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceDescriptorListTest, ShouldFailAddServiceDescriptorLifeTimeMismatch)
{
    sb::di::details::containers::ServiceDescriptorList list(
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());

    list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    auto act = [&] { list.add(sb::di::ServiceDescriber::describeScoped<TestInheritClass1, TestInheritClass5>()); };

    EXPECT_THROW(act(), sb::di::ServiceLifeTimeMismatchException);
}

TEST_F(ServiceDescriptorListTest, ShouldFailAddServiceDescriptorBaseTypeMismatch)
{
    sb::di::details::containers::ServiceDescriptorList list(
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());

    list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    auto act = [&] { list.add(sb::di::ServiceDescriber::describeScoped<TestInheritClass2, TestInheritClass5>()); };

    EXPECT_THROW(act(), sb::di::ServiceBaseTypeMismatchException);
}

TEST_F(ServiceDescriptorListTest, ShouldReturnProperSize)
{
    sb::di::details::containers::ServiceDescriptorList list(
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());

    list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
    list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());

    EXPECT_EQ(list.size(), 3);
}
