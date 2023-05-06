#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <thread>

#include "Classes/BasicInherit.hpp"
#include "Classes/BasicTest.hpp"
#include "Classes/CirularDependency.hpp"
#include "Classes/ComplexDependencies.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/IServiceProvider.hpp"
#include "SevenBit/DI/ServiceCollection.hpp"

class SeriviceProviderScopesTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    SeriviceProviderScopesTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~SeriviceProviderScopesTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(SeriviceProviderScopesTest, ShouldMakeScopedProvider)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addSingleton<TestClass2>()
                        .addSingleton<TestClass3>()
                        .buildServiceProvider();

    EXPECT_NO_THROW(provider->createScope());
}

TEST_F(SeriviceProviderScopesTest, ShouldGetProperServicesRefWithScopedProvider)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    auto scoped1 = provider->createScope();
    auto scoped2 = provider->createScope();
    auto scoped3 = provider->createScope();

    EXPECT_EQ(&scoped3->getService<TestClass1>(), &scoped3->getService<TestClass1>());
    EXPECT_EQ(&scoped3->getService<TestClass1>(), &scoped2->getService<TestClass1>());
    EXPECT_EQ(&scoped3->getService<TestClass1>(), &scoped1->getService<TestClass1>());
    EXPECT_EQ(&scoped3->getService<TestClass1>(), &provider->getService<TestClass1>());
    EXPECT_EQ(&scoped2->getService<TestClass1>(), &scoped2->getService<TestClass1>());
    EXPECT_EQ(&scoped2->getService<TestClass1>(), &scoped1->getService<TestClass1>());
    EXPECT_EQ(&scoped2->getService<TestClass1>(), &provider->getService<TestClass1>());
    EXPECT_EQ(&scoped1->getService<TestClass1>(), &scoped1->getService<TestClass1>());
    EXPECT_EQ(&scoped1->getService<TestClass1>(), &provider->getService<TestClass1>());
    EXPECT_EQ(&provider->getService<TestClass1>(), &provider->getService<TestClass1>());

    EXPECT_EQ(&scoped3->getService<TestClass2>(), &scoped3->getService<TestClass2>());
    EXPECT_NE(&scoped3->getService<TestClass2>(), &scoped2->getService<TestClass2>());
    EXPECT_NE(&scoped3->getService<TestClass2>(), &scoped1->getService<TestClass2>());
    EXPECT_NE(&scoped3->getService<TestClass2>(), &provider->getService<TestClass2>());
    EXPECT_EQ(&scoped2->getService<TestClass2>(), &scoped2->getService<TestClass2>());
    EXPECT_NE(&scoped2->getService<TestClass2>(), &scoped1->getService<TestClass2>());
    EXPECT_NE(&scoped2->getService<TestClass2>(), &provider->getService<TestClass2>());
    EXPECT_EQ(&scoped1->getService<TestClass2>(), &scoped1->getService<TestClass2>());
    EXPECT_NE(&scoped1->getService<TestClass2>(), &provider->getService<TestClass2>());
    EXPECT_EQ(&provider->getService<TestClass2>(), &provider->getService<TestClass2>());

    EXPECT_NE(scoped3->createService<TestClass3>(), scoped3->createService<TestClass3>());
    EXPECT_NE(scoped3->createService<TestClass3>(), scoped2->createService<TestClass3>());
    EXPECT_NE(scoped3->createService<TestClass3>(), scoped1->createService<TestClass3>());
    EXPECT_NE(scoped3->createService<TestClass3>(), provider->createService<TestClass3>());
    EXPECT_NE(scoped2->createService<TestClass3>(), scoped2->createService<TestClass3>());
    EXPECT_NE(scoped2->createService<TestClass3>(), scoped1->createService<TestClass3>());
    EXPECT_NE(scoped2->createService<TestClass3>(), provider->createService<TestClass3>());
    EXPECT_NE(scoped1->createService<TestClass3>(), scoped1->createService<TestClass3>());
    EXPECT_NE(scoped1->createService<TestClass3>(), provider->createService<TestClass3>());
    EXPECT_NE(provider->createService<TestClass3>(), provider->createService<TestClass3>());
}

TEST_F(SeriviceProviderScopesTest, ShouldReturnProperSelfForScope)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    auto scoped1 = provider->createScope();
    auto scoped2 = provider->createScope();
    auto scoped3 = provider->createScope();

    EXPECT_EQ(provider->tryGetService<sb::di::IServiceProvider>(), provider.get());
    EXPECT_EQ(scoped1->tryGetService<sb::di::IServiceProvider>(), scoped1.get());
    EXPECT_EQ(scoped3->tryGetService<sb::di::IServiceProvider>(), scoped3.get());
}

TEST_F(SeriviceProviderScopesTest, ShouldGetServicesDeeperRefWithScopedProvider)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestDependencyClass1>()
                        .addScoped<TestDependencyClass2>()
                        .addTransient<TestDependencyClass3>()
                        .buildServiceProvider();

    auto scoped = provider->createScope();

    auto &singletonFromTop = provider->getService<TestDependencyClass1>();
    auto &singletonFromScoped = scoped->getService<TestDependencyClass1>();
    EXPECT_EQ(&singletonFromTop, &singletonFromScoped);

    auto &scopedFromTop = provider->getService<TestDependencyClass2>();
    auto &scopedFromScoped = scoped->getService<TestDependencyClass2>();
    EXPECT_NE(&scopedFromTop, &scopedFromScoped);
    EXPECT_EQ(scopedFromTop._test1, &singletonFromTop);
    EXPECT_EQ(scopedFromTop._test1, scopedFromScoped._test1);

    auto transientFromTop = provider->createService<TestDependencyClass3>();
    auto transientFromScoped = scoped->createService<TestDependencyClass3>();
    EXPECT_NE(transientFromTop, transientFromScoped);
    EXPECT_EQ(transientFromTop->_test1, &singletonFromTop);
    EXPECT_EQ(transientFromTop->_test1, transientFromScoped->_test1);
    EXPECT_EQ(transientFromTop->_test2, &scopedFromTop);
    EXPECT_EQ(transientFromScoped->_test2, &scopedFromScoped);
}

TEST_F(SeriviceProviderScopesTest, ShouldGetComplexServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<ITestComplexClass1, TestComplexClass1>()
                        .addScoped<ITestComplexClass2, TestComplexClass2>()
                        .addTransient<ITestComplexClass3, TestComplexClass3>()
                        .addScoped<ITestComplexClass4, TestComplexClass4>()
                        .addScoped<ITestComplexClass5, TestComplexClass5>()
                        .buildServiceProvider();

    auto &singleton = provider->getService<ITestComplexClass1>();
    auto &scoped1 = provider->getService<ITestComplexClass2>();
    auto transient = provider->createService<ITestComplexClass3>();
    auto &scoped2 = provider->getService<ITestComplexClass4>();
    auto &scoped3 = provider->getService<ITestComplexClass5>();

    EXPECT_EQ(singleton.number(), 1);
    EXPECT_EQ(scoped1.number(), 2);
    EXPECT_EQ(transient->number(), 3);
    EXPECT_EQ(scoped2.number(), 4);
    EXPECT_EQ(scoped3.number(), 5);

    EXPECT_EQ(scoped1.getOne(), &singleton);
    EXPECT_EQ(transient->getOne(), &singleton);
    EXPECT_EQ(transient->getTwo(), &scoped1);

    EXPECT_EQ(scoped2.getOne(), &singleton);
    EXPECT_EQ(scoped2.getTwo(), &scoped1);
    EXPECT_NE(scoped2.getThree(), transient);

    EXPECT_EQ(scoped3.getOne(), &singleton);
    EXPECT_EQ(scoped3.getTwo(), &scoped1);
    EXPECT_TRUE(scoped3.makeThree());
    EXPECT_NE(scoped3.makeThree(), transient);
}