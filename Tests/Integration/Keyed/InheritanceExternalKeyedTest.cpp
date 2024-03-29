#include <gtest/gtest.h>
#include <memory>

#include "../../Helpers/Classes/Inherit.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceCollection.hpp"

class InheritanceExternalKeyedTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    InheritanceExternalKeyedTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~InheritanceExternalKeyedTest() override = default;

    static void TearDownTestSuite() {}
};

// buildServiceProvider Tests

TEST_F(InheritanceExternalKeyedTest, ShouldFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    TestInheritClass2 test2;
    TestInheritClass4 test4;
    collection.addKeyedSingleton<TestInheritClass1>("key", &test2);
    collection.addKeyedSingleton<TestInheritClass1>("key", &test2);
    collection.addKeyedSingleton<TestInheritClass1>("key", &test4);

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = true;
    EXPECT_THROW(collection.buildServiceProvider(options), sb::di::ServiceAlreadyRegisteredException);
}

TEST_F(InheritanceExternalKeyedTest, ShouldNotFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    TestInheritClass2 test2;
    TestInheritClass4 test4;
    collection.addKeyedSingleton<TestInheritClass1>("key", &test2);
    collection.addKeyedSingleton<TestInheritClass1>("key", &test2);
    collection.addKeyedSingleton<TestInheritClass1>("key", &test4);

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    EXPECT_NO_THROW(collection.buildServiceProvider(options));
}

// tryGetService Tests

TEST_F(InheritanceExternalKeyedTest, ShouldTryGetService)
{
    TestInheritClass2 test2;
    TestInheritClass3 test3;
    TestInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1>("key", &test2)
                        .addKeyedSingleton<TestInheritClass2>("key", &test3)
                        .addKeyedSingleton<TestInheritClass3>("key", &test4)
                        .buildServiceProvider();

    EXPECT_EQ(provider.tryGetKeyedService<TestInheritClass1>("key"), &test2);
    EXPECT_EQ(provider.tryGetKeyedService<TestInheritClass2>("key"), &test3);
    EXPECT_EQ(provider.tryGetKeyedService<TestInheritClass3>("key"), &test4);
    EXPECT_FALSE(provider.tryGetKeyedService<TestInheritClass4>("key"));
}

TEST_F(InheritanceExternalKeyedTest, ShouldTryGetOneLastServiceSingleton)
{
    TestInheritClass2 test2;
    TestInheritClass3 test3;
    TestInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1>("key", &test2)
                        .addKeyedSingleton<TestInheritClass1>("key", &test3)
                        .addKeyedSingleton<TestInheritClass1>("key", &test4)
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetKeyedService<TestInheritClass1>("key"));
    EXPECT_EQ(provider.tryGetKeyedService<TestInheritClass1>("key"), &test4);
}

// getService Tests

TEST_F(InheritanceExternalKeyedTest, ShouldGetService)
{
    TestInheritClass2 test2;
    TestInheritClass3 test3;
    TestInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1>("key", &test2)
                        .addKeyedSingleton<TestInheritClass2>("key", &test3)
                        .addKeyedSingleton<TestInheritClass3>("key", &test4)
                        .buildServiceProvider();

    EXPECT_EQ(&provider.getKeyedService<TestInheritClass1>("key"), &test2);
    EXPECT_EQ(&provider.getKeyedService<TestInheritClass2>("key"), &test3);
    EXPECT_EQ(&provider.getKeyedService<TestInheritClass3>("key"), &test4);
    EXPECT_THROW(provider.getKeyedService<TestInheritClass4>("key"), sb::di::ServiceNotFoundException);
}

TEST_F(InheritanceExternalKeyedTest, ShouldGetOneLastServiceSingleton)
{
    TestInheritClass2 test2;
    TestInheritClass3 test3;
    TestInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1>("key", &test2)
                        .addKeyedSingleton<TestInheritClass1>("key", &test3)
                        .addKeyedSingleton<TestInheritClass1>("key", &test4)
                        .buildServiceProvider();

    EXPECT_EQ(&provider.getKeyedService<TestInheritClass1>("key"), &test4);
}

TEST_F(InheritanceExternalKeyedTest, ShouldGetServices)
{
    TestInheritClass2 test2;
    TestInheritClass3 test3;
    TestInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1>("key", &test2)
                        .addKeyedSingleton<TestInheritClass2>("key", &test3)
                        .addKeyedSingleton<TestInheritClass3>("key", &test4)
                        .buildServiceProvider();

    EXPECT_EQ(provider.getKeyedServices<TestInheritClass1>("key").size(), 1);
    EXPECT_EQ(provider.getKeyedServices<TestInheritClass1>("key")[0], &test2);
    EXPECT_EQ(provider.getKeyedServices<TestInheritClass2>("key").size(), 1);
    EXPECT_EQ(provider.getKeyedServices<TestInheritClass2>("key")[0], &test3);
    EXPECT_EQ(provider.getKeyedServices<TestInheritClass3>("key").size(), 1);
    EXPECT_EQ(provider.getKeyedServices<TestInheritClass3>("key")[0], &test4);

    EXPECT_TRUE(provider.getServices<TestInheritClass4>().empty());
}

TEST_F(InheritanceExternalKeyedTest, ShouldGetCastedServicesInOrder)
{
    TestInheritClass1 test1;
    TestInheritClass2 test2;
    TestInheritClass3 test3;
    TestInheritClass4 test4;
    TestInheritClass5 test5;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton("key", &test1)
                        .addKeyedSingleton<TestInheritClass1>("key", &test2)
                        .addKeyedSingleton<TestInheritClass1>("key", &test3)
                        .addKeyedSingleton<TestInheritClass1>("key", &test4)
                        .addKeyedSingleton<TestInheritClass1>("key", &test5)
                        .buildServiceProvider();

    const auto services = provider.getKeyedServices<TestInheritClass1>("key");
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(services[0], &test1);
    EXPECT_EQ(services[1], &test2);
    EXPECT_EQ(services[2], &test3);
    EXPECT_EQ(services[3], &test4);
    EXPECT_EQ(services[4], &test5);
}

TEST_F(InheritanceExternalKeyedTest, ShouldGetServicesInOrderAfterNormalGet)
{
    TestInheritClass1 test1;
    TestInheritClass2 test2;
    TestInheritClass3 test3;
    TestInheritClass4 test4;
    TestInheritClass5 test5;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton("key", &test1)
                        .addKeyedSingleton<TestInheritClass1>("key", &test2)
                        .addKeyedSingleton<TestInheritClass1>("key", &test3)
                        .addKeyedSingleton<TestInheritClass1>("key", &test4)
                        .addKeyedSingleton<TestInheritClass1>("key", &test5)
                        .buildServiceProvider();

    EXPECT_EQ(provider.tryGetKeyedService<TestInheritClass1>("key"), &test5);
    const auto services = provider.getKeyedServices<TestInheritClass1>("key");
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(services[0], &test1);
    EXPECT_EQ(services[1], &test2);
    EXPECT_EQ(services[2], &test3);
    EXPECT_EQ(services[3], &test4);
    EXPECT_EQ(services[4], &test5);
}

TEST_F(InheritanceExternalKeyedTest, ShouldTryCreateService)
{
    TestInheritClass2 test2;
    TestInheritClass3 test3;
    TestInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1>("key", &test2)
                        .addKeyedSingleton<TestInheritClass2>("key", &test3)
                        .addKeyedSingleton<TestInheritClass3>("key", &test4)
                        .buildServiceProvider();

    EXPECT_FALSE(provider.tryCreateKeyedService<TestInheritClass1>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestInheritClass2>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestInheritClass3>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestInheritClass4>("key"));
}

TEST_F(InheritanceExternalKeyedTest, ShouldCreateService)
{
    TestInheritClass2 test2;
    TestInheritClass3 test3;
    TestInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1>("key", &test2)
                        .addKeyedSingleton<TestInheritClass2>("key", &test3)
                        .addKeyedSingleton<TestInheritClass3>("key", &test4)
                        .buildServiceProvider();

    EXPECT_THROW(provider.createKeyedService<TestInheritClass1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedService<TestInheritClass2>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedService<TestInheritClass3>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedService<TestInheritClass4>("key"), sb::di::ServiceNotFoundException);
}

TEST_F(InheritanceExternalKeyedTest, ShouldCreateServiceInPlace)
{
    TestInheritClass2 test2;
    TestInheritClass3 test3;
    TestInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1>("key", &test2)
                        .addKeyedSingleton<TestInheritClass2>("key", &test3)
                        .addKeyedSingleton<TestInheritClass3>("key", &test4)
                        .buildServiceProvider();

    EXPECT_THROW(provider.createKeyedServiceInPlace<TestInheritClass1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestInheritClass2>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestInheritClass3>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestInheritClass4>("key"), sb::di::ServiceNotFoundException);
}

TEST_F(InheritanceExternalKeyedTest, ShouldCreateServices)
{
    TestInheritClass2 test2;
    TestInheritClass3 test3;
    TestInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestInheritClass1>("key", &test2)
                        .addKeyedSingleton<TestInheritClass2>("key", &test3)
                        .addKeyedSingleton<TestInheritClass3>("key", &test4)
                        .buildServiceProvider();

    EXPECT_TRUE(provider.createKeyedServices<TestInheritClass1>("key").empty());
    EXPECT_TRUE(provider.createKeyedServices<TestInheritClass2>("key").empty());
    EXPECT_TRUE(provider.createKeyedServices<TestInheritClass3>("key").empty());
    EXPECT_TRUE(provider.createKeyedServices<TestInheritClass4>("key").empty());
}

TEST_F(InheritanceExternalKeyedTest, ShouldCreateServicesInOrder)
{
    TestInheritClass1 test1;
    TestInheritClass2 test2;
    TestInheritClass3 test3;
    TestInheritClass4 test4;
    TestInheritClass5 test5;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton("key", &test1)
                        .addKeyedSingleton<TestInheritClass1>("key", &test2)
                        .addKeyedSingleton<TestInheritClass1>("key", &test3)
                        .addKeyedSingleton<TestInheritClass1>("key", &test4)
                        .addKeyedSingleton<TestInheritClass1>("key", &test5)
                        .buildServiceProvider();

    const auto services = provider.createKeyedServices<TestInheritClass1>("key");
    EXPECT_TRUE(services.empty());
}
