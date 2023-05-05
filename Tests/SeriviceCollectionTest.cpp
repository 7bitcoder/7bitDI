#include <algorithm>
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <thread>

#include "Classes/BasicInherit.hpp"
#include "Classes/BasicTest.hpp"
#include "SevenBit/IServiceProvider.hpp"
#include "SevenBit/ServiceCollection.hpp"
#include "SevenBit/ServiceDescriber.hpp"
#include "SevenBit/ServiceDescriptor.hpp"
#include "SevenBit/ServiceLifeTime.hpp"
#include "SevenBit/_Internal/ServiceProvider.hpp"

class SeriviceCollectionTest : public ::testing::Test
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
    sb::ServiceCollection services;

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
    sb::ServiceCollection services;

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();

    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    auto &first = services.at(0);
    EXPECT_EQ(first.getLifeTime(), sb::ServiceLifeTime::singleton());
    EXPECT_EQ(first.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(first.getImplementationTypeId(), typeid(TestClass1));

    auto &four = services.at(3);
    EXPECT_EQ(four.getLifeTime(), sb::ServiceLifeTime::scoped());
    EXPECT_EQ(four.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(four.getImplementationTypeId(), typeid(TestInheritClass5));

    EXPECT_ANY_THROW(services.at(10));
}

TEST_F(SeriviceCollectionTest, ShouldGetFirst)
{
    sb::ServiceCollection services;

    EXPECT_ANY_THROW((services.first()));

    services.addTransient<TestInheritClass1, TestInheritClass5>();
    services.addSingleton<TestClass1>();
    services.addScoped<TestClass1>();

    auto &first = services.first();
    EXPECT_EQ(first.getLifeTime(), sb::ServiceLifeTime::transient());
    EXPECT_EQ(first.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(first.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(SeriviceCollectionTest, ShouldGetLast)
{
    sb::ServiceCollection services;

    EXPECT_ANY_THROW((services.last()));

    services.addSingleton<TestClass1>();
    services.addScoped<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    auto &last = services.last();
    EXPECT_EQ(last.getLifeTime(), sb::ServiceLifeTime::transient());
    EXPECT_EQ(last.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(last.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(SeriviceCollectionTest, ShouldGetWithIndexOperator)
{
    sb::ServiceCollection services;

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();

    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    auto &first = services[0];
    EXPECT_EQ(first.getLifeTime(), sb::ServiceLifeTime::singleton());
    EXPECT_EQ(first.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(first.getImplementationTypeId(), typeid(TestClass1));

    auto &four = services[3];
    EXPECT_EQ(four.getLifeTime(), sb::ServiceLifeTime::scoped());
    EXPECT_EQ(four.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(four.getImplementationTypeId(), typeid(TestInheritClass5));

    EXPECT_ANY_THROW(services[10]);
}

TEST_F(SeriviceCollectionTest, ShouldGetSize)
{
    sb::ServiceCollection services;

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
    sb::ServiceCollection services;

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
    sb::ServiceCollection services;

    EXPECT_TRUE(services.empty());

    services.addSingleton<TestClass1>();

    EXPECT_FALSE(services.empty());

    services.clear();

    EXPECT_TRUE(services.empty());
}

TEST_F(SeriviceCollectionTest, ShouldFindIf)
{
    sb::ServiceCollection services;

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();

    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    auto it = services.findIf([](sb::ServiceDescriptor &descriptor) {
        return descriptor.getServiceTypeId() == typeid(TestInheritClass1) && descriptor.getLifeTime().isSingleton();
    });
    EXPECT_NE(it, services.end());
    EXPECT_EQ(it->getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(it->getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_EQ(it->getLifeTime(), sb::ServiceLifeTime::singleton());

    auto notfound = services.findIf([](sb::ServiceDescriptor &descriptor) {
        return descriptor.getServiceTypeId() == typeid(TestInheritClass5) && descriptor.getLifeTime().isSingleton();
    });
    EXPECT_EQ(notfound, services.end());
}

TEST_F(SeriviceCollectionTest, ShouldCheckWithContainsIf)
{
    sb::ServiceCollection services;

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();

    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    auto contains = services.containsIf([](const sb::ServiceDescriptor &descriptor) {
        return descriptor.getServiceTypeId() == typeid(TestInheritClass1) && descriptor.getLifeTime().isSingleton();
    });
    EXPECT_TRUE(contains);
    auto notContains = services.containsIf([](const sb::ServiceDescriptor &descriptor) {
        return descriptor.getServiceTypeId() == typeid(TestInheritClass5) && descriptor.getLifeTime().isSingleton();
    });
    EXPECT_FALSE(notContains);
}

TEST_F(SeriviceCollectionTest, ShouldCheckWithContains)
{
    sb::ServiceCollection services;

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
    sb::ServiceCollection services;

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
    sb::ServiceCollection services;

    services.add(sb::ServiceDescriber::describeScoped<TestClass1>());
    services.insert(services.begin(), sb::ServiceDescriber::describeScoped<TestClass2>());

    EXPECT_EQ(services.size(), 2);
    EXPECT_EQ((services[0].getImplementationTypeId()), typeid(TestClass2));
    EXPECT_EQ((services[1].getImplementationTypeId()), typeid(TestClass1));
}

TEST_F(SeriviceCollectionTest, ShouldAddDescriptor)
{
    sb::ServiceCollection services;

    auto act = [&]() {
        services.add(sb::ServiceDescriber::describeSingleton<TestClass1>());
        services.add(sb::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());

        services.add(sb::ServiceDescriber::describeScoped<TestClass1>());
        services.add(sb::ServiceDescriber::describeScoped<TestInheritClass1, TestInheritClass5>());

        services.add(sb::ServiceDescriber::describeTransient<TestClass1>());
        services.add(sb::ServiceDescriber::describeTransient<TestInheritClass1, TestInheritClass5>());
    };

    EXPECT_NO_THROW(act());
}

TEST_F(SeriviceCollectionTest, ShouldRemove)
{
    sb::ServiceCollection services;

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
    sb::ServiceCollection services;

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
    sb::ServiceCollection services;

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();

    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    services.removeIf(
        [](sb::ServiceDescriptor &descriptor) { return descriptor.getServiceTypeId() == typeid(TestInheritClass1); });
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldRemoveAll)
{
    sb::ServiceCollection services;

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();

    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    services.removeAll<TestInheritClass1>();
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldRemoveSpecific)
{
    sb::ServiceCollection services;

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();

    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    services.remove<TestInheritClass1, TestInheritClass5>();
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldPop)
{
    sb::ServiceCollection services;

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
    sb::ServiceCollection services;

    services.add<TestInheritClass5>(sb::ServiceLifeTime::scoped());
    services.add<TestInheritClass4>(sb::ServiceLifeTime::singleton());
    services.add<TestInheritClass3>(sb::ServiceLifeTime::transient());

    EXPECT_TRUE(services.contains<TestInheritClass5>());
    EXPECT_TRUE(services.contains<TestInheritClass4>());
    EXPECT_TRUE(services.contains<TestInheritClass3>());
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldAddLifeTimeInheritedServices)
{
    sb::ServiceCollection services;

    services.add<TestInheritClass1, TestInheritClass5>(sb::ServiceLifeTime::scoped());
    services.add<TestInheritClass1, TestInheritClass4>(sb::ServiceLifeTime::singleton());
    services.add<TestInheritClass1, TestInheritClass3>(sb::ServiceLifeTime::transient());

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldAddBasicServices)
{
    sb::ServiceCollection services;

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
    sb::ServiceCollection services;

    services.addSingleton<TestInheritClass1, TestInheritClass5>();
    services.addScoped<TestInheritClass1, TestInheritClass4>();
    services.addTransient<TestInheritClass1, TestInheritClass3>();

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldAddExternalSingleton)
{
    sb::ServiceCollection services;

    TestInheritClass5 test;
    services.addSingleton(&test);

    EXPECT_TRUE(services.contains<TestInheritClass5>());
    EXPECT_EQ(services.size(), 1);
}

TEST_F(SeriviceCollectionTest, ShouldAddExternalInheritedSingleton)
{
    sb::ServiceCollection services;

    TestInheritClass5 test;
    services.addSingleton<TestInheritClass1>(&test);

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_EQ(services.size(), 1);
}

TEST_F(SeriviceCollectionTest, ShouldAddLifeTimeServicesWithFactory)
{
    sb::ServiceCollection services;

    services.add(sb::ServiceLifeTime::scoped(),
                 [](sb::IServiceProvider &) { return std::make_unique<TestInheritClass5>(); });
    services.add(sb::ServiceLifeTime::singleton(),
                 [](sb::IServiceProvider &) { return std::make_unique<TestInheritClass4>(); });
    services.add(sb::ServiceLifeTime::transient(),
                 [](sb::IServiceProvider &) { return std::make_unique<TestInheritClass3>(); });

    EXPECT_TRUE(services.contains<TestInheritClass5>());
    EXPECT_TRUE(services.contains<TestInheritClass4>());
    EXPECT_TRUE(services.contains<TestInheritClass3>());
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldAddLifeTimeInheritedServicesWithFactory)
{
    sb::ServiceCollection services;

    services.add<TestInheritClass1>(sb::ServiceLifeTime::scoped(),
                                    [](sb::IServiceProvider &) { return std::make_unique<TestInheritClass5>(); });
    services.add<TestInheritClass1>(sb::ServiceLifeTime::singleton(),
                                    [](sb::IServiceProvider &) { return std::make_unique<TestInheritClass4>(); });
    services.add<TestInheritClass1>(sb::ServiceLifeTime::transient(),
                                    [](sb::IServiceProvider &) { return std::make_unique<TestInheritClass3>(); });

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldAddBasicServicesWithFactory)
{
    sb::ServiceCollection services;

    services.addSingleton([](sb::IServiceProvider &) { return std::make_unique<TestInheritClass5>(); });
    services.addScoped([](sb::IServiceProvider &) { return std::make_unique<TestInheritClass4>(); });
    services.addTransient([](sb::IServiceProvider &) { return std::make_unique<TestInheritClass3>(); });

    EXPECT_TRUE(services.contains<TestInheritClass5>());
    EXPECT_TRUE(services.contains<TestInheritClass4>());
    EXPECT_TRUE(services.contains<TestInheritClass3>());
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldAddInheritedServicesWithFactory)
{
    sb::ServiceCollection services;

    services.addSingleton<TestInheritClass1>(
        [](sb::IServiceProvider &) { return std::make_unique<TestInheritClass5>(); });
    services.addScoped<TestInheritClass1>([](sb::IServiceProvider &) { return std::make_unique<TestInheritClass4>(); });
    services.addTransient<TestInheritClass1>(
        [](sb::IServiceProvider &) { return std::make_unique<TestInheritClass3>(); });

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldAddLifeTimeServicesWithEmptyFactory)
{
    sb::ServiceCollection services;

    services.add(sb::ServiceLifeTime::scoped(), []() { return std::make_unique<TestInheritClass5>(); });
    services.add(sb::ServiceLifeTime::singleton(), []() { return std::make_unique<TestInheritClass4>(); });
    services.add(sb::ServiceLifeTime::transient(), []() { return std::make_unique<TestInheritClass3>(); });

    EXPECT_TRUE(services.contains<TestInheritClass5>());
    EXPECT_TRUE(services.contains<TestInheritClass4>());
    EXPECT_TRUE(services.contains<TestInheritClass3>());
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldAddLifeTimeInheritedServicesWithEmptyFactory)
{
    sb::ServiceCollection services;

    services.add<TestInheritClass1>(sb::ServiceLifeTime::scoped(),
                                    []() { return std::make_unique<TestInheritClass5>(); });
    services.add<TestInheritClass1>(sb::ServiceLifeTime::singleton(),
                                    []() { return std::make_unique<TestInheritClass4>(); });
    services.add<TestInheritClass1>(sb::ServiceLifeTime::transient(),
                                    []() { return std::make_unique<TestInheritClass3>(); });

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldAddBasicServicesWithEmptyFactory)
{
    sb::ServiceCollection services;

    services.addSingleton([]() { return std::make_unique<TestInheritClass5>(); });
    services.addScoped([]() { return std::make_unique<TestInheritClass4>(); });
    services.addTransient([]() { return std::make_unique<TestInheritClass3>(); });

    EXPECT_TRUE(services.contains<TestInheritClass5>());
    EXPECT_TRUE(services.contains<TestInheritClass4>());
    EXPECT_TRUE(services.contains<TestInheritClass3>());
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldAddInheritedServicesWithEmptyFactory)
{
    sb::ServiceCollection services;

    services.addSingleton<TestInheritClass1>([]() { return std::make_unique<TestInheritClass5>(); });
    services.addScoped<TestInheritClass1>([]() { return std::make_unique<TestInheritClass4>(); });
    services.addTransient<TestInheritClass1>([]() { return std::make_unique<TestInheritClass3>(); });

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_EQ(services.size(), 3);
}

TEST_F(SeriviceCollectionTest, ShouldNotCompileDueToNotInheritedServices)
{
    sb::ServiceCollection services;

    // services.addTransient<TestClass1, TestInheritClass1>();
}