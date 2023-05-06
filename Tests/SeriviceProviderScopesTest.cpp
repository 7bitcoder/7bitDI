#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <thread>

#include "Classes/BasicInherit.hpp"
#include "Classes/BasicTest.hpp"
#include "Classes/CirularDependency.hpp"
#include "SevenBit/Exceptions.hpp"
#include "SevenBit/ServiceCollection.hpp"

class SeriviceProviderScopesTest : public ::testing::Test
{
  protected:
    sb::ServiceCollection collection;

    static void SetUpTestSuite() {}

    SeriviceProviderScopesTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~SeriviceProviderScopesTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(SeriviceProviderScopesTest, MakeScopedProvider)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addSingleton<TestClass2>()
                        .addSingleton<TestClass3>()
                        .buildServiceProvider();

    EXPECT_NO_THROW(provider->createScope());
}

TEST_F(SeriviceProviderScopesTest, CheckServicesRefWithScopedProvider)
{
    auto provider = sb::ServiceCollection{}
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