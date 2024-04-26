#include <gtest/gtest.h>
#include <memory>

#include "../../Helpers/Classes/Inherit.hpp"
#include <SevenBit/DI/Exceptions.hpp>
#include <SevenBit/DI/ServiceCollection.hpp>

class InheritanceKeyedTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    InheritanceKeyedTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~InheritanceKeyedTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(InheritanceKeyedTest, ShouldFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    collection.addKeyedSingleton<TestInheritClass1, TestInheritClass2>("key");
    collection.addKeyedSingleton<TestInheritClass1, TestInheritClass2>("key");
    collection.addKeyedSingleton<TestInheritClass1, TestInheritClass4>("key");

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = true;
    EXPECT_THROW(collection.buildServiceProvider(options), sb::di::ServiceAlreadyRegisteredException);
}

TEST_F(InheritanceKeyedTest, ShouldNotFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    collection.addKeyedSingleton<TestInheritClass1, TestInheritClass2>("key");
    collection.addKeyedSingleton<TestInheritClass1, TestInheritClass2>("key");
    collection.addKeyedSingleton<TestInheritClass1, TestInheritClass4>("key");

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    EXPECT_NO_THROW(collection.buildServiceProvider(options));
}

TEST_F(InheritanceKeyedTest, ShouldFailGetServiceDueToLifetimeMissmatchService)
{
    sb::di::ServiceCollection collection;

    collection.addKeyedSingleton<TestInheritClass1, TestInheritClass3>("key");
    collection.addKeyedSingleton<TestInheritClass1, TestInheritClass5>("key");
    collection.addKeyedTransient<TestInheritClass1, TestInheritClass4>("key");

    EXPECT_THROW(collection.buildServiceProvider(), sb::di::ServiceLifeTimeMismatchException);
}

TEST_F(InheritanceKeyedTest, ShouldTryGetService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1, TestInheritClass2>("key")
                        .addKeyedScoped<TestInheritClass2, TestInheritClass3>("key")
                        .addKeyedTransient<TestInheritClass3, TestInheritClass4>("key")
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetKeyedService<TestInheritClass1>("key"));
    EXPECT_TRUE(provider.tryGetKeyedService<TestInheritClass2>("key"));
    EXPECT_FALSE(provider.tryGetKeyedService<TestInheritClass3>("key"));
    EXPECT_FALSE(provider.tryGetKeyedService<TestInheritClass4>("key"));
}

TEST_F(InheritanceKeyedTest, ShouldTryGetOneLastServiceSingleton)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1, TestInheritClass2>("key")
                        .addKeyedSingleton<TestInheritClass1, TestInheritClass3>("key")
                        .addKeyedSingleton<TestInheritClass1, TestInheritClass5>("key")
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetKeyedService<TestInheritClass1>("key"));
    EXPECT_EQ(provider.tryGetKeyedService<TestInheritClass1>("key")->number(), 5);
}

TEST_F(InheritanceKeyedTest, ShouldTryGetOneLastServiceScope)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedScoped<TestInheritClass1, TestInheritClass2>("key")
                        .addKeyedScoped<TestInheritClass1, TestInheritClass3>("key")
                        .addKeyedScoped<TestInheritClass1, TestInheritClass5>("key")
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetKeyedService<TestInheritClass1>("key"));
    EXPECT_EQ(provider.tryGetKeyedService<TestInheritClass1>("key")->number(), 5);
}

TEST_F(InheritanceKeyedTest, ShouldGetService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1, TestInheritClass2>("key")
                        .addKeyedScoped<TestInheritClass2, TestInheritClass3>("key")
                        .addKeyedTransient<TestInheritClass3, TestInheritClass4>("key")
                        .buildServiceProvider();

    EXPECT_NO_THROW(provider.getKeyedService<TestInheritClass1>("key"));
    EXPECT_NO_THROW(provider.getKeyedService<TestInheritClass2>("key"));
    EXPECT_THROW(provider.getKeyedService<TestInheritClass3>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.getKeyedService<TestInheritClass4>("key"), sb::di::ServiceNotFoundException);
}

TEST_F(InheritanceKeyedTest, ShouldGetOneLastServiceSingleton)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1, TestInheritClass2>("key")
                        .addKeyedSingleton<TestInheritClass1, TestInheritClass3>("key")
                        .addKeyedSingleton<TestInheritClass1, TestInheritClass5>("key")
                        .buildServiceProvider();

    EXPECT_EQ(provider.getKeyedService<TestInheritClass1>("key").number(), 5);
}

TEST_F(InheritanceKeyedTest, ShouldGetOneLastServiceScope)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedScoped<TestInheritClass1, TestInheritClass2>("key")
                        .addKeyedScoped<TestInheritClass1, TestInheritClass3>("key")
                        .addKeyedScoped<TestInheritClass1, TestInheritClass5>("key")
                        .buildServiceProvider();

    EXPECT_EQ(provider.getKeyedService<TestInheritClass1>("key").number(), 5);
}

TEST_F(InheritanceKeyedTest, ShouldGetServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1, TestInheritClass2>("key")
                        .addKeyedScoped<TestInheritClass2, TestInheritClass3>("key")
                        .addKeyedTransient<TestInheritClass3, TestInheritClass4>("key")
                        .buildServiceProvider();

    EXPECT_EQ(provider.getKeyedServices<TestInheritClass1>("key").size(), 1);
    EXPECT_EQ(provider.getKeyedServices<TestInheritClass2>("key").size(), 1);
    EXPECT_TRUE(provider.getKeyedServices<TestInheritClass3>("key").empty());
    EXPECT_TRUE(provider.getKeyedServices<TestInheritClass4>("key").empty());
}

TEST_F(InheritanceKeyedTest, ShouldGetCastedServicesInOrder)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedScoped<TestInheritClass1>("key")
                        .addKeyedScoped<TestInheritClass1, TestInheritClass2>("key")
                        .addKeyedScoped<TestInheritClass1, TestInheritClass3>("key")
                        .addKeyedScoped<TestInheritClass1, TestInheritClass4>("key")
                        .addKeyedScoped<TestInheritClass1, TestInheritClass5>("key")
                        .buildServiceProvider();

    const auto services = provider.getKeyedServices<TestInheritClass1>("key");
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(services[0]->number(), 1);
    EXPECT_EQ(services[1]->number(), 2);
    EXPECT_EQ(services[2]->number(), 3);
    EXPECT_EQ(services[3]->number(), 4);
    EXPECT_EQ(services[4]->number(), 5);
}

TEST_F(InheritanceKeyedTest, ShouldGetServicesInOrderAfterNormalGet)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedScoped<TestInheritClass1>("key")
                        .addKeyedScoped<TestInheritClass1, TestInheritClass2>("key")
                        .addKeyedScoped<TestInheritClass1, TestInheritClass3>("key")
                        .addKeyedScoped<TestInheritClass1, TestInheritClass4>("key")
                        .addKeyedScoped<TestInheritClass1, TestInheritClass5>("key")
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

TEST_F(InheritanceKeyedTest, ShouldTryCreateService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1, TestInheritClass2>("key")
                        .addKeyedScoped<TestInheritClass2, TestInheritClass3>("key")
                        .addKeyedTransient<TestInheritClass3, TestInheritClass4>("key")
                        .buildServiceProvider();

    EXPECT_FALSE(provider.tryCreateKeyedService<TestInheritClass1>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestInheritClass2>("key"));
    EXPECT_TRUE(provider.tryCreateKeyedService<TestInheritClass3>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestInheritClass4>("key"));
}

TEST_F(InheritanceKeyedTest, ShouldCreateService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1, TestInheritClass2>("key")
                        .addKeyedScoped<TestInheritClass2, TestInheritClass3>("key")
                        .addKeyedTransient<TestInheritClass3, TestInheritClass4>("key")
                        .buildServiceProvider();

    EXPECT_THROW(provider.createKeyedService<TestInheritClass1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedService<TestInheritClass2>("key"), sb::di::ServiceNotFoundException);
    EXPECT_NO_THROW(provider.createKeyedService<TestInheritClass3>("key"));
    EXPECT_THROW(provider.createKeyedService<TestInheritClass4>("key"), sb::di::ServiceNotFoundException);
}

TEST_F(InheritanceKeyedTest, ShouldCreateServiceInPlace)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1, TestInheritClass2>("key")
                        .addKeyedScoped<TestInheritClass2, TestInheritClass3>("key")
                        .addKeyedTransient<TestInheritClass3, TestInheritClass4>("key")
                        .buildServiceProvider();

    EXPECT_THROW(provider.createKeyedServiceInPlace<TestInheritClass1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestInheritClass2>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestInheritClass3>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestInheritClass4>("key"), sb::di::ServiceNotFoundException);
}

TEST_F(InheritanceKeyedTest, ShouldCreateServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1, TestInheritClass2>("key")
                        .addKeyedScoped<TestInheritClass2, TestInheritClass3>("key")
                        .addKeyedTransient<TestInheritClass3, TestInheritClass4>("key")
                        .buildServiceProvider();

    EXPECT_TRUE(provider.createKeyedServices<TestInheritClass1>("key").empty());
    EXPECT_TRUE(provider.createKeyedServices<TestInheritClass2>("key").empty());
    EXPECT_EQ(provider.createKeyedServices<TestInheritClass3>("key").size(), 1);
    EXPECT_TRUE(provider.createKeyedServices<TestInheritClass4>("key").empty());
}

TEST_F(InheritanceKeyedTest, ShouldCreateServicesInOrder)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedTransient<TestInheritClass1>("key")
                        .addKeyedTransient<TestInheritClass1, TestInheritClass2>("key")
                        .addKeyedTransient<TestInheritClass1, TestInheritClass3>("key")
                        .addKeyedTransient<TestInheritClass1, TestInheritClass4>("key")
                        .addKeyedTransient<TestInheritClass1, TestInheritClass5>("key")
                        .buildServiceProvider();

    const auto services = provider.createKeyedServices<TestInheritClass1>("key");
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(services[0]->number(), 1);
    EXPECT_EQ(services[1]->number(), 2);
    EXPECT_EQ(services[2]->number(), 3);
    EXPECT_EQ(services[3]->number(), 4);
    EXPECT_EQ(services[4]->number(), 5);
}
