#include <algorithm>
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <thread>

#include "Classes/BasicInherit.hpp"
#include "Classes/BasicTest.hpp"
#include "Classes/ComplexDependencies.hpp"
#include "SevenBit/DI/ServiceCollection.hpp"

class SeriviceCollectionTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    SeriviceCollectionTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~SeriviceCollectionTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(SeriviceCollectionTest, ShouldAddServices)
{
    sb::di::ServiceCollection services;

    auto act = [&]() {
        services.addSingleton<TestClass1>();
        services.addSingleton<TestInheritClass1, TestInheritClass5>();

        services.addScoped<TestClass1>();
        services.addScoped<TestInheritClass1, TestInheritClass5>();

        services.addTransient<TestClass1>();
        services.addTransient<TestInheritClass1, TestInheritClass5>();
    };
    EXPECT_NO_THROW((act()));
}

TEST_F(SeriviceCollectionTest, ShouldGetAt)
{
    sb::di::ServiceCollection services;

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();

    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    auto &first = services.at(0);
    EXPECT_EQ(first.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(first.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(first.getImplementationTypeId(), typeid(TestClass1));

    auto &four = services.at(3);
    EXPECT_EQ(four.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(four.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(four.getImplementationTypeId(), typeid(TestInheritClass5));

    EXPECT_ANY_THROW(services.at(10));
}

TEST_F(SeriviceCollectionTest, ShouldGetFirst)
{
    sb::di::ServiceCollection services;

    EXPECT_ANY_THROW((services.first()));

    services.addTransient<TestInheritClass1, TestInheritClass5>();
    services.addSingleton<TestClass1>();
    services.addScoped<TestClass1>();

    auto &first = services.first();
    EXPECT_EQ(first.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(first.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(first.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(SeriviceCollectionTest, ShouldGetLast)
{
    sb::di::ServiceCollection services;

    EXPECT_ANY_THROW((services.last()));

    services.addSingleton<TestClass1>();
    services.addScoped<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    auto &last = services.last();
    EXPECT_EQ(last.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(last.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(last.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(SeriviceCollectionTest, ShouldGetWithIndexOperator)
{
    sb::di::ServiceCollection services;

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();

    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    auto &first = services[0];
    EXPECT_EQ(first.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(first.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(first.getImplementationTypeId(), typeid(TestClass1));

    auto &four = services[3];
    EXPECT_EQ(four.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(four.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(four.getImplementationTypeId(), typeid(TestInheritClass5));

    EXPECT_ANY_THROW(services[10]);
}

TEST_F(SeriviceCollectionTest, ShouldGetSize)
{
    sb::di::ServiceCollection services;

    EXPECT_EQ(services.size(), 0);

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    EXPECT_EQ(services.size(), 2);

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();
    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    EXPECT_EQ(services.size(), 6);
}

TEST_F(SeriviceCollectionTest, ShouldGetCount)
{
    sb::di::ServiceCollection services;

    EXPECT_EQ(services.count(), 0);

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    EXPECT_EQ(services.count(), 2);

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();
    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    EXPECT_EQ(services.count(), 6);
}

TEST_F(SeriviceCollectionTest, ShouldCheckIfEmptyAndClear)
{
    sb::di::ServiceCollection services;

    EXPECT_TRUE(services.empty());

    services.addSingleton<TestClass1>();

    EXPECT_FALSE(services.empty());

    services.clear();

    EXPECT_TRUE(services.empty());
}

TEST_F(SeriviceCollectionTest, ShouldFindIf)
{
    sb::di::ServiceCollection services;

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();

    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    auto it = services.findIf([](sb::di::ServiceDescriptor &descriptor) {
        return descriptor.getServiceTypeId() == typeid(TestInheritClass1) && descriptor.getLifeTime().isSingleton();
    });
    EXPECT_NE(it, services.end());
    EXPECT_EQ(it->getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(it->getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_EQ(it->getLifeTime(), sb::di::ServiceLifeTime::singleton());

    auto notfound = services.findIf([](sb::di::ServiceDescriptor &descriptor) {
        return descriptor.getServiceTypeId() == typeid(TestInheritClass5) && descriptor.getLifeTime().isSingleton();
    });
    EXPECT_EQ(notfound, services.end());
}

TEST_F(SeriviceCollectionTest, ShouldCheckWithContainsIf)
{
    sb::di::ServiceCollection services;

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();

    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    auto contains = services.containsIf([](const sb::di::ServiceDescriptor &descriptor) {
        return descriptor.getServiceTypeId() == typeid(TestInheritClass1) && descriptor.getLifeTime().isSingleton();
    });
    EXPECT_TRUE(contains);
    auto notContains = services.containsIf([](const sb::di::ServiceDescriptor &descriptor) {
        return descriptor.getServiceTypeId() == typeid(TestInheritClass5) && descriptor.getLifeTime().isSingleton();
    });
    EXPECT_FALSE(notContains);
}

TEST_F(SeriviceCollectionTest, ShouldCheckWithContains)
{
    sb::di::ServiceCollection services;

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();

    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    EXPECT_TRUE(services.contains<TestClass1>());
    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_FALSE(services.contains<TestInheritClass5>());
}

TEST_F(SeriviceCollectionTest, ShouldCheckWithContainsExact)
{
    sb::di::ServiceCollection services;

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();

    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    EXPECT_TRUE((services.containsExact<TestClass1, TestClass1>()));
    EXPECT_TRUE((services.containsExact<TestInheritClass1, TestInheritClass5>()));
    EXPECT_FALSE((services.containsExact<TestInheritClass2, TestInheritClass5>()));
}

TEST_F(SeriviceCollectionTest, ShouldInsert)
{
    sb::di::ServiceCollection services;

    services.add(sb::di::ServiceDescriber::describeScoped<TestClass1>());
    services.insert(services.begin(), sb::di::ServiceDescriber::describeScoped<TestClass2>());

    EXPECT_EQ(services.size(), 2);
    EXPECT_EQ((services[0].getImplementationTypeId()), typeid(TestClass2));
    EXPECT_EQ((services[1].getImplementationTypeId()), typeid(TestClass1));
}

TEST_F(SeriviceCollectionTest, ShouldAddDescriptor)
{
    sb::di::ServiceCollection services;

    auto act = [&]() {
        services.add(sb::di::ServiceDescriber::describeSingleton<TestClass1>());
        services.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());

        services.add(sb::di::ServiceDescriber::describeScoped<TestClass1>());
        services.add(sb::di::ServiceDescriber::describeScoped<TestInheritClass1, TestInheritClass5>());

        services.add(sb::di::ServiceDescriber::describeTransient<TestClass1>());
        services.add(sb::di::ServiceDescriber::describeTransient<TestInheritClass1, TestInheritClass5>());
    };

    EXPECT_NO_THROW(act());
}

TEST_F(SeriviceCollectionTest, ShouldRemove)
{
    sb::di::ServiceCollection services;

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();

    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    services.remove(services.begin());
    EXPECT_EQ(services.size(), 5);

    services.remove(services.begin());
    services.remove(services.begin());
    EXPECT_EQ(services.size(), 3);

    services.remove(services.begin());
    services.remove(services.begin());
    services.remove(services.begin());
    EXPECT_EQ(services.size(), 0);
}

TEST_F(SeriviceCollectionTest, ShouldRemoveRange)
{
    sb::di::ServiceCollection services;

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();

    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    services.removeRange(services.begin(), services.begin() + 3);
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldRemoveIf)
{
    sb::di::ServiceCollection services;

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();

    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    EXPECT_EQ((services.removeIf([](sb::di::ServiceDescriptor &descriptor) {
                  return descriptor.getServiceTypeId() == typeid(TestInheritClass1);
              })),
              3);
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldRemoveAll)
{
    sb::di::ServiceCollection services;

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();

    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    EXPECT_EQ(services.removeAll<TestInheritClass1>(), 3);
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldRemoveSpecific)
{
    sb::di::ServiceCollection services;

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();

    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    EXPECT_EQ((services.remove<TestInheritClass1, TestInheritClass5>()), 3);
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldPop)
{
    sb::di::ServiceCollection services;

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();

    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    services.pop();
    EXPECT_EQ(services.size(), 5);

    services.pop();
    services.pop();
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldAddLifeTimeServices)
{
    sb::di::ServiceCollection services;

    services.add<TestInheritClass5>(sb::di::ServiceLifeTime::scoped());
    services.add<TestInheritClass4>(sb::di::ServiceLifeTime::singleton());
    services.add<TestInheritClass3>(sb::di::ServiceLifeTime::transient());

    EXPECT_TRUE(services.contains<TestInheritClass5>());
    EXPECT_TRUE(services.contains<TestInheritClass4>());
    EXPECT_TRUE(services.contains<TestInheritClass3>());
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldAddLifeTimeInheritedServices)
{
    sb::di::ServiceCollection services;

    services.add<TestInheritClass1, TestInheritClass5>(sb::di::ServiceLifeTime::scoped());
    services.add<TestInheritClass1, TestInheritClass4>(sb::di::ServiceLifeTime::singleton());
    services.add<TestInheritClass1, TestInheritClass3>(sb::di::ServiceLifeTime::transient());

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldAddBasicServices)
{
    sb::di::ServiceCollection services;

    services.addSingleton<TestInheritClass5>();
    services.addScoped<TestInheritClass4>();
    services.addTransient<TestInheritClass3>();

    EXPECT_TRUE(services.contains<TestInheritClass5>());
    EXPECT_TRUE(services.contains<TestInheritClass4>());
    EXPECT_TRUE(services.contains<TestInheritClass3>());
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldAddInheritedServices)
{
    sb::di::ServiceCollection services;

    services.addSingleton<TestInheritClass1, TestInheritClass5>();
    services.addScoped<TestInheritClass1, TestInheritClass4>();
    services.addTransient<TestInheritClass1, TestInheritClass3>();

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldAddExternalSingleton)
{
    sb::di::ServiceCollection services;

    TestInheritClass5 test;
    services.addSingleton(&test);

    EXPECT_TRUE(services.contains<TestInheritClass5>());
    EXPECT_EQ(services.size(), 1);
}

TEST_F(SeriviceCollectionTest, ShouldAddExternalInheritedSingleton)
{
    sb::di::ServiceCollection services;

    TestInheritClass5 test;
    services.addSingleton<TestInheritClass1>(&test);

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_EQ(services.size(), 1);
}

TEST_F(SeriviceCollectionTest, ShouldAddLifeTimeServicesWithFactory)
{
    sb::di::ServiceCollection services;

    services.add(sb::di::ServiceLifeTime::scoped(), []() { return std::make_unique<TestInheritClass5>(); });
    services.add(sb::di::ServiceLifeTime::singleton(), []() { return std::make_unique<TestInheritClass4>(); });
    services.add(sb::di::ServiceLifeTime::transient(), []() { return std::make_unique<TestInheritClass3>(); });

    EXPECT_TRUE(services.contains<TestInheritClass5>());
    EXPECT_TRUE(services.contains<TestInheritClass4>());
    EXPECT_TRUE(services.contains<TestInheritClass3>());
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldAddLifeTimeInheritedServicesWithFactory)
{
    sb::di::ServiceCollection services;

    services.add<TestInheritClass1>(sb::di::ServiceLifeTime::scoped(),
                                    []() { return std::make_unique<TestInheritClass5>(); });
    services.add<TestInheritClass1>(sb::di::ServiceLifeTime::singleton(),
                                    []() { return std::make_unique<TestInheritClass4>(); });
    services.add<TestInheritClass1>(sb::di::ServiceLifeTime::transient(),
                                    []() { return std::make_unique<TestInheritClass3>(); });

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldAddBasicServicesWithFactory)
{
    sb::di::ServiceCollection services;

    services.addSingleton([]() { return std::make_unique<TestInheritClass5>(); });
    services.addScoped([]() { return std::make_unique<TestInheritClass4>(); });
    services.addTransient([]() { return std::make_unique<TestInheritClass3>(); });

    EXPECT_TRUE(services.contains<TestInheritClass5>());
    EXPECT_TRUE(services.contains<TestInheritClass4>());
    EXPECT_TRUE(services.contains<TestInheritClass3>());
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldAddServicesWithComplexFactory)
{
    sb::di::ServiceCollection services;

    services.addSingleton<ITestComplexClass1>([]() { return std::make_unique<TestComplexClass1>(); });
    services.addSingleton<ITestComplexClass2>(
        [](ITestComplexClass1 &other1) { return std::make_unique<TestComplexClass2>(&other1); });
    services.addScoped<ITestComplexClass3>([](ITestComplexClass1 &other1, ITestComplexClass2 *other2) {
        return std::make_unique<TestComplexClass3>(&other1, other2);
    });
    services.addScoped<ITestComplexClass4>(
        [](ITestComplexClass1 &other1, ITestComplexClass2 *other2, std::unique_ptr<ITestComplexClass3> other3) {
            return std::make_unique<TestComplexClass4>(&other1, other2, std::move(other3));
        });

    EXPECT_TRUE(services.contains<ITestComplexClass1>());
    EXPECT_TRUE(services.contains<ITestComplexClass2>());
    EXPECT_TRUE(services.contains<ITestComplexClass3>());
    EXPECT_TRUE(services.contains<ITestComplexClass4>());
    EXPECT_EQ(services.size(), 4);
}

TEST_F(SeriviceCollectionTest, ShouldAddInheritedServicesWithFactory)
{
    sb::di::ServiceCollection services;

    services.addSingleton<TestInheritClass1>([]() { return std::make_unique<TestInheritClass5>(); });
    services.addScoped<TestInheritClass1>([]() { return std::make_unique<TestInheritClass4>(); });
    services.addTransient<TestInheritClass1>([]() { return std::make_unique<TestInheritClass3>(); });

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldNotCompileDueToNotInheritedServices)
{
    sb::di::ServiceCollection services;

    // services.addTransient<TestClass1, TestInheritClass1>();
}
