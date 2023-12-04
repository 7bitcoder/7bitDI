#include <gtest/gtest.h>
#include <iostream>

#include "Classes/BasicInherit.hpp"
#include "SevenBit/DI/Details/Containers/ServiceDescriptorsMap.hpp"
#include "SevenBit/DI/Details/Factories/ServiceCtorFactory.hpp"
#include "SevenBit/DI/ServiceDescriber.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"
#include "SevenBit/DI/ServiceLifeTime.hpp"
class ServiceDescriptorsMapTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceDescriptorsMapTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceDescriptorsMapTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(ServiceDescriptorsMapTest, ShouldAddDescriptors)
{
    std::vector<sb::di::ServiceDescriptor> _descriptors;
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass2, TestInheritClass3>());
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass2, TestInheritClass6>());

    auto act = [&]() {
        sb::di::details::containers::ServiceDescriptorsMap map{_descriptors.begin(), _descriptors.end()};
    };

    EXPECT_NO_THROW((act()));
}

TEST_F(ServiceDescriptorsMapTest, ShouldFailAddServiceDescriptorAlreadyRegistered)
{
    std::vector<sb::di::ServiceDescriptor> _descriptors;
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass2, TestInheritClass3>());
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass2, TestInheritClass5>());

    auto act = [&]() {
        sb::di::details::containers::ServiceDescriptorsMap map{_descriptors.begin(), _descriptors.end(), true};
    };

    EXPECT_THROW((act()), sb::di::ServiceAlreadyRegisteredException);
}

TEST_F(ServiceDescriptorsMapTest, ShouldSealDescriptors)
{
    std::vector<sb::di::ServiceDescriptor> _descriptors;
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass2, TestInheritClass3>());
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass2, TestInheritClass6>());

    sb::di::details::containers::ServiceDescriptorsMap map{_descriptors.begin(), _descriptors.end()};
    auto act = [&]() { map.seal(); };

    EXPECT_NO_THROW((act()));
}

TEST_F(ServiceDescriptorsMapTest, ShouldFindDescriptorList)
{
    std::vector<sb::di::ServiceDescriptor> _descriptors;
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass2, TestInheritClass3>());
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass2, TestInheritClass5>());
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
    _descriptors.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass6>());

    sb::di::details::containers::ServiceDescriptorsMap map{_descriptors.begin(), _descriptors.end()};

    auto first = map.findDescriptors(typeid(TestInheritClass2));
    EXPECT_TRUE(first);
    EXPECT_EQ(first->size(), 2);
    //    EXPECT_EQ(first->begin()->getServiceTypeId(), typeid(TestInheritClass2));
    //    EXPECT_EQ(first->begin()->getImplementationTypeId(), typeid(TestInheritClass3));
    //    EXPECT_EQ(first->begin()->getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(first->last().getServiceTypeId(), typeid(TestInheritClass2));
    EXPECT_EQ(first->last().getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_EQ(first->last().getLifeTime(), sb::di::ServiceLifeTime::scoped());

    auto second = map.findDescriptors(typeid(TestInheritClass1));
    EXPECT_TRUE(second);
    EXPECT_EQ(second->size(), 2);
    //    EXPECT_EQ(second->begin()->getServiceTypeId(), typeid(TestInheritClass1));
    //    EXPECT_EQ(second->begin()->getImplementationTypeId(), typeid(TestInheritClass4));
    //    EXPECT_EQ(second->begin()->getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(second->last().getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(second->last().getImplementationTypeId(), typeid(TestInheritClass6));
    EXPECT_EQ(second->last().getLifeTime(), sb::di::ServiceLifeTime::singleton());

    EXPECT_FALSE(map.findDescriptors(typeid(TestInheritClass3)));
}
