#include <gtest/gtest.h>
#include <iostream>
#include <memory>

#include "Classes/BasicInherit.hpp"
#include "Classes/BasicInheritDesctuction.hpp"
#include "Classes/BasicTest.hpp"
#include "Mocks/ServiceProviderMock.hpp"
#include "SevenBit/DI/Details/ExternalService.hpp"
#include "SevenBit/DI/Details/ServiceInstancesMap.hpp"
#include "SevenBit/DI/ServiceDescriber.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"

class ServiceInstancesMapTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceInstancesMapTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceInstancesMapTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(ServiceInstancesMapTest, ShouldAdd)
{
    sb::di::details::ServiceInstancesMap map{false};

    TestClass1 test;
    sb::di::IServiceInstance::Ptr instance{new sb::di::details::ExternalService{&test}};
    auto act = [&]() { map.insert(typeid(TestClass1), std::move(instance)); };

    EXPECT_NO_THROW((act()));
}

TEST_F(ServiceInstancesMapTest, ShouldFindList)
{
    sb::di::details::ServiceInstancesMap map{false};

    TestInheritClass3 test;
    sb::di::IServiceInstance::Ptr instance{new sb::di::details::ExternalService{&test}};
    map.insert(typeid(TestInheritClass1), std::move(instance));

    TestInheritClass2 test2;
    sb::di::IServiceInstance::Ptr instance2{new sb::di::details::ExternalService{&test2}};
    map.insert(typeid(TestInheritClass1), std::move(instance2));

    auto list = map.findServices(typeid(TestInheritClass1));
    EXPECT_TRUE(list);
    EXPECT_TRUE(list->first()->isValid());
    EXPECT_EQ(list->first()->get(), &test);
    EXPECT_EQ(list->first()->getTypeId(), typeid(TestInheritClass3));
    EXPECT_TRUE(list->last()->isValid());
    EXPECT_EQ(list->last()->get(), &test2);
    EXPECT_EQ(list->last()->getTypeId(), typeid(TestInheritClass2));
}

TEST_F(ServiceInstancesMapTest, ShouldDestructInProperOrder)
{
    ServiceProviderMock mock;
    sb::di::details::ServiceInstancesMap map{true};

    struct DestructionOrderCheck
    {
        int &_cnt;
        int _expected;
        DestructionOrderCheck(int &cnt, int expected) : _cnt(cnt), _expected(expected) {}
        void operator()()
        {
            _cnt++;
            EXPECT_EQ(_cnt, _expected);
        }
    };
    int cnt = 0;

    auto describer = sb::di::ServiceDescriber::describeSingletonFrom<TestInheritDestrClass1>([&]() {
        return std::make_unique<TestInheritDestrClass5<DestructionOrderCheck>>(DestructionOrderCheck{cnt, 4});
    });
    map.insert(describer.getServiceTypeId(), describer.getImplementationFactory().createInstance(mock, false));

    auto describer2 = sb::di::ServiceDescriber::describeSingletonFrom<TestInheritDestrClass2>([&]() {
        return std::make_unique<TestInheritDestrClass5<DestructionOrderCheck>>(DestructionOrderCheck{cnt, 3});
    });
    map.insert(describer2.getServiceTypeId(), describer2.getImplementationFactory().createInstance(mock, false));

    auto describer3 = sb::di::ServiceDescriber::describeSingletonFrom<TestInheritDestrClass3>([&]() {
        return std::make_unique<TestInheritDestrClass5<DestructionOrderCheck>>(DestructionOrderCheck{cnt, 2});
    });
    map.insert(describer3.getServiceTypeId(), describer3.getImplementationFactory().createInstance(mock, false));

    auto describer4 = sb::di::ServiceDescriber::describeSingletonFrom<TestInheritDestrClass4>([&]() {
        return std::make_unique<TestInheritDestrClass5<DestructionOrderCheck>>(DestructionOrderCheck{cnt, 1});
    });
    map.insert(describer4.getServiceTypeId(), describer4.getImplementationFactory().createInstance(mock, false));

    map.clear();
}
