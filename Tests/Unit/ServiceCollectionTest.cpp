#include <algorithm>
#include <gtest/gtest.h>
#include <memory>

#include "../Helpers/Classes/Basic.hpp"
#include "../Helpers/Classes/Complex.hpp"
#include "../Helpers/Classes/Inherit.hpp"
#include <SevenBit/DI/ServiceCollection.hpp>

class ServiceCollectionTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    ServiceCollectionTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceCollectionTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ServiceCollectionTest, ShouldAddServices)
{
    sb::di::ServiceCollection services;

    auto act = [&] {
        services.addSingleton<TestClass1>();
        services.addSingleton<TestInheritClass1, TestInheritClass5>();

        services.addScoped<TestClass1>();
        services.addScoped<TestInheritClass1, TestInheritClass5>();

        services.addTransient<TestClass1>();
        services.addTransient<TestInheritClass1, TestInheritClass5>();

        services.addAlias<TestInheritClass1, TestInheritClass5>();

        services.addKeyedSingleton<TestClass1>("TestClass1");
        services.addKeyedSingleton<TestInheritClass1, TestInheritClass5>("TestInheritClass1");

        services.addKeyedScoped<TestClass1>("TestClass1");
        services.addKeyedScoped<TestInheritClass1, TestInheritClass5>("TestInheritClass1");

        services.addKeyedTransient<TestClass1>("TestClass1");
        services.addKeyedTransient<TestInheritClass1, TestInheritClass5>("TestInheritClass1");

        services.addKeyedAlias<TestInheritClass1, TestInheritClass5>("TestInheritClass1", "TestInheritClass5");
    };
    EXPECT_NO_THROW(act());
}

TEST_F(ServiceCollectionTest, ShouldGetAt)
{
    sb::di::ServiceCollection services;

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();

    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    const auto &first = services.at(0);
    EXPECT_EQ(first.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(first.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(first.getImplementationTypeId(), typeid(TestClass1));

    const auto &four = services.at(3);
    EXPECT_EQ(four.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(four.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(four.getImplementationTypeId(), typeid(TestInheritClass5));

    EXPECT_ANY_THROW(services.at(10));
}

TEST_F(ServiceCollectionTest, ShouldGetFirst)
{
    sb::di::ServiceCollection services;

    EXPECT_ANY_THROW(services.first());

    services.addTransient<TestInheritClass1, TestInheritClass5>();
    services.addSingleton<TestClass1>();
    services.addScoped<TestClass1>();

    const auto &first = services.first();
    EXPECT_EQ(first.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(first.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(first.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceCollectionTest, ShouldGetLast)
{
    sb::di::ServiceCollection services;

    EXPECT_ANY_THROW(services.last());

    services.addSingleton<TestClass1>();
    services.addScoped<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    const auto &last = services.last();
    EXPECT_EQ(last.getLifeTime(), sb::di::ServiceLifeTime::transient());
    EXPECT_EQ(last.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(last.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceCollectionTest, ShouldGetWithIndexOperator)
{
    sb::di::ServiceCollection services;

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();

    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    const auto &first = services[0];
    EXPECT_EQ(first.getLifeTime(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(first.getServiceTypeId(), typeid(TestClass1));
    EXPECT_EQ(first.getImplementationTypeId(), typeid(TestClass1));

    const auto &four = services[3];
    EXPECT_EQ(four.getLifeTime(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(four.getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(four.getImplementationTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceCollectionTest, ShouldGetSize)
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

TEST_F(ServiceCollectionTest, ShouldGetCount)
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

TEST_F(ServiceCollectionTest, ShouldCheckIfEmptyAndClear)
{
    sb::di::ServiceCollection services;

    EXPECT_TRUE(services.empty());

    services.addSingleton<TestClass1>();

    EXPECT_FALSE(services.empty());

    services.clear();

    EXPECT_TRUE(services.empty());
}

TEST_F(ServiceCollectionTest, ShouldFindIf)
{
    sb::di::ServiceCollection services;

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();

    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    const auto it = services.findIf([](const sb::di::ServiceDescriptor &descriptor) {
        return descriptor.getServiceTypeId() == typeid(TestInheritClass1) && descriptor.getLifeTime().isSingleton();
    });
    EXPECT_NE(it, services.end());
    EXPECT_EQ(it->getServiceTypeId(), typeid(TestInheritClass1));
    EXPECT_EQ(it->getImplementationTypeId(), typeid(TestInheritClass5));
    EXPECT_EQ(it->getLifeTime(), sb::di::ServiceLifeTime::singleton());

    const auto notfound = services.findIf([](const sb::di::ServiceDescriptor &descriptor) {
        return descriptor.getServiceTypeId() == typeid(TestInheritClass5) && descriptor.getLifeTime().isSingleton();
    });
    EXPECT_EQ(notfound, services.end());
}

TEST_F(ServiceCollectionTest, ShouldCheckWithContainsIf)
{
    sb::di::ServiceCollection services;

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();

    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    const auto contains = services.containsIf([](const sb::di::ServiceDescriptor &descriptor) {
        return descriptor.getServiceTypeId() == typeid(TestInheritClass1) && descriptor.getLifeTime().isSingleton();
    });
    EXPECT_TRUE(contains);
    const auto notContains = services.containsIf([](const sb::di::ServiceDescriptor &descriptor) {
        return descriptor.getServiceTypeId() == typeid(TestInheritClass5) && descriptor.getLifeTime().isSingleton();
    });
    EXPECT_FALSE(notContains);
}

TEST_F(ServiceCollectionTest, ShouldCheckWithContains)
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

TEST_F(ServiceCollectionTest, ShouldCheckWithContainsKeyed)
{
    sb::di::ServiceCollection services;

    services.addKeyedSingleton<TestClass1>("TestClass1");
    services.addKeyedSingleton<TestInheritClass1, TestInheritClass5>("TestInheritClass1");

    services.addKeyedScoped<TestClass1>("TestClass1");
    services.addKeyedScoped<TestInheritClass1, TestInheritClass5>("TestInheritClass1");

    services.addKeyedTransient<TestClass1>("TestClass1");
    services.addKeyedTransient<TestInheritClass1, TestInheritClass5>("TestInheritClass1");

    services.addKeyedAlias<TestInheritClass1, TestInheritClass5>("TestInheritClass1", "TestInheritClass5");

    EXPECT_TRUE(services.contains<TestClass1>());
    EXPECT_TRUE(services.containsKeyed<TestClass1>("TestClass1"));
    EXPECT_FALSE(services.contains<TestInheritClass5>());
    EXPECT_FALSE(services.containsKeyed<TestClass1>("TestInheritClass5"));
}

TEST_F(ServiceCollectionTest, ShouldCheckWithContainsExact)
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

TEST_F(ServiceCollectionTest, ShouldCheckWithContainsKeyedExact)
{
    sb::di::ServiceCollection services;

    services.addKeyedSingleton<TestClass1>("TestClass1");
    services.addKeyedSingleton<TestInheritClass1, TestInheritClass5>("TestInheritClass1");

    services.addKeyedScoped<TestClass1>("TestClass1");
    services.addKeyedScoped<TestInheritClass1, TestInheritClass5>("TestInheritClass1");

    services.addKeyedTransient<TestClass1>("TestClass1");
    services.addKeyedTransient<TestInheritClass1, TestInheritClass5>("TestInheritClass1");

    services.addKeyedAlias<TestInheritClass1, TestInheritClass5>("TestInheritClass1", "TestInheritClass5");

    EXPECT_TRUE(services.containsExact<TestClass1>());
    EXPECT_TRUE((services.containsKeyedExact<TestInheritClass1, TestInheritClass5>("TestInheritClass1")));
    EXPECT_FALSE((services.containsKeyedExact<TestInheritClass1, TestInheritClass5>("TestInheritClass2")));
    EXPECT_FALSE(services.containsExact<TestInheritClass5>());
    EXPECT_FALSE(services.containsKeyedExact<TestClass1>("TestInheritClass5"));
}

TEST_F(ServiceCollectionTest, ShouldInsert)
{
    sb::di::ServiceCollection services;

    services.add(sb::di::ServiceDescriber::describeScoped<TestClass1>());
    services.insert(services.begin(), sb::di::ServiceDescriber::describeScoped<TestClass2>());

    EXPECT_EQ(services.size(), 2);
    EXPECT_EQ(services[0].getImplementationTypeId(), typeid(TestClass2));
    EXPECT_EQ(services[1].getImplementationTypeId(), typeid(TestClass1));
}

TEST_F(ServiceCollectionTest, ShouldAddDescriptor)
{
    sb::di::ServiceCollection services;

    auto act = [&] {
        services.add(sb::di::ServiceDescriber::describeSingleton<TestClass1>());
        services.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());

        services.add(sb::di::ServiceDescriber::describeScoped<TestClass1>());
        services.add(sb::di::ServiceDescriber::describeScoped<TestInheritClass1, TestInheritClass5>());

        services.add(sb::di::ServiceDescriber::describeTransient<TestClass1>());
        services.add(sb::di::ServiceDescriber::describeTransient<TestInheritClass1, TestInheritClass5>());

        services.add(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass5>());
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceCollectionTest, ShouldRemove)
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

TEST_F(ServiceCollectionTest, ShouldRemoveRange)
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

TEST_F(ServiceCollectionTest, ShouldRemoveIf)
{
    sb::di::ServiceCollection services;

    services.addSingleton<TestClass1>();
    services.addSingleton<TestInheritClass1, TestInheritClass5>();

    services.addScoped<TestClass1>();
    services.addScoped<TestInheritClass1, TestInheritClass5>();

    services.addTransient<TestClass1>();
    services.addTransient<TestInheritClass1, TestInheritClass5>();

    EXPECT_EQ(services.removeIf([](const sb::di::ServiceDescriptor &descriptor) {
        return descriptor.getServiceTypeId() == typeid(TestInheritClass1);
    }),
              3);
    EXPECT_EQ(services.size(), 3);
}

TEST_F(ServiceCollectionTest, ShouldRemoveAll)
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

TEST_F(ServiceCollectionTest, ShouldRemoveAllKeyed)
{
    sb::di::ServiceCollection services;

    services.addKeyedSingleton<TestClass1>("TestClass1");
    services.addKeyedSingleton<TestInheritClass1, TestInheritClass5>("TestInheritClass1");

    services.addKeyedScoped<TestClass1>("TestClass1");
    services.addKeyedScoped<TestInheritClass1, TestInheritClass5>("TestInheritClass1");

    services.addKeyedTransient<TestClass1>("TestClass1");
    services.addKeyedTransient<TestInheritClass1, TestInheritClass5>("TestInheritClass1");

    services.addKeyedAlias<TestInheritClass1, TestInheritClass5>("TestInheritClass1", "TestInheritClass5");

    EXPECT_EQ(services.removeAllKeyed<TestInheritClass1>("TestInheritClass1"), 4);
    EXPECT_EQ(services.size(), 3);
}

TEST_F(ServiceCollectionTest, ShouldRemoveSpecific)
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

TEST_F(ServiceCollectionTest, ShouldRemoveKeyedSpecific)
{
    sb::di::ServiceCollection services;

    services.addKeyedSingleton<TestClass1>("TestClass1");
    services.addKeyedSingleton<TestInheritClass1, TestInheritClass5>("TestInheritClass1");

    services.addKeyedScoped<TestClass1>("TestClass1");
    services.addKeyedScoped<TestInheritClass1, TestInheritClass5>("TestInheritClass1");

    services.addKeyedTransient<TestClass1>("TestClass1");
    services.addKeyedTransient<TestInheritClass1, TestInheritClass5>("TestInheritClass1");

    services.addKeyedAlias<TestInheritClass1, TestInheritClass5>("TestInheritClass1", "TestInheritClass5");

    EXPECT_EQ((services.removeKeyed<TestInheritClass1, TestInheritClass5>("TestInheritClass1")), 4);
    EXPECT_EQ(services.size(), 3);
}

TEST_F(ServiceCollectionTest, ShouldPop)
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

TEST_F(ServiceCollectionTest, ShouldAddLifeTimeServices)
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

TEST_F(ServiceCollectionTest, ShouldAddLifeTimeInheritedServices)
{
    sb::di::ServiceCollection services;

    services.add<TestInheritClass1, TestInheritClass5>(sb::di::ServiceLifeTime::scoped());
    services.add<TestInheritClass1, TestInheritClass4>(sb::di::ServiceLifeTime::singleton());
    services.add<TestInheritClass1, TestInheritClass3>(sb::di::ServiceLifeTime::transient());

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_EQ(services.size(), 3);
}

TEST_F(ServiceCollectionTest, ShouldAddBasicServices)
{
    sb::di::ServiceCollection services;

    services.addSingleton<TestInheritClass5>();
    services.addScoped<TestInheritClass4>();
    services.addTransient<TestInheritClass3>();
    services.addAlias<TestInheritClass1, TestInheritClass3>();

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_TRUE(services.contains<TestInheritClass5>());
    EXPECT_TRUE(services.contains<TestInheritClass4>());
    EXPECT_TRUE(services.contains<TestInheritClass3>());
    EXPECT_EQ(services.size(), 4);
}

TEST_F(ServiceCollectionTest, ShouldAddInheritedServices)
{
    sb::di::ServiceCollection services;

    services.addSingleton<TestInheritClass1, TestInheritClass5>();
    services.addScoped<TestInheritClass1, TestInheritClass4>();
    services.addTransient<TestInheritClass1, TestInheritClass3>();

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_EQ(services.size(), 3);
}

TEST_F(ServiceCollectionTest, ShouldAddInheritedAliasServices)
{
    sb::di::ServiceCollection services;

    services.addAlias<TestInheritClass1, TestInheritClass2>();
    services.addAlias<TestInheritClass1, TestInheritClass3>();
    services.addAlias<TestInheritClass1, TestInheritClass4>();
    services.addAlias<TestInheritClass1, TestInheritClass5>("TestInheritClass5");

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_EQ(services.size(), 4);
}

TEST_F(ServiceCollectionTest, ShouldAddExternalSingleton)
{
    sb::di::ServiceCollection services;

    TestInheritClass5 test;
    services.addSingleton(&test);

    EXPECT_TRUE(services.contains<TestInheritClass5>());
    EXPECT_EQ(services.size(), 1);
}

TEST_F(ServiceCollectionTest, ShouldAddExternalInheritedSingleton)
{
    sb::di::ServiceCollection services;

    TestInheritClass5 test;
    services.addSingleton<TestInheritClass1>(&test);

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_EQ(services.size(), 1);
}

TEST_F(ServiceCollectionTest, ShouldAddLifeTimeServicesWithFactory)
{
    sb::di::ServiceCollection services;

    services.add(sb::di::ServiceLifeTime::scoped(), [] { return std::make_unique<TestInheritClass5>(); });
    services.add(sb::di::ServiceLifeTime::singleton(), [] { return std::make_unique<TestInheritClass4>(); });
    services.add(sb::di::ServiceLifeTime::transient(), [] { return std::make_unique<TestInheritClass3>(); });

    EXPECT_TRUE(services.contains<TestInheritClass5>());
    EXPECT_TRUE(services.contains<TestInheritClass4>());
    EXPECT_TRUE(services.contains<TestInheritClass3>());
    EXPECT_EQ(services.size(), 3);
}

TEST_F(ServiceCollectionTest, ShouldAddLifeTimeInheritedServicesWithFactory)
{
    sb::di::ServiceCollection services;

    services.add<TestInheritClass1>(sb::di::ServiceLifeTime::scoped(),
                                    [] { return std::make_unique<TestInheritClass5>(); });
    services.add<TestInheritClass1>(sb::di::ServiceLifeTime::singleton(),
                                    [] { return std::make_unique<TestInheritClass4>(); });
    services.add<TestInheritClass1>(sb::di::ServiceLifeTime::transient(),
                                    [] { return std::make_unique<TestInheritClass3>(); });

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_EQ(services.size(), 3);
}

TEST_F(ServiceCollectionTest, ShouldAddBasicServicesWithFactory)
{
    sb::di::ServiceCollection services;

    services.addSingleton([] { return std::make_unique<TestInheritClass5>(); });
    services.addScoped([] { return std::make_unique<TestInheritClass4>(); });
    services.addTransient([] { return std::make_unique<TestInheritClass3>(); });

    EXPECT_TRUE(services.contains<TestInheritClass5>());
    EXPECT_TRUE(services.contains<TestInheritClass4>());
    EXPECT_TRUE(services.contains<TestInheritClass3>());
    EXPECT_EQ(services.size(), 3);
}

TEST_F(ServiceCollectionTest, ShouldAddServicesWithComplexFactory)
{
    sb::di::ServiceCollection services;

    services.addSingleton<ITestComplexClass1>([] { return std::make_unique<TestComplexClass1>(); });
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

TEST_F(ServiceCollectionTest, ShouldAddInheritedServicesWithFactory)
{
    sb::di::ServiceCollection services;

    services.addSingleton<TestInheritClass1>([] { return std::make_unique<TestInheritClass5>(); });
    services.addScoped<TestInheritClass1>([] { return std::make_unique<TestInheritClass4>(); });
    services.addTransient<TestInheritClass1>([] { return std::make_unique<TestInheritClass3>(); });

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_EQ(services.size(), 3);
}

TEST_F(ServiceCollectionTest, ShouldAddKeyedLifeTimeServices)
{
    sb::di::ServiceCollection services;

    services.addKeyed<TestInheritClass5>(sb::di::ServiceLifeTime::scoped(), "TestInheritClass5");
    services.addKeyed<TestInheritClass4>(sb::di::ServiceLifeTime::singleton(), "TestInheritClass4");
    services.addKeyed<TestInheritClass3>(sb::di::ServiceLifeTime::transient(), "TestInheritClass3");

    EXPECT_TRUE(services.contains<TestInheritClass5>());
    EXPECT_TRUE(services.contains<TestInheritClass4>());
    EXPECT_TRUE(services.contains<TestInheritClass3>());
    EXPECT_TRUE(services.containsKeyed<TestInheritClass5>("TestInheritClass5"));
    EXPECT_TRUE(services.containsKeyed<TestInheritClass4>("TestInheritClass4"));
    EXPECT_TRUE(services.containsKeyed<TestInheritClass3>("TestInheritClass3"));
    EXPECT_EQ(services.size(), 3);
}

TEST_F(ServiceCollectionTest, ShouldAddKeyedLifeTimeInheritedServices)
{
    sb::di::ServiceCollection services;

    services.addKeyed<TestInheritClass1, TestInheritClass5>(sb::di::ServiceLifeTime::scoped(), "TestInheritClass1");
    services.addKeyed<TestInheritClass1, TestInheritClass4>(sb::di::ServiceLifeTime::singleton(), "TestInheritClass1");
    services.addKeyed<TestInheritClass1, TestInheritClass3>(sb::di::ServiceLifeTime::transient(), "TestInheritClass1");

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_TRUE(services.containsKeyed<TestInheritClass1>("TestInheritClass1"));
    EXPECT_EQ(services.size(), 3);
}

TEST_F(ServiceCollectionTest, ShouldAddKeyedBasicServices)
{
    sb::di::ServiceCollection services;

    services.addKeyedSingleton<TestInheritClass5>("TestInheritClass5");
    services.addKeyedScoped<TestInheritClass4>("TestInheritClass4");
    services.addKeyedTransient<TestInheritClass3>("TestInheritClass3");
    services.addKeyedAlias<TestInheritClass1, TestInheritClass3>("TestInheritClass1", "TestInheritClass3");

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_TRUE(services.contains<TestInheritClass5>());
    EXPECT_TRUE(services.contains<TestInheritClass4>());
    EXPECT_TRUE(services.contains<TestInheritClass3>());
    EXPECT_EQ(services.size(), 4);
}

TEST_F(ServiceCollectionTest, ShouldAddKeyedInheritedServices)
{
    sb::di::ServiceCollection services;

    services.addKeyedSingleton<TestInheritClass1, TestInheritClass5>("TestInheritClass1");
    services.addKeyedScoped<TestInheritClass1, TestInheritClass4>("TestInheritClass1");
    services.addKeyedTransient<TestInheritClass1, TestInheritClass3>("TestInheritClass1");

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_TRUE(services.containsKeyed<TestInheritClass1>("TestInheritClass1"));
    EXPECT_EQ(services.size(), 3);
}

TEST_F(ServiceCollectionTest, ShouldAddKeyedInheritedAliasServices)
{
    sb::di::ServiceCollection services;

    services.addKeyedAlias<TestInheritClass1, TestInheritClass5>("TestInheritClass1");
    services.addKeyedAlias<TestInheritClass1, TestInheritClass4>("TestInheritClass1");
    services.addKeyedAlias<TestInheritClass1, TestInheritClass3>("TestInheritClass1");

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_TRUE(services.containsKeyed<TestInheritClass1>("TestInheritClass1"));
    EXPECT_EQ(services.size(), 3);
}

TEST_F(ServiceCollectionTest, ShouldAddKeyedExternalSingleton)
{
    sb::di::ServiceCollection services;

    TestInheritClass5 test;
    services.addKeyedSingleton("TestInheritClass5", &test);

    EXPECT_TRUE(services.contains<TestInheritClass5>());
    EXPECT_TRUE(services.containsKeyed<TestInheritClass5>("TestInheritClass5"));
    EXPECT_EQ(services.size(), 1);
}

TEST_F(ServiceCollectionTest, ShouldAddKeyedExternalInheritedSingleton)
{
    sb::di::ServiceCollection services;

    TestInheritClass5 test;
    services.addKeyedSingleton<TestInheritClass1>("TestInheritClass5", &test);

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_TRUE(services.containsKeyed<TestInheritClass1>("TestInheritClass5"));
    EXPECT_EQ(services.size(), 1);
}

TEST_F(ServiceCollectionTest, ShouldAddKeyedLifeTimeServicesWithFactory)
{
    sb::di::ServiceCollection services;

    services.addKeyed(sb::di::ServiceLifeTime::scoped(), "TestInheritClass5",
                      [] { return std::make_unique<TestInheritClass5>(); });
    services.addKeyed(sb::di::ServiceLifeTime::singleton(), "TestInheritClass4",
                      [] { return std::make_unique<TestInheritClass4>(); });
    services.addKeyed(sb::di::ServiceLifeTime::transient(), "TestInheritClass3",
                      [] { return std::make_unique<TestInheritClass3>(); });

    EXPECT_TRUE(services.contains<TestInheritClass5>());
    EXPECT_TRUE(services.contains<TestInheritClass4>());
    EXPECT_TRUE(services.contains<TestInheritClass3>());
    EXPECT_TRUE(services.containsKeyed<TestInheritClass5>("TestInheritClass5"));
    EXPECT_TRUE(services.containsKeyed<TestInheritClass4>("TestInheritClass4"));
    EXPECT_TRUE(services.containsKeyed<TestInheritClass3>("TestInheritClass3"));
    EXPECT_EQ(services.size(), 3);
}

TEST_F(ServiceCollectionTest, ShouldAddKeyedLifeTimeInheritedServicesWithFactory)
{
    sb::di::ServiceCollection services;

    services.addKeyed<TestInheritClass1>(sb::di::ServiceLifeTime::scoped(), "TestInheritClass1",
                                         [] { return std::make_unique<TestInheritClass5>(); });
    services.addKeyed<TestInheritClass1>(sb::di::ServiceLifeTime::singleton(), "TestInheritClass1",
                                         [] { return std::make_unique<TestInheritClass4>(); });
    services.addKeyed<TestInheritClass1>(sb::di::ServiceLifeTime::transient(), "TestInheritClass1",
                                         [] { return std::make_unique<TestInheritClass3>(); });

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_TRUE(services.containsKeyed<TestInheritClass1>("TestInheritClass1"));
    EXPECT_EQ(services.size(), 3);
}

TEST_F(ServiceCollectionTest, ShouldAddKeyedBasicServicesWithFactory)
{
    sb::di::ServiceCollection services;

    services.addKeyedSingleton("TestInheritClass5", [] { return std::make_unique<TestInheritClass5>(); });
    services.addKeyedScoped("TestInheritClass4", [] { return std::make_unique<TestInheritClass4>(); });
    services.addKeyedTransient("TestInheritClass3", [] { return std::make_unique<TestInheritClass3>(); });

    EXPECT_TRUE(services.contains<TestInheritClass5>());
    EXPECT_TRUE(services.contains<TestInheritClass4>());
    EXPECT_TRUE(services.contains<TestInheritClass3>());
    EXPECT_TRUE(services.containsKeyed<TestInheritClass5>("TestInheritClass5"));
    EXPECT_TRUE(services.containsKeyed<TestInheritClass4>("TestInheritClass4"));
    EXPECT_TRUE(services.containsKeyed<TestInheritClass3>("TestInheritClass3"));
    EXPECT_EQ(services.size(), 3);
}

TEST_F(ServiceCollectionTest, ShouldAddKeyedServicesWithComplexFactory)
{
    sb::di::ServiceCollection services;

    services.addKeyedSingleton<ITestComplexClass1>("ITestComplexClass1",
                                                   [] { return std::make_unique<TestComplexClass1>(); });
    services.addKeyedSingleton<ITestComplexClass2>(
        "ITestComplexClass2", [](ITestComplexClass1 &other1) { return std::make_unique<TestComplexClass2>(&other1); });
    services.addKeyedScoped<ITestComplexClass3>("ITestComplexClass3",
                                                [](ITestComplexClass1 &other1, ITestComplexClass2 *other2) {
                                                    return std::make_unique<TestComplexClass3>(&other1, other2);
                                                });
    services.addKeyedScoped<ITestComplexClass4>(
        "ITestComplexClass4",
        [](ITestComplexClass1 &other1, ITestComplexClass2 *other2, std::unique_ptr<ITestComplexClass3> other3) {
            return std::make_unique<TestComplexClass4>(&other1, other2, std::move(other3));
        });

    EXPECT_TRUE(services.contains<ITestComplexClass1>());
    EXPECT_TRUE(services.contains<ITestComplexClass2>());
    EXPECT_TRUE(services.contains<ITestComplexClass3>());
    EXPECT_TRUE(services.contains<ITestComplexClass4>());
    EXPECT_TRUE(services.containsKeyed<ITestComplexClass1>("ITestComplexClass1"));
    EXPECT_TRUE(services.containsKeyed<ITestComplexClass2>("ITestComplexClass2"));
    EXPECT_TRUE(services.containsKeyed<ITestComplexClass3>("ITestComplexClass3"));
    EXPECT_TRUE(services.containsKeyed<ITestComplexClass4>("ITestComplexClass4"));
    EXPECT_EQ(services.size(), 4);
}

TEST_F(ServiceCollectionTest, ShouldAddKeyedInheritedServicesWithFactory)
{
    sb::di::ServiceCollection services;

    services.addKeyedSingleton<TestInheritClass1>("TestInheritClass1",
                                                  [] { return std::make_unique<TestInheritClass5>(); });
    services.addKeyedScoped<TestInheritClass1>("TestInheritClass1",
                                               [] { return std::make_unique<TestInheritClass4>(); });
    services.addKeyedTransient<TestInheritClass1>("TestInheritClass1",
                                                  [] { return std::make_unique<TestInheritClass3>(); });

    EXPECT_TRUE(services.contains<TestInheritClass1>());
    EXPECT_TRUE(services.containsKeyed<TestInheritClass1>("TestInheritClass1"));
    EXPECT_EQ(services.size(), 3);
}
