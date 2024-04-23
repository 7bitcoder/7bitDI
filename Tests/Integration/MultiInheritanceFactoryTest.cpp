#include <gtest/gtest.h>
#include <memory>

#include "../Helpers/Classes/MultiInherit.hpp"
#include <SevenBit/DI/ServiceCollection.hpp>

class MultiInheritanceFactoryTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    MultiInheritanceFactoryTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~MultiInheritanceFactoryTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(MultiInheritanceFactoryTest, ShouldTryGetService)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass1>([] { return TestMultiInheritClass2{}; })
                        .addSingleton<TestMultiInherit2Class1>([] { return TestMultiInheritClass2{}; })
                        .addTransient<TestMultiInheritClass2>([] { return TestMultiInheritClass3{}; })
                        .addTransient<TestMultiInherit3Class1>([] { return TestMultiInheritClass3{}; })
                        .addScoped<TestMultiInheritClass3>([] { return TestMultiInheritClass4{}; })
                        .addScoped<TestMultiInherit4Class4>([] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider(options);

    auto service1 = provider.tryGetService<TestMultiInheritClass1>();
    auto service2 = provider.tryGetService<TestMultiInherit2Class1>();
    auto service5 = provider.tryGetService<TestMultiInheritClass3>();
    auto service6 = provider.tryGetService<TestMultiInherit4Class4>();

    EXPECT_TRUE(service1);
    EXPECT_TRUE(service2);
    EXPECT_TRUE(service5);
    EXPECT_TRUE(service6);
    EXPECT_FALSE(provider.tryGetService<TestMultiInheritClass2>());
    EXPECT_FALSE(provider.tryGetService<TestMultiInherit3Class1>());
    EXPECT_EQ(service1->first(), 2);
    EXPECT_EQ(service1->arr[0], 1);
    EXPECT_EQ(service1->arr[8], 9);
    EXPECT_EQ(service1->end, 1);
    EXPECT_EQ(service2->second(), 22);
    EXPECT_EQ(service2->arr[0], 1);
    EXPECT_EQ(service2->arr[8], 9);
    EXPECT_EQ(service2->end, 2);
    EXPECT_EQ(service5->second(), 44);
    EXPECT_EQ(service5->third(), 444);
    EXPECT_EQ(service5->arr[0], 1);
    EXPECT_EQ(service5->arr[8], 9);
    EXPECT_EQ(service5->end, 5);
    EXPECT_EQ(service6->fourth(), 4444);
    EXPECT_EQ(service6->arr[0], 1);
    EXPECT_EQ(service6->arr[8], 9);
    EXPECT_EQ(service6->end, 6);
}

TEST_F(MultiInheritanceFactoryTest, ShouldTryGetOneLastServiceSingleton)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInherit2Class1>([] { return TestMultiInheritClass2{}; })
                        .addSingleton<TestMultiInherit2Class1>([] { return TestMultiInheritClass3{}; })
                        .addSingleton<TestMultiInherit2Class1>([] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider();

    const auto service = provider.tryGetService<TestMultiInherit2Class1>();

    EXPECT_TRUE(service);
    EXPECT_EQ(service->second(), 44);
    EXPECT_EQ(service->arr[0], 1);
    EXPECT_EQ(service->arr[8], 9);
    EXPECT_EQ(service->end, 2);
}

TEST_F(MultiInheritanceFactoryTest, ShouldTryGetOneLastServiceScoped)
{
    auto provider = sb::di::ServiceCollection{}
                        .addScoped<TestMultiInherit2Class1>([] { return TestMultiInheritClass2{}; })
                        .addScoped<TestMultiInherit2Class1>([] { return TestMultiInheritClass3{}; })
                        .addScoped<TestMultiInherit2Class1>([] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider();

    const auto service = provider.tryGetService<TestMultiInherit2Class1>();

    EXPECT_TRUE(service);
    EXPECT_EQ(service->second(), 44);
    EXPECT_EQ(service->arr[0], 1);
    EXPECT_EQ(service->arr[8], 9);
    EXPECT_EQ(service->end, 2);
}

TEST_F(MultiInheritanceFactoryTest, ShouldGetService)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass1>([] { return TestMultiInheritClass2{}; })
                        .addSingleton<TestMultiInherit2Class1>([] { return TestMultiInheritClass2{}; })
                        .addTransient<TestMultiInheritClass2>([] { return TestMultiInheritClass3{}; })
                        .addTransient<TestMultiInherit3Class1>([] { return TestMultiInheritClass3{}; })
                        .addScoped<TestMultiInheritClass3>([] { return TestMultiInheritClass4{}; })
                        .addScoped<TestMultiInherit4Class4>([] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider(options);

    auto &service1 = provider.getService<TestMultiInheritClass1>();
    auto &service2 = provider.getService<TestMultiInherit2Class1>();
    auto &service5 = provider.getService<TestMultiInheritClass3>();
    auto &service6 = provider.getService<TestMultiInherit4Class4>();

    EXPECT_THROW(provider.getService<TestMultiInheritClass2>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.getService<TestMultiInherit3Class1>(), sb::di::ServiceNotFoundException);
    EXPECT_EQ(service1.first(), 2);
    EXPECT_EQ(service1.arr[0], 1);
    EXPECT_EQ(service1.arr[8], 9);
    EXPECT_EQ(service1.end, 1);
    EXPECT_EQ(service2.second(), 22);
    EXPECT_EQ(service2.arr[0], 1);
    EXPECT_EQ(service2.arr[8], 9);
    EXPECT_EQ(service2.end, 2);
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

TEST_F(MultiInheritanceFactoryTest, ShouldGetOneLastServiceSingleton)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInherit2Class1>([] { return TestMultiInheritClass2{}; })
                        .addSingleton<TestMultiInherit2Class1>([] { return TestMultiInheritClass3{}; })
                        .addSingleton<TestMultiInherit2Class1>([] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider();

    auto &service = provider.getService<TestMultiInherit2Class1>();

    EXPECT_EQ(service.second(), 44);
    EXPECT_EQ(service.arr[0], 1);
    EXPECT_EQ(service.arr[8], 9);
    EXPECT_EQ(service.end, 2);
}

TEST_F(MultiInheritanceFactoryTest, ShouldGetOneLastServiceScoped)
{
    auto provider = sb::di::ServiceCollection{}
                        .addScoped<TestMultiInherit2Class1>([] { return TestMultiInheritClass2{}; })
                        .addScoped<TestMultiInherit2Class1>([] { return TestMultiInheritClass3{}; })
                        .addScoped<TestMultiInherit2Class1>([] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider();

    auto &service = provider.getService<TestMultiInherit2Class1>();

    EXPECT_EQ(service.second(), 44);
    EXPECT_EQ(service.arr[0], 1);
    EXPECT_EQ(service.arr[8], 9);
    EXPECT_EQ(service.end, 2);
}

TEST_F(MultiInheritanceFactoryTest, ShouldGetServices)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass1>([] { return TestMultiInheritClass2{}; })
                        .addSingleton<TestMultiInherit2Class1>([] { return TestMultiInheritClass2{}; })
                        .addTransient<TestMultiInheritClass2>([] { return TestMultiInheritClass3{}; })
                        .addTransient<TestMultiInherit3Class1>([] { return TestMultiInheritClass3{}; })
                        .addScoped<TestMultiInheritClass3>([] { return TestMultiInheritClass4{}; })
                        .addScoped<TestMultiInherit4Class4>([] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider(options);

    EXPECT_EQ(provider.getServices<TestMultiInheritClass1>().size(), 1);
    EXPECT_EQ(provider.getServices<TestMultiInherit2Class1>().size(), 1);
    EXPECT_TRUE(provider.getServices<TestMultiInheritClass2>().empty());
    EXPECT_TRUE(provider.getServices<TestMultiInherit3Class1>().empty());
    EXPECT_EQ(provider.getServices<TestMultiInheritClass3>().size(), 1);
    EXPECT_EQ(provider.getServices<TestMultiInherit4Class4>().size(), 1);
}

TEST_F(MultiInheritanceFactoryTest, ShouldGetCastedServicesInOrder)
{
    auto provider = sb::di::ServiceCollection{}
                        .addScoped<TestMultiInherit2Class1>([] { return TestMultiInheritClass2{}; })
                        .addScoped<TestMultiInherit2Class1>([] { return TestMultiInheritClass3{}; })
                        .addScoped<TestMultiInherit2Class1>([] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider();

    const auto services = provider.getServices<TestMultiInherit2Class1>();

    EXPECT_EQ(services.size(), 3);
    EXPECT_EQ(services[0]->second(), 22);
    EXPECT_EQ(services[1]->second(), 33);
    EXPECT_EQ(services[2]->second(), 44);
}

TEST_F(MultiInheritanceFactoryTest, ShouldGetServicesInOrderAfterNormalGet)
{
    auto provider = sb::di::ServiceCollection{}
                        .addScoped<TestMultiInherit2Class1>([] { return TestMultiInheritClass2{}; })
                        .addScoped<TestMultiInherit2Class1>([] { return TestMultiInheritClass3{}; })
                        .addScoped<TestMultiInherit2Class1>([] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetService<TestMultiInherit2Class1>());
    const auto services = provider.getServices<TestMultiInherit2Class1>();
    EXPECT_EQ(services.size(), 3);
    EXPECT_EQ(services[0]->second(), 22);
    EXPECT_EQ(services[1]->second(), 33);
    EXPECT_EQ(services[2]->second(), 44);
}

TEST_F(MultiInheritanceFactoryTest, ShouldTryCreateService)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass1>([] { return TestMultiInheritClass2{}; })
                        .addSingleton<TestMultiInherit2Class1>([] { return TestMultiInheritClass2{}; })
                        .addTransient<TestMultiInheritClass2>([] { return TestMultiInheritClass3{}; })
                        .addTransient<TestMultiInherit3Class1>([] { return TestMultiInheritClass3{}; })
                        .addScoped<TestMultiInheritClass3>([] { return TestMultiInheritClass4{}; })
                        .addScoped<TestMultiInherit4Class4>([] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider(options);

    const auto service3 = provider.tryCreateService<TestMultiInheritClass2>();
    const auto service4 = provider.tryCreateService<TestMultiInherit3Class1>();

    EXPECT_TRUE(service3);
    EXPECT_TRUE(service4);
    EXPECT_FALSE(provider.tryCreateService<TestMultiInheritClass1>());
    EXPECT_FALSE(provider.tryCreateService<TestMultiInherit2Class1>());
    EXPECT_FALSE(provider.tryCreateService<TestMultiInheritClass3>());
    EXPECT_FALSE(provider.tryCreateService<TestMultiInherit4Class4>());
}

TEST_F(MultiInheritanceFactoryTest, ShouldCreateService)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass1>([] { return TestMultiInheritClass2{}; })
                        .addSingleton<TestMultiInherit2Class1>([] { return TestMultiInheritClass2{}; })
                        .addTransient<TestMultiInheritClass2>([] { return TestMultiInheritClass3{}; })
                        .addTransient<TestMultiInherit3Class1>([] { return TestMultiInheritClass3{}; })
                        .addScoped<TestMultiInheritClass3>([] { return TestMultiInheritClass4{}; })
                        .addScoped<TestMultiInherit4Class4>([] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider(options);

    auto service3 = provider.createService<TestMultiInheritClass2>();
    auto service4 = provider.createService<TestMultiInherit3Class1>();

    EXPECT_TRUE(service3);
    EXPECT_TRUE(service4);
    EXPECT_THROW(provider.createService<TestMultiInheritClass1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createService<TestMultiInherit2Class1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createService<TestMultiInheritClass3>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createService<TestMultiInherit4Class4>(), sb::di::ServiceNotFoundException);
}

TEST_F(MultiInheritanceFactoryTest, ShouldNotCreateServiceInPlace)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass1>([] { return TestMultiInheritClass2{}; })
                        .addSingleton<TestMultiInherit2Class1>([] { return TestMultiInheritClass2{}; })
                        .addTransient<TestMultiInheritClass2>([] { return TestMultiInheritClass3{}; })
                        .addTransient<TestMultiInherit3Class1>([] { return TestMultiInheritClass3{}; })
                        .addScoped<TestMultiInheritClass3>([] { return TestMultiInheritClass4{}; })
                        .addScoped<TestMultiInherit4Class4>([] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider(options);

    EXPECT_THROW(provider.createServiceInPlace<TestMultiInheritClass2>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestMultiInherit3Class1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestMultiInheritClass1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestMultiInherit2Class1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestMultiInheritClass3>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestMultiInherit4Class4>(), sb::di::ServiceNotFoundException);
}

TEST_F(MultiInheritanceFactoryTest, ShouldCreateServices)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass1>([] { return TestMultiInheritClass2{}; })
                        .addSingleton<TestMultiInherit2Class1>([] { return TestMultiInheritClass2{}; })
                        .addTransient<TestMultiInheritClass2>([] { return TestMultiInheritClass3{}; })
                        .addTransient<TestMultiInherit3Class1>([] { return TestMultiInheritClass3{}; })
                        .addScoped<TestMultiInheritClass3>([] { return TestMultiInheritClass4{}; })
                        .addScoped<TestMultiInherit4Class4>([] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider(options);

    EXPECT_TRUE(provider.createServices<TestMultiInheritClass1>().empty());
    EXPECT_TRUE(provider.createServices<TestMultiInherit2Class1>().empty());
    EXPECT_EQ(provider.createServices<TestMultiInheritClass2>().size(), 1);
    EXPECT_EQ(provider.createServices<TestMultiInherit3Class1>().size(), 1);
    EXPECT_TRUE(provider.createServices<TestMultiInheritClass3>().empty());
    EXPECT_TRUE(provider.createServices<TestMultiInherit4Class4>().empty());
}

TEST_F(MultiInheritanceFactoryTest, ShouldCreateServicesInOrder)
{
    auto provider = sb::di::ServiceCollection{}
                        .addTransient<TestMultiInherit2Class1>([] { return TestMultiInheritClass2{}; })
                        .addTransient<TestMultiInherit2Class1>([] { return TestMultiInheritClass3{}; })
                        .addTransient<TestMultiInherit2Class1>([] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider();

    const auto services = provider.createServices<TestMultiInherit2Class1>();

    EXPECT_EQ(services.size(), 3);
    EXPECT_EQ(services[0]->second(), 22);
    EXPECT_EQ(services[1]->second(), 33);
    EXPECT_EQ(services[2]->second(), 44);
}
