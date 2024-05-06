#include <gtest/gtest.h>
#include <thread>

#include "../Helpers/Classes/Basic.hpp"
#include "../Helpers/Classes/CirularDependency.hpp"
#include "../Helpers/Classes/Complex.hpp"
#include <SevenBit/DI/Exceptions.hpp>
#include <SevenBit/DI/ServiceCollection.hpp>

class ThreadSafeTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    ThreadSafeTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ThreadSafeTest() override = default;

    static void TearDownTestSuite() {}
};

void getSafeServices(sb::di::ServiceProvider &provider)
{
    auto &service1 = provider.getService<ITestComplexClass1>();
    auto &service2 = provider.getService<ITestComplexClass2>();
    auto service3 = provider.createService<ITestComplexClass3>();
    auto &service4 = provider.getService<ITestComplexClass4>();
    auto &service5 = provider.getService<ITestComplexClass5>();
    auto &service6 = provider.getService<ITestComplexClass6>();

    EXPECT_EQ(service1.number(), 1);
    EXPECT_EQ(service2.number(), 2);
    EXPECT_EQ(service3->number(), 3);
    EXPECT_EQ(service4.number(), 4);
    EXPECT_EQ(service5.number(), 5);
    EXPECT_EQ(service6.number(), 6);
    EXPECT_EQ(service2.getOne(), &service1);
    EXPECT_EQ(service3->getOne(), &service1);
    EXPECT_EQ(service3->getTwo(), &service2);
    EXPECT_EQ(service4.getOne(), &service1);
    EXPECT_EQ(service4.getTwo(), &service2);
    EXPECT_NE(service4.getThree().get(), service3.get());
    EXPECT_EQ(service5.getOne(), &service1);
    EXPECT_EQ(service5.getTwo(), &service2);
    EXPECT_NE(service5.makeThree(), service3);
    EXPECT_EQ(&service6.getOne(), &service1);
    EXPECT_EQ(&service6.getTwo(), &service2);
    EXPECT_FALSE(service6.getNonExisting());
    EXPECT_NE(service6.makeThree(), service3);
}

TEST_F(ThreadSafeTest, ShouldGetSafeServices)
{
    sb::di::ServiceProviderOptions options;
    options.threadSafe = true;

    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<ITestComplexClass1, TestComplexClass1>()
                        .addSingleton<ITestComplexClass2, TestComplexClass2>()
                        .addTransient<ITestComplexClass3, TestComplexClass3>()
                        .addScoped<ITestComplexClass4, TestComplexClass4>()
                        .addScoped<ITestComplexClass5, TestComplexClass5>()
                        .addScoped<ITestComplexClass6, TestComplexClass6>()
                        .buildServiceProvider(options);

    std::vector<std::thread> threads;
    constexpr size_t maxThreads = 50;
    for (size_t i = 0; i < maxThreads; ++i)
    {
        threads.emplace_back(getSafeServices, std::ref(provider));
    }
    for (auto &th : threads)
    {
        th.join();
    }
}
