#include <gtest/gtest.h>
#include <memory>

#include "../../Helpers/Classes/Basic.hpp"
#include <SevenBit/DI/Exceptions.hpp>
#include <SevenBit/DI/ServiceCollection.hpp>

class BasicExternalKeyedTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    BasicExternalKeyedTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~BasicExternalKeyedTest() override = default;

    static void TearDownTestSuite() {}
};

// buildServiceProvider Tests

TEST_F(BasicExternalKeyedTest, ShouldFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    TestClass1 test1;
    TestClass2 test2;
    collection.addKeyedSingleton("key", &test1);
    collection.addKeyedSingleton("key", &test1);
    collection.addKeyedSingleton("key", &test2);

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = true;
    EXPECT_THROW(collection.buildServiceProvider(options), sb::di::ServiceRegisterException);
}

TEST_F(BasicExternalKeyedTest, ShouldNotFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    TestClass1 test1;
    TestClass2 test2;
    collection.addKeyedSingleton("key", &test1);
    collection.addKeyedSingleton("key", &test1);
    collection.addKeyedSingleton("key", &test2);

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    EXPECT_NO_THROW(collection.buildServiceProvider(options));
}

// tryGetService Tests

TEST_F(BasicExternalKeyedTest, ShouldTryGetService)
{
    TestClass1 test1;
    TestClass2 test2;
    TestClass3 test3;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton("key", &test1)
                        .addKeyedSingleton("key", &test2)
                        .addKeyedSingleton("key", &test3)
                        .buildServiceProvider();

    EXPECT_EQ(provider.tryGetKeyedService<TestClass1>("key"), &test1);
    EXPECT_EQ(provider.tryGetKeyedService<TestClass2>("key"), &test2);
    EXPECT_EQ(provider.tryGetKeyedService<TestClass3>("key"), &test3);
    EXPECT_FALSE(provider.tryGetKeyedService<TestClass4>("key"));
    EXPECT_FALSE(provider.tryGetKeyedService<TestClass1>("key2"));
}

// getService Tests

TEST_F(BasicExternalKeyedTest, ShouldGetService)
{
    TestClass1 test1;
    TestClass2 test2;
    TestClass3 test3;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton("key", &test1)
                        .addKeyedSingleton("key", &test2)
                        .addKeyedSingleton("key", &test3)
                        .buildServiceProvider();

    EXPECT_EQ(&provider.getKeyedService<TestClass1>("key"), &test1);
    EXPECT_EQ(&provider.getKeyedService<TestClass2>("key"), &test2);
    EXPECT_EQ(&provider.getKeyedService<TestClass3>("key"), &test3);
    EXPECT_THROW(provider.getKeyedService<TestClass4>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.getKeyedService<TestClass1>("key2"), sb::di::ServiceNotFoundException);
}

TEST_F(BasicExternalKeyedTest, ShouldGetServices)
{
    TestClass1 test1;
    TestClass2 test2;
    TestClass3 test3;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton("key", &test1)
                        .addKeyedSingleton("key", &test2)
                        .addKeyedSingleton("key", &test3)
                        .buildServiceProvider();

    EXPECT_EQ(provider.getKeyedServices<TestClass1>("key").size(), 1);
    EXPECT_EQ(provider.getKeyedServices<TestClass1>("key")[0], &test1);
    EXPECT_EQ(provider.getKeyedServices<TestClass2>("key").size(), 1);
    EXPECT_EQ(provider.getKeyedServices<TestClass2>("key")[0], &test2);
    EXPECT_EQ(provider.getKeyedServices<TestClass3>("key").size(), 1);
    EXPECT_EQ(provider.getKeyedServices<TestClass3>("key")[0], &test3);
}

// tryCreateService Tests

TEST_F(BasicExternalKeyedTest, ShouldTryCreateService)
{
    TestClass1 test1;
    TestClass2 test2;
    TestClass3 test3;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton("key", &test1)
                        .addKeyedSingleton("key", &test2)
                        .addKeyedSingleton("key", &test3)
                        .buildServiceProvider();

    EXPECT_FALSE(provider.tryCreateKeyedService<TestClass1>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestClass2>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestClass3>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestClass4>("key"));
}

// createService Tests

TEST_F(BasicExternalKeyedTest, ShouldCreateService)
{
    TestClass1 test1;
    TestClass2 test2;
    TestClass3 test3;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton("key", &test1)
                        .addKeyedSingleton("key", &test2)
                        .addKeyedSingleton("key", &test3)
                        .buildServiceProvider();

    EXPECT_THROW(provider.createKeyedService<TestClass1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedService<TestClass2>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedService<TestClass3>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedService<TestClass4>("key"), sb::di::ServiceNotFoundException);
}

// createServiceInPlace Tests

TEST_F(BasicExternalKeyedTest, ShouldCreateServiceInPlace)
{
    TestClass1 test1;
    TestClass2 test2;
    TestClass3 test3;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton("key", &test1)
                        .addKeyedSingleton("key", &test2)
                        .addKeyedSingleton("key", &test3)
                        .buildServiceProvider();

    EXPECT_THROW(provider.createKeyedServiceInPlace<TestClass1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestClass2>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestClass3>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestClass4>("key"), sb::di::ServiceNotFoundException);
}

// createServices Tests

TEST_F(BasicExternalKeyedTest, ShouldCreateServices)
{
    TestClass1 test1;
    TestClass2 test2;
    TestClass3 test3;
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton("key", &test1)
                        .addKeyedSingleton("key", &test2)
                        .addKeyedSingleton("key", &test3)
                        .buildServiceProvider();

    EXPECT_EQ(provider.createKeyedServices<TestClass1>("key").size(), 0);
    EXPECT_EQ(provider.createKeyedServices<TestClass2>("key").size(), 0);
    EXPECT_EQ(provider.createKeyedServices<TestClass3>("key").size(), 0);
}
