#include <gtest/gtest.h>
#include <memory>

#include "../../Helpers/Classes/MultiInherit.hpp"
#include <SevenBit/DI/ServiceCollection.hpp>

class MultiInheritanceFactoryKeyedTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    MultiInheritanceFactoryKeyedTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~MultiInheritanceFactoryKeyedTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(MultiInheritanceFactoryKeyedTest, ShouldTryGetService)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInheritClass1>("key", [] { return TestMultiInheritClass2{}; })
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass2{}; })
                        .addKeyedTransient<TestMultiInheritClass2>("key", [] { return TestMultiInheritClass3{}; })
                        .addKeyedTransient<TestMultiInherit3Class1>("key", [] { return TestMultiInheritClass3{}; })
                        .addKeyedScoped<TestMultiInheritClass3>("key", [] { return TestMultiInheritClass4{}; })
                        .addKeyedScoped<TestMultiInherit4Class4>("key", [] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider(options);

    auto service1 = provider.tryGetKeyedService<TestMultiInheritClass1>("key");
    auto service2 = provider.tryGetKeyedService<TestMultiInherit2Class1>("key");
    auto service5 = provider.tryGetKeyedService<TestMultiInheritClass3>("key");
    auto service6 = provider.tryGetKeyedService<TestMultiInherit4Class4>("key");

    EXPECT_TRUE(service1);
    EXPECT_TRUE(service2);
    EXPECT_TRUE(service5);
    EXPECT_TRUE(service6);
    EXPECT_FALSE(provider.tryGetKeyedService<TestMultiInheritClass2>("key"));
    EXPECT_FALSE(provider.tryGetKeyedService<TestMultiInherit3Class1>("key"));
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

TEST_F(MultiInheritanceFactoryKeyedTest, ShouldTryGetOneLastServiceSingleton)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass2{}; })
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass3{}; })
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider();

    const auto service = provider.tryGetKeyedService<TestMultiInherit2Class1>("key");

    EXPECT_TRUE(service);
    EXPECT_EQ(service->second(), 44);
    EXPECT_EQ(service->arr[0], 1);
    EXPECT_EQ(service->arr[8], 9);
    EXPECT_EQ(service->end, 2);
}

TEST_F(MultiInheritanceFactoryKeyedTest, ShouldTryGetOneLastServiceScoped)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedScoped<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass2{}; })
                        .addKeyedScoped<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass3{}; })
                        .addKeyedScoped<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider();

    const auto service = provider.tryGetKeyedService<TestMultiInherit2Class1>("key");

    EXPECT_TRUE(service);
    EXPECT_EQ(service->second(), 44);
    EXPECT_EQ(service->arr[0], 1);
    EXPECT_EQ(service->arr[8], 9);
    EXPECT_EQ(service->end, 2);
}

TEST_F(MultiInheritanceFactoryKeyedTest, ShouldGetService)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInheritClass1>("key", [] { return TestMultiInheritClass2{}; })
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass2{}; })
                        .addKeyedTransient<TestMultiInheritClass2>("key", [] { return TestMultiInheritClass3{}; })
                        .addKeyedTransient<TestMultiInherit3Class1>("key", [] { return TestMultiInheritClass3{}; })
                        .addKeyedScoped<TestMultiInheritClass3>("key", [] { return TestMultiInheritClass4{}; })
                        .addKeyedScoped<TestMultiInherit4Class4>("key", [] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider(options);

    auto &service1 = provider.getKeyedService<TestMultiInheritClass1>("key");
    auto &service2 = provider.getKeyedService<TestMultiInherit2Class1>("key");
    auto &service5 = provider.getKeyedService<TestMultiInheritClass3>("key");
    auto &service6 = provider.getKeyedService<TestMultiInherit4Class4>("key");

    EXPECT_THROW(provider.getKeyedService<TestMultiInheritClass2>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.getKeyedService<TestMultiInherit3Class1>("key"), sb::di::ServiceNotFoundException);
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

TEST_F(MultiInheritanceFactoryKeyedTest, ShouldGetOneLastServiceSingleton)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass2{}; })
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass3{}; })
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider();

    auto &service = provider.getKeyedService<TestMultiInherit2Class1>("key");

    EXPECT_EQ(service.second(), 44);
    EXPECT_EQ(service.arr[0], 1);
    EXPECT_EQ(service.arr[8], 9);
    EXPECT_EQ(service.end, 2);
}

TEST_F(MultiInheritanceFactoryKeyedTest, ShouldGetOneLastServiceScoped)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedScoped<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass2{}; })
                        .addKeyedScoped<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass3{}; })
                        .addKeyedScoped<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider();

    auto &service = provider.getKeyedService<TestMultiInherit2Class1>("key");

    EXPECT_EQ(service.second(), 44);
    EXPECT_EQ(service.arr[0], 1);
    EXPECT_EQ(service.arr[8], 9);
    EXPECT_EQ(service.end, 2);
}

TEST_F(MultiInheritanceFactoryKeyedTest, ShouldGetServices)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInheritClass1>("key", [] { return TestMultiInheritClass2{}; })
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass2{}; })
                        .addKeyedTransient<TestMultiInheritClass2>("key", [] { return TestMultiInheritClass3{}; })
                        .addKeyedTransient<TestMultiInherit3Class1>("key", [] { return TestMultiInheritClass3{}; })
                        .addKeyedScoped<TestMultiInheritClass3>("key", [] { return TestMultiInheritClass4{}; })
                        .addKeyedScoped<TestMultiInherit4Class4>("key", [] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider(options);

    EXPECT_EQ(provider.getKeyedServices<TestMultiInheritClass1>("key").size(), 1);
    EXPECT_EQ(provider.getKeyedServices<TestMultiInherit2Class1>("key").size(), 1);
    EXPECT_TRUE(provider.getKeyedServices<TestMultiInheritClass2>("key").empty());
    EXPECT_TRUE(provider.getKeyedServices<TestMultiInherit3Class1>("key").empty());
    EXPECT_EQ(provider.getKeyedServices<TestMultiInheritClass3>("key").size(), 1);
    EXPECT_EQ(provider.getKeyedServices<TestMultiInherit4Class4>("key").size(), 1);
}

TEST_F(MultiInheritanceFactoryKeyedTest, ShouldGetCastedServicesInOrder)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedScoped<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass2{}; })
                        .addKeyedScoped<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass3{}; })
                        .addKeyedScoped<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider();

    const auto services = provider.getKeyedServices<TestMultiInherit2Class1>("key");

    EXPECT_EQ(services.size(), 3);
    EXPECT_EQ(services[0]->second(), 22);
    EXPECT_EQ(services[1]->second(), 33);
    EXPECT_EQ(services[2]->second(), 44);
}

TEST_F(MultiInheritanceFactoryKeyedTest, ShouldGetServicesInOrderAfterNormalGet)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedScoped<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass2{}; })
                        .addKeyedScoped<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass3{}; })
                        .addKeyedScoped<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetKeyedService<TestMultiInherit2Class1>("key"));
    const auto services = provider.getKeyedServices<TestMultiInherit2Class1>("key");
    EXPECT_EQ(services.size(), 3);
    EXPECT_EQ(services[0]->second(), 22);
    EXPECT_EQ(services[1]->second(), 33);
    EXPECT_EQ(services[2]->second(), 44);
}

TEST_F(MultiInheritanceFactoryKeyedTest, ShouldTryCreateService)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInheritClass1>("key", [] { return TestMultiInheritClass2{}; })
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass2{}; })
                        .addKeyedTransient<TestMultiInheritClass2>("key", [] { return TestMultiInheritClass3{}; })
                        .addKeyedTransient<TestMultiInherit3Class1>("key", [] { return TestMultiInheritClass3{}; })
                        .addKeyedScoped<TestMultiInheritClass3>("key", [] { return TestMultiInheritClass4{}; })
                        .addKeyedScoped<TestMultiInherit4Class4>("key", [] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider(options);

    const auto service3 = provider.tryCreateKeyedService<TestMultiInheritClass2>("key");
    const auto service4 = provider.tryCreateKeyedService<TestMultiInherit3Class1>("key");

    EXPECT_TRUE(service3);
    EXPECT_TRUE(service4);
    EXPECT_FALSE(provider.tryCreateKeyedService<TestMultiInheritClass1>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestMultiInherit2Class1>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestMultiInheritClass3>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestMultiInherit4Class4>("key"));
}

TEST_F(MultiInheritanceFactoryKeyedTest, ShouldCreateService)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInheritClass1>("key", [] { return TestMultiInheritClass2{}; })
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass2{}; })
                        .addKeyedTransient<TestMultiInheritClass2>("key", [] { return TestMultiInheritClass3{}; })
                        .addKeyedTransient<TestMultiInherit3Class1>("key", [] { return TestMultiInheritClass3{}; })
                        .addKeyedScoped<TestMultiInheritClass3>("key", [] { return TestMultiInheritClass4{}; })
                        .addKeyedScoped<TestMultiInherit4Class4>("key", [] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider(options);

    auto service3 = provider.createKeyedService<TestMultiInheritClass2>("key");
    auto service4 = provider.createKeyedService<TestMultiInherit3Class1>("key");

    EXPECT_TRUE(service3);
    EXPECT_TRUE(service4);
    EXPECT_THROW(provider.createKeyedService<TestMultiInheritClass1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedService<TestMultiInherit2Class1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedService<TestMultiInheritClass3>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedService<TestMultiInherit4Class4>("key"), sb::di::ServiceNotFoundException);
}

TEST_F(MultiInheritanceFactoryKeyedTest, ShouldNotCreateServiceInPlace)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInheritClass1>("key", [] { return TestMultiInheritClass2{}; })
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass2{}; })
                        .addKeyedTransient<TestMultiInheritClass2>("key", [] { return TestMultiInheritClass3{}; })
                        .addKeyedTransient<TestMultiInherit3Class1>("key", [] { return TestMultiInheritClass3{}; })
                        .addKeyedScoped<TestMultiInheritClass3>("key", [] { return TestMultiInheritClass4{}; })
                        .addKeyedScoped<TestMultiInherit4Class4>("key", [] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider(options);

    EXPECT_THROW(provider.createKeyedServiceInPlace<TestMultiInheritClass2>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestMultiInherit3Class1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestMultiInheritClass1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestMultiInherit2Class1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestMultiInheritClass3>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestMultiInherit4Class4>("key"), sb::di::ServiceNotFoundException);
}

TEST_F(MultiInheritanceFactoryKeyedTest, ShouldCreateServices)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInheritClass1>("key", [] { return TestMultiInheritClass2{}; })
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass2{}; })
                        .addKeyedTransient<TestMultiInheritClass2>("key", [] { return TestMultiInheritClass3{}; })
                        .addKeyedTransient<TestMultiInherit3Class1>("key", [] { return TestMultiInheritClass3{}; })
                        .addKeyedScoped<TestMultiInheritClass3>("key", [] { return TestMultiInheritClass4{}; })
                        .addKeyedScoped<TestMultiInherit4Class4>("key", [] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider(options);

    EXPECT_TRUE(provider.createKeyedServices<TestMultiInheritClass1>("key").empty());
    EXPECT_TRUE(provider.createKeyedServices<TestMultiInherit2Class1>("key").empty());
    EXPECT_EQ(provider.createKeyedServices<TestMultiInheritClass2>("key").size(), 1);
    EXPECT_EQ(provider.createKeyedServices<TestMultiInherit3Class1>("key").size(), 1);
    EXPECT_TRUE(provider.createKeyedServices<TestMultiInheritClass3>("key").empty());
    EXPECT_TRUE(provider.createKeyedServices<TestMultiInherit4Class4>("key").empty());
}

TEST_F(MultiInheritanceFactoryKeyedTest, ShouldCreateServicesInOrder)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedTransient<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass2{}; })
                        .addKeyedTransient<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass3{}; })
                        .addKeyedTransient<TestMultiInherit2Class1>("key", [] { return TestMultiInheritClass4{}; })
                        .buildServiceProvider();

    const auto services = provider.createKeyedServices<TestMultiInherit2Class1>("key");

    EXPECT_EQ(services.size(), 3);
    EXPECT_EQ(services[0]->second(), 22);
    EXPECT_EQ(services[1]->second(), 33);
    EXPECT_EQ(services[2]->second(), 44);
}
