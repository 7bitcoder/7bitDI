#include <gtest/gtest.h>
#include <memory>

#include "../Helpers/Classes/Basic.hpp"
#include "../Helpers/Classes/CirularDependency.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceCollection.hpp"

class BasicTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    BasicTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~BasicTest() override = default;

    static void TearDownTestSuite() {}
};

// buildServiceProvider Tests

TEST_F(BasicTest, ShouldFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    collection.addSingleton<TestClass1>();
    collection.addSingleton<TestClass1>();
    collection.addSingleton<TestClass2>();

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = true;
    EXPECT_THROW(collection.buildServiceProvider(options), sb::di::ServiceAlreadyRegisteredException);
}

TEST_F(BasicTest, ShouldNotFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    collection.addSingleton<TestClass1>();
    collection.addSingleton<TestClass1>();
    collection.addSingleton<TestClass2>();

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    EXPECT_NO_THROW(collection.buildServiceProvider(options));
}

// tryGetService Tests

TEST_F(BasicTest, ShouldTryGetService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetService<TestClass1>());
    EXPECT_TRUE(provider.tryGetService<TestClass2>());
    EXPECT_FALSE(provider.tryGetService<TestClass3>());
    EXPECT_FALSE(provider.tryGetService<TestClass4>());
}

TEST_F(BasicTest, ShouldFailTryGetServiceDueToCircularDependency)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<CircularDependencyA>()
                        .addScoped<CircularDependencyB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider.tryGetService<CircularDependencyA>(), sb::di::CircularDependencyException);
}

TEST_F(BasicTest, ShouldTryGetSelf)
{
    auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    const auto self = provider.tryGetService<sb::di::ServiceProvider>();
    EXPECT_TRUE(self);
    EXPECT_EQ(self, &provider);
}

// getService Tests

TEST_F(BasicTest, ShouldGetService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    EXPECT_NO_THROW(provider.getService<TestClass1>());
    EXPECT_NO_THROW(provider.getService<TestClass2>());
    EXPECT_THROW(provider.getService<TestClass3>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.getService<TestClass4>(), sb::di::ServiceNotFoundException);
}
TEST_F(BasicTest, ShouldFailGetServiceDueToCircularDependency)
{

    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<CircularDependencyA>()
                        .addScoped<CircularDependencyB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider.getService<CircularDependencyA>(), sb::di::CircularDependencyException);
}

TEST_F(BasicTest, ShouldGetSelf)
{
    auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    auto &self = provider.getService<sb::di::ServiceProvider>();
    EXPECT_EQ(&self, &provider);
}

// getServices Tests

TEST_F(BasicTest, ShouldGetEmptyServicesForNotExisting)
{
    auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    const auto all = provider.getServices<TestClass1>();
    EXPECT_TRUE(all.empty());
}

TEST_F(BasicTest, ShouldFailGetServicesDueToCircularDependency)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<CircularDependencyA>()
                        .addScoped<CircularDependencyB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider.getServices<CircularDependencyA>(), sb::di::CircularDependencyException);
}

TEST_F(BasicTest, ShouldGetSelfServices)
{
    auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    const auto services = provider.getServices<sb::di::ServiceProvider>();
    EXPECT_EQ(services.size(), 1);
    EXPECT_EQ(services[0], &provider);
}

// tryCreateService Tests

TEST_F(BasicTest, ShouldTryCreateService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    EXPECT_FALSE(provider.tryCreateService<TestClass1>());
    EXPECT_FALSE(provider.tryCreateService<TestClass2>());
    EXPECT_TRUE(provider.tryCreateService<TestClass3>());
    EXPECT_FALSE(provider.tryCreateService<TestClass4>());
}

TEST_F(BasicTest, ShouldFailTryCreateServiceDueToCircularDependency)
{
    auto provider = sb::di::ServiceCollection{}
                        .addTransient<CircularDependencyUniqueA>()
                        .addTransient<CircularDependencyUniqueB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider.tryCreateService<CircularDependencyUniqueA>(), sb::di::CircularDependencyException);
}

TEST_F(BasicTest, ShouldNotTryCreateSelf)
{
    auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    EXPECT_FALSE(provider.tryCreateService<sb::di::ServiceProvider>());
}

// createService Tests

TEST_F(BasicTest, ShouldCreateService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    EXPECT_THROW(provider.createService<TestClass1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createService<TestClass2>(), sb::di::ServiceNotFoundException);
    EXPECT_NO_THROW(provider.createService<TestClass3>());
    EXPECT_THROW(provider.createService<TestClass4>(), sb::di::ServiceNotFoundException);
}

TEST_F(BasicTest, ShouldFailCreateServiceDueToCircularDependency)
{
    auto provider = sb::di::ServiceCollection{}
                        .addTransient<CircularDependencyUniqueA>()
                        .addTransient<CircularDependencyUniqueB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider.createService<CircularDependencyUniqueA>(), sb::di::CircularDependencyException);
}

TEST_F(BasicTest, ShouldNotCreateSelfService)
{
    auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    EXPECT_THROW(provider.createService<sb::di::ServiceProvider>(), sb::di::ServiceNotFoundException);
}

// createServiceInPlace Tests

TEST_F(BasicTest, ShouldCreateServiceInPlace)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    EXPECT_THROW(provider.createServiceInPlace<TestClass1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestClass2>(), sb::di::ServiceNotFoundException);
    EXPECT_NO_THROW(provider.createServiceInPlace<TestClass3>());
    EXPECT_THROW(provider.createServiceInPlace<TestClass4>(), sb::di::ServiceNotFoundException);
}

TEST_F(BasicTest, ShouldFailCreateServiceInPlaceDueToCircularDependency)
{
    auto provider = sb::di::ServiceCollection{}
                        .addTransient<CircularDependencyUniqueA>()
                        .addTransient<CircularDependencyUniqueB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider.createServiceInPlace<CircularDependencyUniqueA>(), sb::di::CircularDependencyException);
}

// createServices Tests

TEST_F(BasicTest, ShouldCreateEmptyServicesForNotExisting)
{
    auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    const auto all = provider.createServices<TestClass1>();
    EXPECT_TRUE(all.empty());
}

TEST_F(BasicTest, ShouldFaildCreateServicesDueToCircularDependency)
{
    auto provider = sb::di::ServiceCollection{}
                        .addTransient<CircularDependencyUniqueA>()
                        .addTransient<CircularDependencyUniqueB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider.createServices<CircularDependencyUniqueA>(), sb::di::CircularDependencyException);
}

TEST_F(BasicTest, ShouldFailCreateSelfServices)
{
    auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    const auto services = provider.createServices<sb::di::ServiceProvider>();
    EXPECT_TRUE(services.empty());
}
