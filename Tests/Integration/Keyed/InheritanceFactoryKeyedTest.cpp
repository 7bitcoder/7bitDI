#include <gtest/gtest.h>
#include <memory>

#include "../../Helpers/Classes/Inherit.hpp"
#include <SevenBit/DI/Exceptions.hpp>
#include <SevenBit/DI/ServiceCollection.hpp>

class InheritanceFactoryKeyedTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    InheritanceFactoryKeyedTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~InheritanceFactoryKeyedTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(InheritanceFactoryKeyedTest, ShouldFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    collection.addKeyedSingleton<TestInheritClass1>("key", [] { return TestInheritClass2{}; });
    collection.addKeyedSingleton<TestInheritClass1>("key", [] { return TestInheritClass2{}; });
    collection.addKeyedSingleton<TestInheritClass1>("key", [] { return TestInheritClass4{}; });

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = true;
    EXPECT_THROW(collection.buildServiceProvider(options), sb::di::ServiceAlreadyRegisteredException);
}

TEST_F(InheritanceFactoryKeyedTest, ShouldNotFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    collection.addKeyedSingleton<TestInheritClass1>("key", [] { return TestInheritClass2{}; });
    collection.addKeyedSingleton<TestInheritClass1>("key", [] { return TestInheritClass2{}; });
    collection.addKeyedSingleton<TestInheritClass1>("key", [] { return TestInheritClass4{}; });

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    EXPECT_NO_THROW(collection.buildServiceProvider(options));
}

TEST_F(InheritanceFactoryKeyedTest, ShouldFailGetServiceDueToLifetimeMissmatchService)
{
    sb::di::ServiceCollection collection;

    collection.addKeyedSingleton<TestInheritClass1>("key", [] { return TestInheritClass3{}; });
    collection.addKeyedSingleton<TestInheritClass1>("key", [] { return TestInheritClass5{}; });
    collection.addKeyedTransient<TestInheritClass1>("key", [] { return TestInheritClass4{}; });

    EXPECT_THROW(collection.buildServiceProvider(), sb::di::ServiceLifeTimeMismatchException);
}

TEST_F(InheritanceFactoryKeyedTest, ShouldTryGetService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1>("key", [] { return TestInheritClass2{}; })
                        .addKeyedScoped<TestInheritClass2>("key", [] { return TestInheritClass3{}; })
                        .addKeyedTransient<TestInheritClass3>("key", [] { return TestInheritClass4{}; })
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetKeyedService<TestInheritClass1>("key"));
    EXPECT_TRUE(provider.tryGetKeyedService<TestInheritClass2>("key"));
    EXPECT_FALSE(provider.tryGetKeyedService<TestInheritClass3>("key"));
    EXPECT_FALSE(provider.tryGetKeyedService<TestInheritClass4>("key"));
}

TEST_F(InheritanceFactoryKeyedTest, ShouldTryGetOneLastServiceSingleton)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1>("key", [] { return TestInheritClass2{}; })
                        .addKeyedSingleton<TestInheritClass1>("key", [] { return TestInheritClass3{}; })
                        .addKeyedSingleton<TestInheritClass1>("key", [] { return TestInheritClass5{}; })
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetKeyedService<TestInheritClass1>("key"));
    EXPECT_EQ(provider.tryGetKeyedService<TestInheritClass1>("key")->number(), 5);
}

TEST_F(InheritanceFactoryKeyedTest, ShouldTryGetOneLastServiceScope)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedScoped<TestInheritClass1>("key", [] { return TestInheritClass2{}; })
                        .addKeyedScoped<TestInheritClass1>("key", [] { return TestInheritClass3{}; })
                        .addKeyedScoped<TestInheritClass1>("key", [] { return TestInheritClass5{}; })
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetKeyedService<TestInheritClass1>("key"));
    EXPECT_EQ(provider.tryGetKeyedService<TestInheritClass1>("key")->number(), 5);
}

TEST_F(InheritanceFactoryKeyedTest, ShouldGetService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1>("key", [] { return TestInheritClass2{}; })
                        .addKeyedScoped<TestInheritClass2>("key", [] { return TestInheritClass3{}; })
                        .addKeyedTransient<TestInheritClass3>("key", [] { return TestInheritClass4{}; })
                        .buildServiceProvider();

    EXPECT_NO_THROW(provider.getKeyedService<TestInheritClass1>("key"));
    EXPECT_NO_THROW(provider.getKeyedService<TestInheritClass2>("key"));
    EXPECT_THROW(provider.getKeyedService<TestInheritClass3>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.getKeyedService<TestInheritClass4>("key"), sb::di::ServiceNotFoundException);
}

TEST_F(InheritanceFactoryKeyedTest, ShouldGetOneLastServiceSingleton)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1>("key", [] { return TestInheritClass2{}; })
                        .addKeyedSingleton<TestInheritClass1>("key", [] { return TestInheritClass3{}; })
                        .addKeyedSingleton<TestInheritClass1>("key", [] { return TestInheritClass5{}; })
                        .buildServiceProvider();

    EXPECT_EQ(provider.getKeyedService<TestInheritClass1>("key").number(), 5);
}

TEST_F(InheritanceFactoryKeyedTest, ShouldGetOneLastServiceScope)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedScoped<TestInheritClass1>("key", [] { return TestInheritClass2{}; })
                        .addKeyedScoped<TestInheritClass1>("key", [] { return TestInheritClass3{}; })
                        .addKeyedScoped<TestInheritClass1>("key", [] { return TestInheritClass5{}; })
                        .buildServiceProvider();

    EXPECT_EQ(provider.getKeyedService<TestInheritClass1>("key").number(), 5);
}

TEST_F(InheritanceFactoryKeyedTest, ShouldGetServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1>("key", [] { return TestInheritClass2{}; })
                        .addKeyedScoped<TestInheritClass2>("key", [] { return TestInheritClass3{}; })
                        .addKeyedTransient<TestInheritClass3>("key", [] { return TestInheritClass4{}; })
                        .buildServiceProvider();

    EXPECT_EQ(provider.getKeyedServices<TestInheritClass1>("key").size(), 1);
    EXPECT_EQ(provider.getKeyedServices<TestInheritClass2>("key").size(), 1);
    EXPECT_TRUE(provider.getKeyedServices<TestInheritClass3>("key").empty());
    EXPECT_TRUE(provider.getKeyedServices<TestInheritClass4>("key").empty());
}

TEST_F(InheritanceFactoryKeyedTest, ShouldGetCastedServicesInOrder)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedScoped("key", [] { return TestInheritClass1{}; })
                        .addKeyedScoped<TestInheritClass1>("key", [] { return TestInheritClass2{}; })
                        .addKeyedScoped<TestInheritClass1>("key", [] { return TestInheritClass3{}; })
                        .addKeyedScoped<TestInheritClass1>("key", [] { return TestInheritClass4{}; })
                        .addKeyedScoped<TestInheritClass1>("key", [] { return TestInheritClass5{}; })
                        .buildServiceProvider();

    const auto services = provider.getKeyedServices<TestInheritClass1>("key");
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(services[0]->number(), 1);
    EXPECT_EQ(services[1]->number(), 2);
    EXPECT_EQ(services[2]->number(), 3);
    EXPECT_EQ(services[3]->number(), 4);
    EXPECT_EQ(services[4]->number(), 5);
}

TEST_F(InheritanceFactoryKeyedTest, ShouldGetServicesInOrderAfterNormalGet)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedScoped("key", [] { return TestInheritClass1{}; })
                        .addKeyedScoped<TestInheritClass1>("key", [] { return TestInheritClass2{}; })
                        .addKeyedScoped<TestInheritClass1>("key", [] { return TestInheritClass3{}; })
                        .addKeyedScoped<TestInheritClass1>("key", [] { return TestInheritClass4{}; })
                        .addKeyedScoped<TestInheritClass1>("key", [] { return TestInheritClass5{}; })
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetKeyedService<TestInheritClass1>("key"));
    const auto services = provider.getKeyedServices<TestInheritClass1>("key");
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(services[0]->number(), 1);
    EXPECT_EQ(services[1]->number(), 2);
    EXPECT_EQ(services[2]->number(), 3);
    EXPECT_EQ(services[3]->number(), 4);
    EXPECT_EQ(services[4]->number(), 5);
    EXPECT_TRUE(provider.tryGetKeyedService<TestInheritClass1>("key"));
}

TEST_F(InheritanceFactoryKeyedTest, ShouldTryCreateService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1>("key", [] { return TestInheritClass2{}; })
                        .addKeyedScoped<TestInheritClass2>("key", [] { return TestInheritClass3{}; })
                        .addKeyedTransient<TestInheritClass3>("key", [] { return TestInheritClass4{}; })
                        .buildServiceProvider();

    EXPECT_FALSE(provider.tryCreateKeyedService<TestInheritClass1>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestInheritClass2>("key"));
    EXPECT_TRUE(provider.tryCreateKeyedService<TestInheritClass3>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestInheritClass4>("key"));
}

TEST_F(InheritanceFactoryKeyedTest, ShouldCreateService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1>("key", [] { return TestInheritClass2{}; })
                        .addKeyedScoped<TestInheritClass2>("key", [] { return TestInheritClass3{}; })
                        .addKeyedTransient<TestInheritClass3>("key", [] { return TestInheritClass4{}; })
                        .buildServiceProvider();

    EXPECT_THROW(provider.createKeyedService<TestInheritClass1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedService<TestInheritClass2>("key"), sb::di::ServiceNotFoundException);
    EXPECT_NO_THROW(provider.createKeyedService<TestInheritClass3>("key"));
    EXPECT_THROW(provider.createKeyedService<TestInheritClass4>("key"), sb::di::ServiceNotFoundException);
}

TEST_F(InheritanceFactoryKeyedTest, ShouldCreateServiceInPlace)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1>("key", [] { return TestInheritClass2{}; })
                        .addKeyedScoped<TestInheritClass2>("key", [] { return TestInheritClass3{}; })
                        .addKeyedTransient<TestInheritClass3>("key", [] { return TestInheritClass4{}; })
                        .buildServiceProvider();

    EXPECT_THROW(provider.createKeyedServiceInPlace<TestInheritClass1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestInheritClass2>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestInheritClass3>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestInheritClass4>("key"), sb::di::ServiceNotFoundException);
}

TEST_F(InheritanceFactoryKeyedTest, ShouldCreateServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1>("key", [] { return TestInheritClass2{}; })
                        .addKeyedScoped<TestInheritClass2>("key", [] { return TestInheritClass3{}; })
                        .addKeyedTransient<TestInheritClass3>("key", [] { return TestInheritClass4{}; })
                        .buildServiceProvider();

    EXPECT_TRUE(provider.createKeyedServices<TestInheritClass1>("key").empty());
    EXPECT_TRUE(provider.createKeyedServices<TestInheritClass2>("key").empty());
    EXPECT_EQ(provider.createKeyedServices<TestInheritClass3>("key").size(), 1);
    EXPECT_TRUE(provider.createKeyedServices<TestInheritClass4>("key").empty());
}

TEST_F(InheritanceFactoryKeyedTest, ShouldCreateServicesInOrder)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedTransient("key", [] { return TestInheritClass1{}; })
                        .addKeyedTransient<TestInheritClass1>("key", [] { return TestInheritClass2{}; })
                        .addKeyedTransient<TestInheritClass1>("key", [] { return TestInheritClass3{}; })
                        .addKeyedTransient<TestInheritClass1>("key", [] { return TestInheritClass4{}; })
                        .addKeyedTransient<TestInheritClass1>("key", [] { return TestInheritClass5{}; })
                        .buildServiceProvider();

    const auto services = provider.createKeyedServices<TestInheritClass1>("key");
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(services[0]->number(), 1);
    EXPECT_EQ(services[1]->number(), 2);
    EXPECT_EQ(services[2]->number(), 3);
    EXPECT_EQ(services[3]->number(), 4);
    EXPECT_EQ(services[4]->number(), 5);
}
