#include <gtest/gtest.h>
#include <iostream>
#include <memory>

#include "Classes/BasicInherit.hpp"
#include "Classes/BasicInheritDesctuction.hpp"
#include "Classes/BasicTest.hpp"
#include "Mocks/ServiceProviderMock.hpp"
#include "SevenBit/DI/Details/ExternalService.hpp"
#include "SevenBit/DI/Details/ServicesMap.hpp"
#include "SevenBit/DI/ServiceDescriber.hpp"
#include "SevenBit/DI/ServiceDescriptor.hpp"

class ServicesMapTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServicesMapTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServicesMapTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(ServicesMapTest, ShouldAdd)
{
    sb::di::details::ServicesMap map{false};

    TestClass1 test;
    sb::di::IServiceInstance::Ptr instance{new sb::di::details::ExternalService{&test}};
    auto act = [&]() { map.add(typeid(TestClass1), std::move(instance)); };

    EXPECT_NO_THROW((act()));
}

TEST_F(ServicesMapTest, ShouldFindList)
{
    sb::di::details::ServicesMap map{false};

    TestInheritClass3 test;
    sb::di::IServiceInstance::Ptr instance{new sb::di::details::ExternalService{&test}};
    map.add(typeid(TestInheritClass1), std::move(instance));

    TestInheritClass2 test2;
    sb::di::IServiceInstance::Ptr instance2{new sb::di::details::ExternalService{&test2}};
    map.add(typeid(TestInheritClass1), std::move(instance2));

    auto list = map.tryGetList(typeid(TestInheritClass1));
    EXPECT_TRUE(list);
    EXPECT_TRUE(list->first()->isValid());
    EXPECT_EQ(list->first()->get(), &test);
    EXPECT_EQ(list->first()->getTypeId(), typeid(TestInheritClass3));
    EXPECT_TRUE(list->last()->isValid());
    EXPECT_EQ(list->last()->get(), &test2);
    EXPECT_EQ(list->last()->getTypeId(), typeid(TestInheritClass2));
}

TEST_F(ServicesMapTest, ShouldDestructInProperOrder)
{
    ServiceProviderMock mock;
    sb::di::details::ServicesMap map{true};

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
    map.add(describer.getServiceTypeId(), describer.getImplementationFactory().createInstance(mock));

    auto describer2 = sb::di::ServiceDescriber::describeSingletonFrom<TestInheritDestrClass2>([&]() {
        return std::make_unique<TestInheritDestrClass5<DestructionOrderCheck>>(DestructionOrderCheck{cnt, 3});
    });
    map.add(describer2.getServiceTypeId(), describer2.getImplementationFactory().createInstance(mock));

    auto describer3 = sb::di::ServiceDescriber::describeSingletonFrom<TestInheritDestrClass3>([&]() {
        return std::make_unique<TestInheritDestrClass5<DestructionOrderCheck>>(DestructionOrderCheck{cnt, 2});
    });
    map.add(describer3.getServiceTypeId(), describer3.getImplementationFactory().createInstance(mock));

    auto describer4 = sb::di::ServiceDescriber::describeSingletonFrom<TestInheritDestrClass4>([&]() {
        return std::make_unique<TestInheritDestrClass5<DestructionOrderCheck>>(DestructionOrderCheck{cnt, 1});
    });
    map.add(describer4.getServiceTypeId(), describer4.getImplementationFactory().createInstance(mock));

    map.clear();
}
