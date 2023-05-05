#include <gtest/gtest.h>
#include <iostream>

#include "Classes/BasicInherit.hpp"
#include "Classes/BasicTest.hpp"
#include "SevenBit/Exceptions.hpp"
#include "SevenBit/IServiceFactory.hpp"
#include "SevenBit/ServiceDescriber.hpp"
#include "SevenBit/ServiceLifeTime.hpp"
#include "SevenBit/_Internal/ServiceCtorFactory.hpp"
#include "SevenBit/_Internal/ServiceDescriptorList.hpp"

class ServiceDescriptorListTest : public ::testing::Test
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
    sb::internal::ServiceDescriptorList list;

    auto act = [&]() {
        list.add(sb::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());
        list.add(sb::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
        list.add(sb::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
    };

    EXPECT_NO_THROW((act()));
}

TEST_F(ServiceDescriptorListTest, ShouldFailAddServiceDescriptorAlreadyRegistered)
{
    sb::internal::ServiceDescriptorList list;

    list.add(sb::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());
    list.add(sb::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    auto act = [&]() { list.add(sb::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>()); };

    EXPECT_THROW((act()), sb::ServiceAlreadyRegisteredException);
}

TEST_F(ServiceDescriptorListTest, ShouldFailAddServiceDescriptorLifeTimeMismatch)
{
    sb::internal::ServiceDescriptorList list;

    list.add(sb::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());
    list.add(sb::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    auto act = [&]() { list.add(sb::ServiceDescriber::describeScoped<TestInheritClass1, TestInheritClass5>()); };

    EXPECT_THROW((act()), sb::ServiceLifeTimeMissmatchException);
}

TEST_F(ServiceDescriptorListTest, ShouldFailAddServiceDescriptorBaseTypeMismatch)
{
    sb::internal::ServiceDescriptorList list;

    list.add(sb::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());
    list.add(sb::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    auto act = [&]() { list.add(sb::ServiceDescriber::describeScoped<TestInheritClass2, TestInheritClass5>()); };

    EXPECT_THROW((act()), sb::ServiceBaseTypeMissmatchException);
}

TEST_F(ServiceDescriptorListTest, ShouldContainDescriptor)
{
    sb::internal::ServiceDescriptorList list;

    list.add(sb::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());
    list.add(sb::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
    list.add(sb::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());

    EXPECT_TRUE(list.contains(typeid(TestInheritClass5)));
    EXPECT_TRUE(list.contains(typeid(TestInheritClass4)));
    EXPECT_TRUE(list.contains(typeid(TestInheritClass3)));
    EXPECT_FALSE(list.contains(typeid(TestInheritClass2)));
}

TEST_F(ServiceDescriptorListTest, ShouldReturnProperSize)
{
    sb::internal::ServiceDescriptorList list;

    list.add(sb::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());
    list.add(sb::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
    list.add(sb::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());

    EXPECT_EQ(list.size(), 3);
}

TEST_F(ServiceDescriptorListTest, ShouldReturnProperEmpty)
{
    sb::internal::ServiceDescriptorList list;

    list.add(sb::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());
    list.add(sb::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
    list.add(sb::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());

    EXPECT_FALSE(list.empty());
}