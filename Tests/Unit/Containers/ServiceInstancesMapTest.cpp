#include <gtest/gtest.h>
#include <memory>

#include "../../Helpers/Classes/Basic.hpp"
#include "../../Helpers/Classes/Inherit.hpp"
#include "../../Helpers/Classes/InheritDesctuction.hpp"
#include "../../Helpers/Mocks/ServiceProviderMock.hpp"
#include "SevenBit/DI/Details/Containers/ServiceInstancesMap.hpp"
#include "SevenBit/DI/Details/Services/ExternalService.hpp"
#include "SevenBit/DI/ServiceDescriber.hpp"

class ServiceInstancesMapTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceInstancesMapTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceInstancesMapTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ServiceInstancesMapTest, ShouldInsert)
{
    sb::di::details::containers::ServiceInstancesMap map{false};

    TestClass1 test;
    auto implementation = std::make_unique<sb::di::details::services::ExternalService<TestClass1>>(&test);
    auto act = [&] {
        map.insert(typeid(TestClass1), sb::di::ServiceInstance{sb::di::ServiceInstance{std::move(implementation)}});
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceInstancesMapTest, ShouldCheckEmpty)
{
    sb::di::details::containers::ServiceInstancesMap map{false};

    TestInheritClass3 test;
    auto implementation = std::make_unique<sb::di::details::services::ExternalService<TestInheritClass3>>(&test);
    TestInheritClass2 test2;
    auto implementation2 = std::make_unique<sb::di::details::services::ExternalService<TestInheritClass2>>(&test2);
    map.insert(typeid(TestInheritClass1), sb::di::ServiceInstance{sb::di::ServiceInstance{std::move(implementation)}})
        .add(sb::di::ServiceInstance{sb::di::ServiceInstance{std::move(implementation2)}});

    EXPECT_FALSE(map.empty());
}

TEST_F(ServiceInstancesMapTest, ShouldContainsList)
{
    sb::di::details::containers::ServiceInstancesMap map{false};

    TestInheritClass3 test;
    auto implementation = std::make_unique<sb::di::details::services::ExternalService<TestInheritClass3>>(&test);
    TestInheritClass2 test2;
    auto implementation2 = std::make_unique<sb::di::details::services::ExternalService<TestInheritClass2>>(&test2);
    map.insert(typeid(TestInheritClass1), sb::di::ServiceInstance{std::move(implementation)})
        .add(sb::di::ServiceInstance{std::move(implementation2)});

    EXPECT_TRUE(map.contains(typeid(TestInheritClass1)));
    EXPECT_FALSE(map.contains(typeid(TestInheritClass2)));
}

TEST_F(ServiceInstancesMapTest, ShouldFindInstances)
{
    sb::di::details::containers::ServiceInstancesMap map{false};

    TestInheritClass3 test;
    auto implementation = std::make_unique<sb::di::details::services::ExternalService<TestInheritClass3>>(&test);
    TestInheritClass2 test2;
    auto implementation2 = std::make_unique<sb::di::details::services::ExternalService<TestInheritClass2>>(&test2);
    map.insert(typeid(TestInheritClass1), sb::di::ServiceInstance{std::move(implementation)})
        .add(sb::di::ServiceInstance{std::move(implementation2)});

    const auto list = map.findInstances(typeid(TestInheritClass1));
    EXPECT_TRUE(list);
    EXPECT_TRUE(list->first().isValid());
    EXPECT_EQ(list->first().getAs<void>(), &test);
    EXPECT_EQ(list->first().tryGetImplementation()->getTypeId(), typeid(TestInheritClass3));
    EXPECT_TRUE(list->last().isValid());
    EXPECT_EQ(list->last().getAs<void>(), &test2);
    EXPECT_EQ(list->last().tryGetImplementation()->getTypeId(), typeid(TestInheritClass2));
}

TEST_F(ServiceInstancesMapTest, ShouldDestructInProperOrder)
{
    ServiceProviderMock mock;
    sb::di::details::containers::ServiceInstancesMap map{true};

    struct DestructionOrderCheck
    {
        int &_cnt;
        int _expected;
        DestructionOrderCheck(int &cnt, const int expected) : _cnt(cnt), _expected(expected) {}
        void operator()() const
        {
            _cnt++;
            EXPECT_EQ(_cnt, _expected);
        }
    };
    int cnt = 0;

    const auto describer = sb::di::ServiceDescriber::describeSingletonFrom<TestInheritDestrClass1>([&] {
        return std::make_unique<TestInheritDestrClass5<DestructionOrderCheck>>(DestructionOrderCheck{cnt, 4});
    });
    map.insert(describer.getServiceTypeId(),
               sb::di::ServiceInstance{describer.getImplementationFactory().createInstance(mock, false)});

    const auto describer2 = sb::di::ServiceDescriber::describeSingletonFrom<TestInheritDestrClass2>([&] {
        return std::make_unique<TestInheritDestrClass5<DestructionOrderCheck>>(DestructionOrderCheck{cnt, 3});
    });
    map.insert(describer2.getServiceTypeId(),
               sb::di::ServiceInstance{describer2.getImplementationFactory().createInstance(mock, false)});

    const auto describer3 = sb::di::ServiceDescriber::describeSingletonFrom<TestInheritDestrClass3>([&] {
        return std::make_unique<TestInheritDestrClass5<DestructionOrderCheck>>(DestructionOrderCheck{cnt, 2});
    });
    map.insert(describer3.getServiceTypeId(),
               sb::di::ServiceInstance{describer3.getImplementationFactory().createInstance(mock, false)});

    const auto describer4 = sb::di::ServiceDescriber::describeSingletonFrom<TestInheritDestrClass4>([&] {
        return std::make_unique<TestInheritDestrClass5<DestructionOrderCheck>>(DestructionOrderCheck{cnt, 1});
    });
    map.insert(describer4.getServiceTypeId(),
               sb::di::ServiceInstance{describer4.getImplementationFactory().createInstance(mock, false)});

    map.clear();

    EXPECT_TRUE(map.empty());
}
