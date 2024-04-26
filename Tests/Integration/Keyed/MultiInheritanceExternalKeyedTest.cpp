#include <gtest/gtest.h>
#include <memory>

#include "../../Helpers/Classes/MultiInherit.hpp"
#include <SevenBit/DI/ServiceCollection.hpp>

class MultiInheritanceExternalKeyedTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    MultiInheritanceExternalKeyedTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~MultiInheritanceExternalKeyedTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(MultiInheritanceExternalKeyedTest, ShouldTryGetService)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    TestMultiInheritClass2 test2;
    TestMultiInheritClass3 test3;
    TestMultiInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInheritClass1>("key", &test2)
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", &test2)
                        .addKeyedSingleton<TestMultiInheritClass2>("key", &test3)
                        .addKeyedSingleton<TestMultiInherit3Class1>("key", &test3)
                        .addKeyedSingleton<TestMultiInheritClass3>("key", &test4)
                        .addKeyedSingleton<TestMultiInherit4Class4>("key", &test4)
                        .buildServiceProvider(options);

    EXPECT_EQ(provider.tryGetKeyedService<TestMultiInheritClass1>("key"), &test2);
    EXPECT_EQ(provider.tryGetKeyedService<TestMultiInherit2Class1>("key"), &test2);
    EXPECT_EQ(provider.tryGetKeyedService<TestMultiInheritClass2>("key"), &test3);
    EXPECT_EQ(provider.tryGetKeyedService<TestMultiInherit3Class1>("key"), &test3);
    EXPECT_EQ(provider.tryGetKeyedService<TestMultiInheritClass3>("key"), &test4);
    EXPECT_EQ(provider.tryGetKeyedService<TestMultiInherit4Class4>("key"), &test4);
}

TEST_F(MultiInheritanceExternalKeyedTest, ShouldTryGetOneLastServiceSingleton)
{
    TestMultiInheritClass2 test2;
    TestMultiInheritClass3 test3;
    TestMultiInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", &test2)
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", &test3)
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", &test4)
                        .buildServiceProvider();

    const auto service = provider.tryGetKeyedService<TestMultiInherit2Class1>("key");

    EXPECT_TRUE(service);
    EXPECT_EQ(service, &test4);
}

TEST_F(MultiInheritanceExternalKeyedTest, ShouldGetService)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    TestMultiInheritClass2 test2;
    TestMultiInheritClass3 test3;
    TestMultiInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInheritClass1>("key", &test2)
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", &test2)
                        .addKeyedSingleton<TestMultiInheritClass2>("key", &test3)
                        .addKeyedSingleton<TestMultiInherit3Class1>("key", &test3)
                        .addKeyedSingleton<TestMultiInheritClass3>("key", &test4)
                        .addKeyedSingleton<TestMultiInherit4Class4>("key", &test4)
                        .buildServiceProvider(options);

    EXPECT_EQ(&provider.getKeyedService<TestMultiInheritClass1>("key"), &test2);
    EXPECT_EQ(&provider.getKeyedService<TestMultiInherit2Class1>("key"), &test2);
    EXPECT_EQ(&provider.getKeyedService<TestMultiInheritClass2>("key"), &test3);
    EXPECT_EQ(&provider.getKeyedService<TestMultiInherit3Class1>("key"), &test3);
    EXPECT_EQ(&provider.getKeyedService<TestMultiInheritClass3>("key"), &test4);
    EXPECT_EQ(&provider.getKeyedService<TestMultiInherit4Class4>("key"), &test4);
}

TEST_F(MultiInheritanceExternalKeyedTest, ShouldGetOneLastServiceSingleton)
{
    TestMultiInheritClass2 test2;
    TestMultiInheritClass3 test3;
    TestMultiInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", &test2)
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", &test3)
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", &test4)
                        .buildServiceProvider();

    EXPECT_EQ(&provider.getKeyedService<TestMultiInherit2Class1>("key"), &test4);
}

TEST_F(MultiInheritanceExternalKeyedTest, ShouldGetServices)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    TestMultiInheritClass2 test2;
    TestMultiInheritClass3 test3;
    TestMultiInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInheritClass1>("key", &test2)
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", &test2)
                        .addKeyedSingleton<TestMultiInheritClass2>("key", &test3)
                        .addKeyedSingleton<TestMultiInherit3Class1>("key", &test3)
                        .addKeyedSingleton<TestMultiInheritClass3>("key", &test4)
                        .addKeyedSingleton<TestMultiInherit4Class4>("key", &test4)
                        .buildServiceProvider(options);

    EXPECT_EQ(provider.getKeyedServices<TestMultiInheritClass1>("key").size(), 1);
    EXPECT_EQ(provider.getKeyedServices<TestMultiInheritClass1>("key")[0], &test2);
    EXPECT_EQ(provider.getKeyedServices<TestMultiInherit2Class1>("key").size(), 1);
    EXPECT_EQ(provider.getKeyedServices<TestMultiInherit2Class1>("key")[0], &test2);
    EXPECT_EQ(provider.getKeyedServices<TestMultiInheritClass2>("key").size(), 1);
    EXPECT_EQ(provider.getKeyedServices<TestMultiInheritClass2>("key")[0], &test3);
    EXPECT_EQ(provider.getKeyedServices<TestMultiInherit3Class1>("key").size(), 1);
    EXPECT_EQ(provider.getKeyedServices<TestMultiInherit3Class1>("key")[0], &test3);
    EXPECT_EQ(provider.getKeyedServices<TestMultiInheritClass3>("key").size(), 1);
    EXPECT_EQ(provider.getKeyedServices<TestMultiInheritClass3>("key")[0], &test4);
    EXPECT_EQ(provider.getKeyedServices<TestMultiInherit4Class4>("key").size(), 1);
    EXPECT_EQ(provider.getKeyedServices<TestMultiInherit4Class4>("key")[0], &test4);
}

TEST_F(MultiInheritanceExternalKeyedTest, ShouldGetCastedServicesInOrder)
{
    TestMultiInheritClass2 test2;
    TestMultiInheritClass3 test3;
    TestMultiInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", &test2)
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", &test3)
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", &test4)
                        .buildServiceProvider();

    const auto services = provider.getKeyedServices<TestMultiInherit2Class1>("key");

    EXPECT_EQ(services.size(), 3);
    EXPECT_EQ(services[0], &test2);
    EXPECT_EQ(services[1], &test3);
    EXPECT_EQ(services[2], &test4);
}

TEST_F(MultiInheritanceExternalKeyedTest, ShouldGetServicesInOrderAfterNormalGet)
{
    TestMultiInheritClass2 test2;
    TestMultiInheritClass3 test3;
    TestMultiInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", &test2)
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", &test3)
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", &test4)
                        .buildServiceProvider();

    EXPECT_EQ(provider.tryGetKeyedService<TestMultiInherit2Class1>("key"), &test4);
    const auto services = provider.getKeyedServices<TestMultiInherit2Class1>("key");
    EXPECT_EQ(services.size(), 3);
    EXPECT_EQ(services[0], &test2);
    EXPECT_EQ(services[1], &test3);
    EXPECT_EQ(services[2], &test4);
}

TEST_F(MultiInheritanceExternalKeyedTest, ShouldTryCreateService)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    TestMultiInheritClass2 test2;
    TestMultiInheritClass3 test3;
    TestMultiInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInheritClass1>("key", &test2)
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", &test2)
                        .addKeyedSingleton<TestMultiInheritClass2>("key", &test3)
                        .addKeyedSingleton<TestMultiInherit3Class1>("key", &test3)
                        .addKeyedSingleton<TestMultiInheritClass3>("key", &test4)
                        .addKeyedSingleton<TestMultiInherit4Class4>("key", &test4)
                        .buildServiceProvider(options);

    EXPECT_FALSE(provider.tryCreateKeyedService<TestMultiInheritClass2>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestMultiInherit3Class1>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestMultiInheritClass1>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestMultiInherit2Class1>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestMultiInheritClass3>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestMultiInherit4Class4>("key"));
}

TEST_F(MultiInheritanceExternalKeyedTest, ShouldCreateService)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    TestMultiInheritClass2 test2;
    TestMultiInheritClass3 test3;
    TestMultiInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInheritClass1>("key", &test2)
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", &test2)
                        .addKeyedSingleton<TestMultiInheritClass2>("key", &test3)
                        .addKeyedSingleton<TestMultiInherit3Class1>("key", &test3)
                        .addKeyedSingleton<TestMultiInheritClass3>("key", &test4)
                        .addKeyedSingleton<TestMultiInherit4Class4>("key", &test4)
                        .buildServiceProvider(options);

    EXPECT_THROW(provider.createKeyedService<TestMultiInheritClass2>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedService<TestMultiInherit3Class1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedService<TestMultiInheritClass1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedService<TestMultiInherit2Class1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedService<TestMultiInheritClass3>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedService<TestMultiInherit4Class4>("key"), sb::di::ServiceNotFoundException);
}

TEST_F(MultiInheritanceExternalKeyedTest, ShouldNotCreateServiceInPlace)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    TestMultiInheritClass2 test2;
    TestMultiInheritClass3 test3;
    TestMultiInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInheritClass1>("key", &test2)
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", &test2)
                        .addKeyedSingleton<TestMultiInheritClass2>("key", &test3)
                        .addKeyedSingleton<TestMultiInherit3Class1>("key", &test3)
                        .addKeyedSingleton<TestMultiInheritClass3>("key", &test4)
                        .addKeyedSingleton<TestMultiInherit4Class4>("key", &test4)
                        .buildServiceProvider(options);

    EXPECT_THROW(provider.createKeyedServiceInPlace<TestMultiInheritClass2>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestMultiInherit3Class1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestMultiInheritClass1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestMultiInherit2Class1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestMultiInheritClass3>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestMultiInherit4Class4>("key"), sb::di::ServiceNotFoundException);
}

TEST_F(MultiInheritanceExternalKeyedTest, ShouldCreateServices)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    TestMultiInheritClass2 test2;
    TestMultiInheritClass3 test3;
    TestMultiInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInheritClass1>("key", &test2)
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", &test2)
                        .addKeyedSingleton<TestMultiInheritClass2>("key", &test3)
                        .addKeyedSingleton<TestMultiInherit3Class1>("key", &test3)
                        .addKeyedSingleton<TestMultiInheritClass3>("key", &test4)
                        .addKeyedSingleton<TestMultiInherit4Class4>("key", &test4)
                        .buildServiceProvider(options);

    EXPECT_TRUE(provider.createKeyedServices<TestMultiInheritClass1>("key").empty());
    EXPECT_TRUE(provider.createKeyedServices<TestMultiInherit2Class1>("key").empty());
    EXPECT_TRUE(provider.createKeyedServices<TestMultiInheritClass2>("key").empty());
    EXPECT_TRUE(provider.createKeyedServices<TestMultiInherit3Class1>("key").empty());
    EXPECT_TRUE(provider.createKeyedServices<TestMultiInheritClass3>("key").empty());
    EXPECT_TRUE(provider.createKeyedServices<TestMultiInherit4Class4>("key").empty());
}

TEST_F(MultiInheritanceExternalKeyedTest, ShouldCreateServicesInOrder)
{
    TestMultiInheritClass2 test2;
    TestMultiInheritClass3 test3;
    TestMultiInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", &test2)
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", &test3)
                        .addKeyedSingleton<TestMultiInherit2Class1>("key", &test4)
                        .buildServiceProvider();

    EXPECT_TRUE(provider.createKeyedServices<TestMultiInherit2Class1>("key").empty());
}
