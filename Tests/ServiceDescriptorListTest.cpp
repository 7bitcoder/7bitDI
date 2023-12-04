#include <gtest/gtest.h>
#include <iostream>

#include "Classes/BasicInherit.hpp"
#include "Classes/BasicTest.hpp"
#include "SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Details/Factories/ServiceCtorFactory.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceFactory.hpp"
#include "SevenBit/DI/ServiceDescriber.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"

class ServiceDescriptorListTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceDescriptorListTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceDescriptorListTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(ServiceDescriptorListTest, ShouldAddServiceDescriptors)
{
    sb::di::details::containers::ServiceDescriptorList list(
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());

    auto act = [&]() {
        list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
        list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
    };

    EXPECT_NO_THROW((act()));
}

TEST_F(ServiceDescriptorListTest, ShouldFailAddServiceDescriptorLifeTimeMismatch)
{
    sb::di::details::containers::ServiceDescriptorList list(
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());

    list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    auto act = [&]() { list.add(sb::di::ServiceDescriber::describeScoped<TestInheritClass1, TestInheritClass5>()); };

    EXPECT_THROW((act()), sb::di::ServiceLifeTimeMissmatchException);
}

TEST_F(ServiceDescriptorListTest, ShouldFailAddServiceDescriptorBaseTypeMismatch)
{
    sb::di::details::containers::ServiceDescriptorList list(
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());

    list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    auto act = [&]() { list.add(sb::di::ServiceDescriber::describeScoped<TestInheritClass2, TestInheritClass5>()); };

    EXPECT_THROW((act()), sb::di::ServiceBaseTypeMissmatchException);
}

TEST_F(ServiceDescriptorListTest, ShouldReturnProperSize)
{
    sb::di::details::containers::ServiceDescriptorList list(
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());

    list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
    list.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());

    EXPECT_EQ(list.size(), 3);
}
