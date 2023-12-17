#include <gtest/gtest.h>
#include <memory>

#include "../Helpers/Classes/Basic.hpp"
#include "../Helpers/Classes/CirularDependency.hpp"
#include "../Helpers/Classes/Complex.hpp"
#include "../Helpers/Classes/Inherit.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceCollection.hpp"

class ComplexInjectionsTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    ComplexInjectionsTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ComplexInjectionsTest() override = default;

    static void TearDownTestSuite() {}
};

// buildServiceProvider Tests

TEST_F(ComplexInjectionsTest, ShouldTryGetService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<ITestComplexClass1, TestComplexClass1>()
                        .addSingleton<ITestComplexClass2, TestComplexClass2>()
                        .addTransient<ITestComplexClass3, TestComplexClass3>()
                        .addScoped<ITestComplexClass4, TestComplexClass4>()
                        .addScoped<ITestComplexClass5, TestComplexClass5>()
                        .addScoped<ITestComplexClass6, TestComplexClass6>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetService<ITestComplexClass1>());
    EXPECT_TRUE(provider.tryGetService<ITestComplexClass2>());
    EXPECT_FALSE(provider.tryGetService<ITestComplexClass3>());
    EXPECT_TRUE(provider.tryGetService<ITestComplexClass4>());
    EXPECT_TRUE(provider.tryGetService<ITestComplexClass5>());
    EXPECT_TRUE(provider.tryGetService<ITestComplexClass6>());
}

TEST_F(ComplexInjectionsTest, ShouldTryGetDeepNestedService)
{

    sb::di::ServiceCollection collection;
    TestNested<50>::addAllRecurse(collection);

    auto provider = collection.buildServiceProvider();

    auto nested = provider.getService<TestNested<50>>();
    nested.checkRecurse(provider);
}
