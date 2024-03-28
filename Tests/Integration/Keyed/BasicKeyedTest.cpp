#include <gtest/gtest.h>
#include <memory>

#include "../../Helpers/Classes/Basic.hpp"
#include "../../Helpers/Classes/CirularDependency.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceCollection.hpp"

class BasicKeyedTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    BasicKeyedTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~BasicKeyedTest() override = default;

    static void TearDownTestSuite() {}
};

// buildServiceProvider Tests

TEST_F(BasicKeyedTest, ShouldFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    collection.addKeyedSingleton<TestClass1>("keySingleton");
    collection.addKeyedSingleton<TestClass1>("keySingleton");
    collection.addKeyedSingleton<TestClass2>("keySingleton");

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = true;
    EXPECT_THROW(collection.buildServiceProvider(options), sb::di::ServiceAlreadyRegisteredException);
}

TEST_F(BasicKeyedTest, ShouldNotFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    collection.addKeyedSingleton<TestClass1>("keySingleton");
    collection.addKeyedSingleton<TestClass1>("keySingleton");
    collection.addKeyedSingleton<TestClass2>("keySingleton");

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    EXPECT_NO_THROW(collection.buildServiceProvider(options));
}

// tryGetService Tests

TEST_F(BasicKeyedTest, ShouldTryGetService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestClass1>("keySingleton")
                        .addKeyedScoped<TestClass2>("keyScoped")
                        .addKeyedTransient<TestClass3>("keyTransient")
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetKeyedService<TestClass1>("keySingleton"));
    EXPECT_TRUE(provider.tryGetKeyedService<TestClass2>("keyScoped"));
    EXPECT_FALSE(provider.tryGetKeyedService<TestClass3>("keyTransient"));
    EXPECT_FALSE(provider.tryGetKeyedService<TestClass4>("keySingleton"));
    EXPECT_FALSE(provider.tryGetKeyedService<TestClass1>("keySingleton2"));
}

TEST_F(BasicKeyedTest, ShouldFailTryGetServiceDueToCircularDependency)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<CircularDependencyA>("keySingleton")
                        .addSingleton<CircularDependencyA>()
                        .addKeyedScoped<CircularDependencyB>("keyScoped")
                        .addScoped<CircularDependencyB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider.tryGetKeyedService<CircularDependencyA>("keySingleton"), sb::di::CircularDependencyException);
}

TEST_F(BasicKeyedTest, ShouldGetService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestClass1>("keySingleton")
                        .addKeyedScoped<TestClass2>("keyScoped")
                        .addKeyedTransient<TestClass3>("keyTransient")
                        .buildServiceProvider();

    EXPECT_NO_THROW(provider.getKeyedService<TestClass1>("keySingleton"));
    EXPECT_NO_THROW(provider.getKeyedService<TestClass2>("keyScoped"));
    EXPECT_THROW(provider.getKeyedService<TestClass3>("keyTransient"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.getKeyedService<TestClass4>("keySingleton"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.getKeyedService<TestClass1>("keySingleton2"), sb::di::ServiceNotFoundException);
}

TEST_F(BasicKeyedTest, ShouldGetServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestClass1>("keySingleton")
                        .addKeyedScoped<TestClass2>("keyScoped")
                        .addKeyedTransient<TestClass3>("keyTransient")
                        .buildServiceProvider();

    EXPECT_EQ(provider.getKeyedServices<TestClass1>("keySingleton").size(), 1);
    EXPECT_EQ(provider.getKeyedServices<TestClass2>("keyScoped").size(), 1);
    EXPECT_EQ(provider.getKeyedServices<TestClass3>("keyTransient").size(), 0);
    EXPECT_EQ(provider.getKeyedServices<TestClass1>("keySingleton2").size(), 0);
}

TEST_F(BasicKeyedTest, ShouldGetEmptyServicesForNotExisting)
{
    auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    const auto all = provider.getKeyedServices<TestClass1>("key");
    EXPECT_TRUE(all.empty());
}

TEST_F(BasicKeyedTest, ShouldTryCreateService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestClass1>("keySingleton")
                        .addKeyedScoped<TestClass2>("keyScoped")
                        .addKeyedTransient<TestClass3>("keyTransient")
                        .buildServiceProvider();

    EXPECT_FALSE(provider.tryCreateKeyedService<TestClass1>("keySingleton"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestClass2>("keyScoped"));
    EXPECT_TRUE(provider.tryCreateKeyedService<TestClass3>("keyTransient"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestClass4>("keySingleton"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestClass1>("keySingleton2"));
}

TEST_F(BasicKeyedTest, ShouldCreateService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestClass1>("keySingleton")
                        .addKeyedScoped<TestClass2>("keyScoped")
                        .addKeyedTransient<TestClass3>("keyTransient")
                        .buildServiceProvider();

    EXPECT_THROW(provider.createKeyedService<TestClass1>("keySingleton"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedService<TestClass2>("keyScoped"), sb::di::ServiceNotFoundException);
    EXPECT_NO_THROW(provider.createKeyedService<TestClass3>("keyTransient"));
    EXPECT_THROW(provider.createKeyedService<TestClass4>("keySingleton"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedService<TestClass3>("keyTransient2"), sb::di::ServiceNotFoundException);
}

TEST_F(BasicKeyedTest, ShouldCreateServiceInPlace)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestClass1>("keySingleton")
                        .addKeyedScoped<TestClass2>("keyScoped")
                        .addKeyedTransient<TestClass3>("keyTransient")
                        .buildServiceProvider();

    EXPECT_THROW(provider.createKeyedServiceInPlace<TestClass1>("keySingleton"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestClass2>("keyScoped"), sb::di::ServiceNotFoundException);
    EXPECT_NO_THROW(provider.createKeyedServiceInPlace<TestClass3>("keyTransient"));
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestClass4>("keySingleton"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestClass3>("keyTransient2"), sb::di::ServiceNotFoundException);
}

TEST_F(BasicKeyedTest, ShouldCreateServicess)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestClass1>("keySingleton")
                        .addKeyedScoped<TestClass2>("keyScoped")
                        .addKeyedTransient<TestClass3>("keyTransient")
                        .buildServiceProvider();

    EXPECT_EQ(provider.createKeyedServices<TestClass1>("keySingleton").size(), 0);
    EXPECT_EQ(provider.createKeyedServices<TestClass2>("keyScoped").size(), 0);
    EXPECT_EQ(provider.createKeyedServices<TestClass3>("keyTransient").size(), 1);
    EXPECT_EQ(provider.createKeyedServices<TestClass1>("keySingleton2").size(), 0);
}

TEST_F(BasicKeyedTest, ShouldCreateEmptyServicesForNotExisting)
{
    auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    const auto all = provider.createKeyedServices<TestClass1>("keyTransient");
    EXPECT_TRUE(all.empty());
}
