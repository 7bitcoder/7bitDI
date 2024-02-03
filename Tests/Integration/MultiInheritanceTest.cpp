#include <gtest/gtest.h>
#include <memory>

#include "../Helpers/Classes/MultiInherit.hpp"
#include "SevenBit/DI/ServiceCollection.hpp"

class MultiInheritanceTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    MultiInheritanceTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~MultiInheritanceTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(MultiInheritanceTest, ShouldGetService)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass1, TestMultiInheritClass2>()
                        .addSingleton<TestMultiInherit2Class1, TestMultiInheritClass2>()
                        .addTransient<TestMultiInheritClass2, TestMultiInheritClass3>()
                        .addTransient<TestMultiInherit3Class1, TestMultiInheritClass3>()
                        .addScoped<TestMultiInheritClass3, TestMultiInheritClass4>()
                        .addScoped<TestMultiInherit4Class4, TestMultiInheritClass4>()
                        .buildServiceProvider(options);

    auto &service1 = provider.getService<TestMultiInheritClass1>();
    auto &service2 = provider.getService<TestMultiInherit2Class1>();
    auto service3 = provider.createService<TestMultiInheritClass2>();
    auto service4 = provider.createService<TestMultiInherit3Class1>();
    auto &service5 = provider.getService<TestMultiInheritClass3>();
    auto &service6 = provider.getService<TestMultiInherit4Class4>();

    EXPECT_EQ(service1.first(), 2);
    EXPECT_EQ(service1.arr[0], 1);
    EXPECT_EQ(service1.arr[8], 9);
    EXPECT_EQ(service1.end, 1);
    EXPECT_EQ(service2.second(), 22);
    EXPECT_EQ(service2.arr[0], 1);
    EXPECT_EQ(service2.arr[8], 9);
    EXPECT_EQ(service2.end, 2);
    EXPECT_EQ(service3->first(), 3);
    EXPECT_EQ(service3->second(), 33);
    EXPECT_EQ(service3->arr[0], 1);
    EXPECT_EQ(service3->arr[8], 9);
    EXPECT_EQ(service3->end, 3);
    EXPECT_EQ(service4->third(), 333);
    EXPECT_EQ(service4->arr[0], 1);
    EXPECT_EQ(service4->arr[8], 9);
    EXPECT_EQ(service4->end, 4);
    EXPECT_EQ(service5.first(), 4);
    EXPECT_EQ(service5.second(), 44);
    EXPECT_EQ(service5.third(), 444);
    EXPECT_EQ(service5.arr[0], 1);
    EXPECT_EQ(service5.arr[8], 9);
    EXPECT_EQ(service5.end, 5);
    EXPECT_EQ(service6.fourth(), 4444);
    EXPECT_EQ(service6.arr[0], 1);
    EXPECT_EQ(service6.arr[8], 9);
    EXPECT_EQ(service6.end, 6);
}

TEST_F(MultiInheritanceTest, ShouldGetAliasesService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass2>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass2>()
                        .addAlias<TestMultiInherit2Class1, TestMultiInheritClass2>()
                        .addTransient<TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass3, TestMultiInheritClass4>()
                        .addAlias<TestMultiInherit4Class4, TestMultiInheritClass4>()
                        .buildServiceProvider();

    auto &service1 = provider.getService<TestMultiInheritClass1>();
    auto &service2 = provider.getService<TestMultiInherit2Class1>();
    auto service3 = provider.createService<TestMultiInheritClass3>();
    auto service4 = provider.createService<TestMultiInherit4Class4>();

    EXPECT_EQ(service1.first(), 2);
    EXPECT_EQ(service1.arr[0], 1);
    EXPECT_EQ(service1.arr[8], 9);
    EXPECT_EQ(service1.end, 1);
    EXPECT_EQ(service2.second(), 22);
    EXPECT_EQ(service2.arr[0], 1);
    EXPECT_EQ(service2.arr[8], 9);
    EXPECT_EQ(service2.end, 2);
    EXPECT_EQ(service3->first(), 4);
    EXPECT_EQ(service3->second(), 44);
    EXPECT_EQ(service3->third(), 444);
    EXPECT_EQ(service3->arr[0], 1);
    EXPECT_EQ(service3->arr[8], 9);
    EXPECT_EQ(service3->end, 5);
    EXPECT_EQ(service4->fourth(), 4444);
    EXPECT_EQ(service4->arr[0], 1);
    EXPECT_EQ(service4->arr[8], 9);
    EXPECT_EQ(service4->end, 6);
}
