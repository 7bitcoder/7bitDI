#include <gtest/gtest.h>
#include <memory>

#include "../../Helpers/Classes/Basic.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceCollection.hpp"

class BasicFactoryKeyedTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    BasicFactoryKeyedTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~BasicFactoryKeyedTest() override = default;

    static void TearDownTestSuite() {}
};

// buildServiceProvider Tests

TEST_F(BasicFactoryKeyedTest, ShouldFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    collection.addKeyedSingleton("key", [] { return TestClass1{}; });
    collection.addKeyedSingleton("key", [] { return TestClass1{}; });
    collection.addKeyedSingleton("key", [] { return TestClass2{}; });

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = true;
    EXPECT_THROW(collection.buildServiceProvider(options), sb::di::ServiceAlreadyRegisteredException);
}

TEST_F(BasicFactoryKeyedTest, ShouldNotFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    collection.addKeyedSingleton("key", [] { return TestClass1{}; });
    collection.addKeyedSingleton("key", [] { return TestClass1{}; });
    collection.addKeyedSingleton("key", [] { return TestClass2{}; });

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    EXPECT_NO_THROW(collection.buildServiceProvider(options));
}

// tryGetService Tests

TEST_F(BasicFactoryKeyedTest, ShouldTryGetService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton("key", [] { return TestClass1{}; })
                        .addKeyedScoped("key", [] { return TestClass2{}; })
                        .addKeyedTransient("key", [] { return TestClass3{}; })
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetKeyedService<TestClass1>("key"));
    EXPECT_TRUE(provider.tryGetKeyedService<TestClass2>("key"));
    EXPECT_FALSE(provider.tryGetKeyedService<TestClass3>("key"));
    EXPECT_FALSE(provider.tryGetKeyedService<TestClass4>("key"));
}

TEST_F(BasicFactoryKeyedTest, ShouldGetService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton("key", [] { return TestClass1{}; })
                        .addKeyedScoped("key", [] { return TestClass2{}; })
                        .addKeyedTransient("key", [] { return TestClass3{}; })
                        .buildServiceProvider();

    EXPECT_NO_THROW(provider.getKeyedService<TestClass1>("key"));
    EXPECT_NO_THROW(provider.getKeyedService<TestClass2>("key"));
    EXPECT_THROW(provider.getKeyedService<TestClass3>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.getKeyedService<TestClass4>("key"), sb::di::ServiceNotFoundException);
}

TEST_F(BasicFactoryKeyedTest, ShouldGetServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton("key", [] { return TestClass1{}; })
                        .addKeyedScoped("key", [] { return TestClass2{}; })
                        .addKeyedTransient("key", [] { return TestClass3{}; })
                        .buildServiceProvider();

    EXPECT_EQ(provider.getKeyedServices<TestClass1>("key").size(), 1);
    EXPECT_EQ(provider.getKeyedServices<TestClass2>("key").size(), 1);
    EXPECT_EQ(provider.getKeyedServices<TestClass3>("key").size(), 0);
}

TEST_F(BasicFactoryKeyedTest, ShouldTryCreateService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton("key", [] { return TestClass1{}; })
                        .addKeyedScoped("key", [] { return TestClass2{}; })
                        .addKeyedTransient("key", [] { return TestClass3{}; })
                        .buildServiceProvider();

    EXPECT_FALSE(provider.tryCreateKeyedService<TestClass1>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestClass2>("key"));
    EXPECT_TRUE(provider.tryCreateKeyedService<TestClass3>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestClass4>("key"));
}

TEST_F(BasicFactoryKeyedTest, ShouldCreateService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton("key", [] { return TestClass1{}; })
                        .addKeyedScoped("key", [] { return TestClass2{}; })
                        .addKeyedTransient("key", [] { return TestClass3{}; })
                        .buildServiceProvider();

    EXPECT_THROW(provider.createKeyedService<TestClass1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedService<TestClass2>("key"), sb::di::ServiceNotFoundException);
    EXPECT_NO_THROW(provider.createKeyedService<TestClass3>("key"));
    EXPECT_THROW(provider.createKeyedService<TestClass4>("key"), sb::di::ServiceNotFoundException);
}

TEST_F(BasicFactoryKeyedTest, ShouldCreateServiceInPlace)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton("key", [] { return TestClass1{}; })
                        .addKeyedScoped("key", [] { return TestClass2{}; })
                        .addKeyedTransient("key", [] { return TestClass3{}; })
                        .buildServiceProvider();

    EXPECT_THROW(provider.createKeyedServiceInPlace<TestClass1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestClass2>("key"), sb::di::ServiceNotFoundException);
    EXPECT_NO_THROW(provider.createKeyedServiceInPlace<TestClass3>("key"));
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestClass4>("key"), sb::di::ServiceNotFoundException);
}

TEST_F(BasicFactoryKeyedTest, ShouldCreateServicess)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton("key", [] { return TestClass1{}; })
                        .addKeyedScoped("key", [] { return TestClass2{}; })
                        .addKeyedTransient("key", [] { return TestClass3{}; })
                        .buildServiceProvider();

    EXPECT_EQ(provider.createKeyedServices<TestClass1>("key").size(), 0);
    EXPECT_EQ(provider.createKeyedServices<TestClass2>("key").size(), 0);
    EXPECT_EQ(provider.createKeyedServices<TestClass3>("key").size(), 1);
}
