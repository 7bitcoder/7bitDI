#include <gtest/gtest.h>
#include <memory>

#include "../Helpers/Classes/Basic.hpp"
#include "../Helpers/Classes/Complex.hpp"
#include "../Helpers/Classes/Dependencies.hpp"
#include <SevenBit/DI/ServiceCollection.hpp>

class ScopeTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    ScopeTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ScopeTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ScopeTest, ShouldMakeScopedProvider)
{
    const auto provider = sb::di::ServiceCollection{}
                              .addSingleton<TestClass1>()
                              .addSingleton<TestClass2>()
                              .addSingleton<TestClass3>()
                              .buildServiceProvider();

    auto act = [&] { auto scoped = provider.createScope(); };

    EXPECT_NO_THROW(act());
}

TEST_F(ScopeTest, ShouldGetProperServicesRefWithScopedProvider)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    auto scoped1 = provider.createScope();
    auto scoped2 = provider.createScope();
    auto scoped3 = provider.createScope();

    EXPECT_EQ(&scoped3.getService<TestClass1>(), &scoped3.getService<TestClass1>());
    EXPECT_EQ(&scoped3.getService<TestClass1>(), &scoped2.getService<TestClass1>());
    EXPECT_EQ(&scoped3.getService<TestClass1>(), &scoped1.getService<TestClass1>());
    EXPECT_EQ(&scoped3.getService<TestClass1>(), &provider.getService<TestClass1>());
    EXPECT_EQ(&scoped2.getService<TestClass1>(), &scoped2.getService<TestClass1>());
    EXPECT_EQ(&scoped2.getService<TestClass1>(), &scoped1.getService<TestClass1>());
    EXPECT_EQ(&scoped2.getService<TestClass1>(), &provider.getService<TestClass1>());
    EXPECT_EQ(&scoped1.getService<TestClass1>(), &scoped1.getService<TestClass1>());
    EXPECT_EQ(&scoped1.getService<TestClass1>(), &provider.getService<TestClass1>());
    EXPECT_EQ(&provider.getService<TestClass1>(), &provider.getService<TestClass1>());

    EXPECT_EQ(&scoped3.getService<TestClass2>(), &scoped3.getService<TestClass2>());
    EXPECT_NE(&scoped3.getService<TestClass2>(), &scoped2.getService<TestClass2>());
    EXPECT_NE(&scoped3.getService<TestClass2>(), &scoped1.getService<TestClass2>());
    EXPECT_NE(&scoped3.getService<TestClass2>(), &provider.getService<TestClass2>());
    EXPECT_EQ(&scoped2.getService<TestClass2>(), &scoped2.getService<TestClass2>());
    EXPECT_NE(&scoped2.getService<TestClass2>(), &scoped1.getService<TestClass2>());
    EXPECT_NE(&scoped2.getService<TestClass2>(), &provider.getService<TestClass2>());
    EXPECT_EQ(&scoped1.getService<TestClass2>(), &scoped1.getService<TestClass2>());
    EXPECT_NE(&scoped1.getService<TestClass2>(), &provider.getService<TestClass2>());
    EXPECT_EQ(&provider.getService<TestClass2>(), &provider.getService<TestClass2>());

    EXPECT_NE(scoped3.createService<TestClass3>(), scoped3.createService<TestClass3>());
    EXPECT_NE(scoped3.createService<TestClass3>(), scoped2.createService<TestClass3>());
    EXPECT_NE(scoped3.createService<TestClass3>(), scoped1.createService<TestClass3>());
    EXPECT_NE(scoped3.createService<TestClass3>(), provider.createService<TestClass3>());
    EXPECT_NE(scoped2.createService<TestClass3>(), scoped2.createService<TestClass3>());
    EXPECT_NE(scoped2.createService<TestClass3>(), scoped1.createService<TestClass3>());
    EXPECT_NE(scoped2.createService<TestClass3>(), provider.createService<TestClass3>());
    EXPECT_NE(scoped1.createService<TestClass3>(), scoped1.createService<TestClass3>());
    EXPECT_NE(scoped1.createService<TestClass3>(), provider.createService<TestClass3>());
    EXPECT_NE(provider.createService<TestClass3>(), provider.createService<TestClass3>());
}

TEST_F(ScopeTest, ShouldReturnProperSelfForScope)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    auto scoped1 = provider.createScope();
    auto scoped2 = provider.createScope();
    auto scoped3 = provider.createScope();

    EXPECT_EQ(provider.tryGetService<sb::di::ServiceProvider>(), &provider);
    EXPECT_EQ(scoped1.tryGetService<sb::di::ServiceProvider>(), &scoped1);
    EXPECT_EQ(scoped3.tryGetService<sb::di::ServiceProvider>(), &scoped3);
}

TEST_F(ScopeTest, ShouldGetServicesDeeperRefWithScopedProvider)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestDependencyClass>()
                        .addScoped<TestDependencyPtrClass1>()
                        .addTransient<TestDependencyRefClass1>()
                        .buildServiceProvider();

    auto scoped = provider.createScope();

    auto &singletonFromTop = provider.getService<TestDependencyClass>();
    auto &singletonFromScoped = scoped.getService<TestDependencyClass>();
    EXPECT_EQ(&singletonFromTop, &singletonFromScoped);

    auto &scopedFromTop = provider.getService<TestDependencyPtrClass1>();
    auto &scopedFromScoped = scoped.getService<TestDependencyPtrClass1>();
    EXPECT_NE(&scopedFromTop, &scopedFromScoped);
    EXPECT_EQ(scopedFromTop._test1, &singletonFromTop);
    EXPECT_EQ(scopedFromTop._test1, scopedFromScoped._test1);

    const auto transientFromTop = provider.createService<TestDependencyRefClass1>();
    const auto transientFromScoped = scoped.createService<TestDependencyRefClass1>();
    EXPECT_NE(transientFromTop, transientFromScoped);
    EXPECT_EQ(&transientFromTop->_test1, &singletonFromTop);
    EXPECT_EQ(&transientFromTop->_test1, &transientFromScoped->_test1);
}

TEST_F(ScopeTest, ShouldGetComplexServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<ITestComplexClass1, TestComplexClass1>()
                        .addScoped<ITestComplexClass2, TestComplexClass2>()
                        .addTransient<ITestComplexClass3, TestComplexClass3>()
                        .addScoped<ITestComplexClass5, TestComplexClass5>()
                        .addScoped<ITestComplexClass4>([](ITestComplexClass1 &other1, ITestComplexClass2 *other2,
                                                          std::unique_ptr<ITestComplexClass3> other3) {
                            return std::make_unique<TestComplexClass4>(&other1, other2, std::move(other3));
                        })
                        .buildServiceProvider();

    auto &singleton = provider.getService<ITestComplexClass1>();
    auto &scoped1 = provider.getService<ITestComplexClass2>();
    const auto transient = provider.createService<ITestComplexClass3>();
    auto &scoped2 = provider.getService<ITestComplexClass4>();
    auto &scoped3 = provider.getService<ITestComplexClass5>();

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
