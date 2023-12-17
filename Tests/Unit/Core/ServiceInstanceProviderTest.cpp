#include <gtest/gtest.h>
#include <memory>

#include "../../Helpers/Classes/Basic.hpp"
#include "../../Helpers/Classes/CirularDependency.hpp"
#include "../../Helpers/Classes/Inherit.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceCollection.hpp"

class ServiceInstanceProviderTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    ServiceInstanceProviderTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceInstanceProviderTest() override = default;

    static void TearDownTestSuite() {}
};

// buildServiceProvider Tests

TEST_F(ServiceInstanceProviderTest, ShouldFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    collection.addSingleton<TestClass1>();
    collection.addSingleton<TestClass1>();
    collection.addSingleton<TestClass2>();

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = true;
    EXPECT_THROW(collection.buildServiceProvider(options), sb::di::ServiceAlreadyRegisteredException);
}

TEST_F(ServiceInstanceProviderTest, ShouldFailGetServiceDueToAlreadyRegisteredInheritedService)
{
    sb::di::ServiceCollection collection;

    collection.addSingleton<TestInheritClass1, TestInheritClass2>();
    collection.addSingleton<TestInheritClass1, TestInheritClass2>();
    collection.addSingleton<TestInheritClass1, TestInheritClass4>();

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = true;
    EXPECT_THROW(collection.buildServiceProvider(options), sb::di::ServiceAlreadyRegisteredException);
}

TEST_F(ServiceInstanceProviderTest, ShouldFailGetServiceDueToLifetimeMissmatchInheritedService)
{
    sb::di::ServiceCollection collection;

    collection.addSingleton<TestInheritClass1, TestInheritClass3>();
    collection.addSingleton<TestInheritClass1, TestInheritClass5>();
    collection.addTransient<TestInheritClass1, TestInheritClass4>();

    EXPECT_THROW(collection.buildServiceProvider(), sb::di::ServiceLifeTimeMismatchException);
}

// tryGetInstance Tests

TEST_F(ServiceInstanceProviderTest, ShouldTryGetInstance)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    auto &instanceProvider = provider.getInstanceProvider();
    EXPECT_TRUE(instanceProvider.tryGetInstance(typeid(TestClass1)));
    EXPECT_TRUE(instanceProvider.tryGetInstance(typeid(TestClass2)));
    EXPECT_FALSE(instanceProvider.tryGetInstance(typeid(TestClass3)));
    EXPECT_FALSE(instanceProvider.tryGetInstance(typeid(TestClass4)));
}

TEST_F(ServiceInstanceProviderTest, ShouldTryGetInheritedInstance)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass4>()
                        .buildServiceProvider();

    auto &instanceProvider = provider.getInstanceProvider();
    EXPECT_TRUE(instanceProvider.tryGetInstance(typeid(TestInheritClass1)));
    EXPECT_TRUE(instanceProvider.tryGetInstance(typeid(TestInheritClass2)));
    EXPECT_FALSE(instanceProvider.tryGetInstance(typeid(TestInheritClass3)));
    EXPECT_FALSE(instanceProvider.tryGetInstance(typeid(TestInheritClass4)));
}

// tryGetService Tests

TEST_F(ServiceInstanceProviderTest, ShouldTryGetService)
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

TEST_F(ServiceInstanceProviderTest, ShouldTryGetInheritedService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass4>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetService<TestInheritClass1>());
    EXPECT_TRUE(provider.tryGetService<TestInheritClass2>());
    EXPECT_FALSE(provider.tryGetService<TestInheritClass3>());
    EXPECT_FALSE(provider.tryGetService<TestInheritClass4>());
}

TEST_F(ServiceInstanceProviderTest, ShouldTryGetOneLastServiceSingleton)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addSingleton<TestInheritClass1, TestInheritClass3>()
                        .addSingleton<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetService<TestInheritClass1>());
    EXPECT_EQ(provider.tryGetService<TestInheritClass1>()->number(), 5);
}

TEST_F(ServiceInstanceProviderTest, ShouldTryGetOneLastServiceScope)
{
    auto provider = sb::di::ServiceCollection{}
                        .addScoped<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass1, TestInheritClass3>()
                        .addScoped<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetService<TestInheritClass1>());
    EXPECT_EQ(provider.tryGetService<TestInheritClass1>()->number(), 5);
}

TEST_F(ServiceInstanceProviderTest, ShouldFailTryGetServiceDueToCircularDependency)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<CircularDependencyA>()
                        .addScoped<CircularDependencyB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider.tryGetService<CircularDependencyA>(), sb::di::CircularDependencyException);
}

TEST_F(ServiceInstanceProviderTest, ShouldTryGetSelf)
{
    auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    const auto self = provider.tryGetService<sb::di::ServiceProvider>();
    EXPECT_TRUE(self);
    EXPECT_EQ(self, &provider);
}

// getInstance Tests

TEST_F(ServiceInstanceProviderTest, ShouldGetInstance)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    auto &instanceProvider = provider.getInstanceProvider();
    EXPECT_NO_THROW(instanceProvider.getInstance(typeid(TestClass1)));
    EXPECT_NO_THROW(instanceProvider.getInstance(typeid(TestClass2)));
    EXPECT_TRUE(instanceProvider.getInstance(typeid(TestClass1)));
    EXPECT_TRUE(instanceProvider.getInstance(typeid(TestClass2)));
    EXPECT_THROW(instanceProvider.getInstance(typeid(TestClass3)), sb::di::ServiceNotFoundException);
    EXPECT_THROW(instanceProvider.getInstance(typeid(TestClass4)), sb::di::ServiceNotFoundException);
}

TEST_F(ServiceInstanceProviderTest, ShouldGetInheritedInstance)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass4>()
                        .buildServiceProvider();

    auto &instanceProvider = provider.getInstanceProvider();
    EXPECT_NO_THROW(instanceProvider.getInstance(typeid(TestInheritClass1)));
    EXPECT_NO_THROW(instanceProvider.getInstance(typeid(TestInheritClass2)));
    EXPECT_TRUE(instanceProvider.getInstance(typeid(TestInheritClass1)));
    EXPECT_TRUE(instanceProvider.getInstance(typeid(TestInheritClass2)));
    EXPECT_THROW(instanceProvider.getInstance(typeid(TestInheritClass3)), sb::di::ServiceNotFoundException);
    EXPECT_THROW(instanceProvider.getInstance(typeid(TestInheritClass4)), sb::di::ServiceNotFoundException);
}

// getService Tests

TEST_F(ServiceInstanceProviderTest, ShouldGetService)
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

TEST_F(ServiceInstanceProviderTest, ShouldGetInheritedService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass4>()
                        .buildServiceProvider();

    EXPECT_NO_THROW(provider.getService<TestInheritClass1>());
    EXPECT_NO_THROW(provider.getService<TestInheritClass2>());
    EXPECT_THROW(provider.getService<TestInheritClass3>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.getService<TestInheritClass4>(), sb::di::ServiceNotFoundException);
}

TEST_F(ServiceInstanceProviderTest, ShouldGetOneLastServiceSingleton)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addSingleton<TestInheritClass1, TestInheritClass3>()
                        .addSingleton<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    EXPECT_EQ(provider.getService<TestInheritClass1>().number(), 5);
}

TEST_F(ServiceInstanceProviderTest, ShouldGetOneLastServiceScope)
{
    auto provider = sb::di::ServiceCollection{}
                        .addScoped<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass1, TestInheritClass3>()
                        .addScoped<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    EXPECT_EQ(provider.getService<TestInheritClass1>().number(), 5);
}

TEST_F(ServiceInstanceProviderTest, ShouldFailGetServiceDueToCircularDependency)
{

    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<CircularDependencyA>()
                        .addScoped<CircularDependencyB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider.getService<CircularDependencyA>(), sb::di::CircularDependencyException);
}

TEST_F(ServiceInstanceProviderTest, ShouldGetSelf)
{
    auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    auto &self = provider.getService<sb::di::ServiceProvider>();
    EXPECT_EQ(&self, &provider);
}

// getInstances Tests

TEST_F(ServiceInstanceProviderTest, ShouldGetInstances)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    auto &instanceProvider = provider.getInstanceProvider();
    EXPECT_EQ(instanceProvider.tryGetInstances(typeid(TestClass1))->size(), 1);
    EXPECT_EQ(instanceProvider.tryGetInstances(typeid(TestClass2))->size(), 1);
    EXPECT_FALSE(instanceProvider.tryGetInstances(typeid(TestClass3)));
    EXPECT_FALSE(instanceProvider.tryGetInstances(typeid(TestClass4)));
}

TEST_F(ServiceInstanceProviderTest, ShouldGetInstancesInOrder)
{
    auto provider = sb::di::ServiceCollection{}
                        .addScoped<TestInheritClass1>()
                        .addScoped<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass1, TestInheritClass3>()
                        .addScoped<TestInheritClass1, TestInheritClass4>()
                        .addScoped<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    auto &services = *provider.getInstanceProvider().tryGetInstances(typeid(TestInheritClass1));
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[0]->get())->number(), 1);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[1]->get())->number(), 2);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[2]->get())->number(), 3);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[3]->get())->number(), 4);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[4]->get())->number(), 5);
}

// getServices Tests

TEST_F(ServiceInstanceProviderTest, ShouldGetInheritedServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass4>()
                        .buildServiceProvider();

    EXPECT_EQ(provider.getServices<TestInheritClass1>().size(), 1);
    EXPECT_EQ(provider.getServices<TestInheritClass2>().size(), 1);
    EXPECT_TRUE(provider.getServices<TestInheritClass3>().empty());
    EXPECT_TRUE(provider.getServices<TestInheritClass4>().empty());
}

TEST_F(ServiceInstanceProviderTest, ShouldGetCastedServicesInOrder)
{
    auto provider = sb::di::ServiceCollection{}
                        .addScoped<TestInheritClass1>()
                        .addScoped<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass1, TestInheritClass3>()
                        .addScoped<TestInheritClass1, TestInheritClass4>()
                        .addScoped<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    const auto services = provider.getServices<TestInheritClass1>();
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(services[0]->number(), 1);
    EXPECT_EQ(services[1]->number(), 2);
    EXPECT_EQ(services[2]->number(), 3);
    EXPECT_EQ(services[3]->number(), 4);
    EXPECT_EQ(services[4]->number(), 5);
}

TEST_F(ServiceInstanceProviderTest, ShouldGetServicesInOrderAfterNormalGet)
{
    auto provider = sb::di::ServiceCollection{}
                        .addScoped<TestInheritClass1>()
                        .addScoped<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass1, TestInheritClass3>()
                        .addScoped<TestInheritClass1, TestInheritClass4>()
                        .addScoped<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetService<TestInheritClass1>());
    const auto services = provider.getServices<TestInheritClass1>();
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(services[0]->number(), 1);
    EXPECT_EQ(services[1]->number(), 2);
    EXPECT_EQ(services[2]->number(), 3);
    EXPECT_EQ(services[3]->number(), 4);
    EXPECT_EQ(services[4]->number(), 5);
    EXPECT_TRUE(provider.tryGetService<TestInheritClass1>());
}

TEST_F(ServiceInstanceProviderTest, ShouldGetEmptyServicesForNotExisting)
{
    auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    const auto all = provider.getServices<TestClass1>();
    EXPECT_TRUE(all.empty());
}

TEST_F(ServiceInstanceProviderTest, ShouldFailGetServicesDueToCircularDependency)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<CircularDependencyA>()
                        .addScoped<CircularDependencyB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider.getServices<CircularDependencyA>(), sb::di::CircularDependencyException);
}

TEST_F(ServiceInstanceProviderTest, ShouldGetSelfServices)
{
    auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    const auto services = provider.getServices<sb::di::ServiceProvider>();
    EXPECT_EQ(services.size(), 1);
    EXPECT_EQ(services[0], &provider);
}

// tryCreateInstance Tests

TEST_F(ServiceInstanceProviderTest, ShouldTryCreateInstance)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    auto &instanceProvider = provider.getInstanceProvider();
    EXPECT_FALSE(instanceProvider.tryCreateInstance(typeid(TestClass1)));
    EXPECT_FALSE(instanceProvider.tryCreateInstance(typeid(TestClass2)));
    EXPECT_TRUE(instanceProvider.tryCreateInstance(typeid(TestClass3)));
    EXPECT_FALSE(instanceProvider.tryCreateInstance(typeid(TestClass4)));
}

TEST_F(ServiceInstanceProviderTest, ShouldTryCreateInheritedInstance)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass4>()
                        .buildServiceProvider();

    auto &instanceProvider = provider.getInstanceProvider();
    EXPECT_FALSE(instanceProvider.tryCreateInstance(typeid(TestInheritClass1)));
    EXPECT_FALSE(instanceProvider.tryCreateInstance(typeid(TestInheritClass2)));
    EXPECT_TRUE(instanceProvider.tryCreateInstance(typeid(TestInheritClass3)));
    EXPECT_FALSE(instanceProvider.tryCreateInstance(typeid(TestInheritClass4)));
}

// tryCreateService Tests

TEST_F(ServiceInstanceProviderTest, ShouldTryCreateService)
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

TEST_F(ServiceInstanceProviderTest, ShouldTryCreateInheritedService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass4>()
                        .buildServiceProvider();

    EXPECT_FALSE(provider.tryCreateService<TestInheritClass1>());
    EXPECT_FALSE(provider.tryCreateService<TestInheritClass2>());
    EXPECT_TRUE(provider.tryCreateService<TestInheritClass3>());
    EXPECT_FALSE(provider.tryCreateService<TestInheritClass4>());
}

TEST_F(ServiceInstanceProviderTest, ShouldFailTryCreateServiceDueToCircularDependency)
{
    auto provider = sb::di::ServiceCollection{}
                        .addTransient<CircularDependencyUniqueA>()
                        .addTransient<CircularDependencyUniqueB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider.tryCreateService<CircularDependencyUniqueA>(), sb::di::CircularDependencyException);
}

TEST_F(ServiceInstanceProviderTest, ShouldNotTryCreateSelf)
{
    auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    EXPECT_FALSE(provider.tryCreateService<sb::di::ServiceProvider>());
}

// createInstance Tests

TEST_F(ServiceInstanceProviderTest, ShouldCreateInstance)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    auto &instanceProvider = provider.getInstanceProvider();
    EXPECT_THROW(instanceProvider.createInstance(typeid(TestClass1)), sb::di::ServiceNotFoundException);
    EXPECT_THROW(instanceProvider.createInstance(typeid(TestClass2)), sb::di::ServiceNotFoundException);
    EXPECT_NO_THROW(instanceProvider.createInstance(typeid(TestClass3)));
    EXPECT_TRUE(instanceProvider.createInstance(typeid(TestClass3)));
    EXPECT_THROW(instanceProvider.createInstance(typeid(TestClass4)), sb::di::ServiceNotFoundException);
}

TEST_F(ServiceInstanceProviderTest, ShouldCreateInheritedInstance)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass4>()
                        .buildServiceProvider();

    auto &instanceProvider = provider.getInstanceProvider();
    EXPECT_THROW(instanceProvider.createInstance(typeid(TestInheritClass1)), sb::di::ServiceNotFoundException);
    EXPECT_THROW(instanceProvider.createInstance(typeid(TestInheritClass2)), sb::di::ServiceNotFoundException);
    EXPECT_NO_THROW(instanceProvider.createInstance(typeid(TestInheritClass3)));
    EXPECT_TRUE(instanceProvider.createInstance(typeid(TestInheritClass3)));
    EXPECT_THROW(instanceProvider.createInstance(typeid(TestInheritClass4)), sb::di::ServiceNotFoundException);
}

// createInstance Tests

TEST_F(ServiceInstanceProviderTest, ShouldCreateService)
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

TEST_F(ServiceInstanceProviderTest, ShouldCreateInheritedService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass4>()
                        .buildServiceProvider();

    EXPECT_THROW(provider.createService<TestInheritClass1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createService<TestInheritClass2>(), sb::di::ServiceNotFoundException);
    EXPECT_NO_THROW(provider.createService<TestInheritClass3>());
    EXPECT_THROW(provider.createService<TestInheritClass4>(), sb::di::ServiceNotFoundException);
}

TEST_F(ServiceInstanceProviderTest, ShouldFailCreateServiceDueToCircularDependency)
{
    auto provider = sb::di::ServiceCollection{}
                        .addTransient<CircularDependencyUniqueA>()
                        .addTransient<CircularDependencyUniqueB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider.createService<CircularDependencyUniqueA>(), sb::di::CircularDependencyException);
}

TEST_F(ServiceInstanceProviderTest, ShouldNotCreateSelfService)
{
    auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    EXPECT_THROW(provider.createService<sb::di::ServiceProvider>(), sb::di::ServiceNotFoundException);
}

// createInstances Tests

TEST_F(ServiceInstanceProviderTest, ShouldCreateInstances)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    auto &instanceProvider = provider.getInstanceProvider();
    EXPECT_FALSE(instanceProvider.tryCreateInstances(typeid(TestClass1)));
    EXPECT_FALSE(instanceProvider.tryCreateInstances(typeid(TestClass2)));
    EXPECT_EQ(instanceProvider.tryCreateInstances(typeid(TestClass3))->size(), 1);
    EXPECT_FALSE(instanceProvider.tryCreateInstances(typeid(TestClass4)));
}

TEST_F(ServiceInstanceProviderTest, ShouldCreateInstancesInOrder)
{
    auto provider = sb::di::ServiceCollection{}
                        .addTransient<TestInheritClass1>()
                        .addTransient<TestInheritClass1, TestInheritClass2>()
                        .addTransient<TestInheritClass1, TestInheritClass3>()
                        .addTransient<TestInheritClass1, TestInheritClass4>()
                        .addTransient<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    auto services = *provider.getInstanceProvider().tryCreateInstances(typeid(TestInheritClass1));
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[0]->get())->number(), 1);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[1]->get())->number(), 2);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[2]->get())->number(), 3);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[3]->get())->number(), 4);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[4]->get())->number(), 5);
}

// createServices Tests

TEST_F(ServiceInstanceProviderTest, ShouldCreateInheritedServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass4>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider.createServices<TestInheritClass1>().empty());
    EXPECT_TRUE(provider.createServices<TestInheritClass2>().empty());
    EXPECT_EQ(provider.createServices<TestInheritClass3>().size(), 1);
    EXPECT_TRUE(provider.createServices<TestInheritClass4>().empty());
}

TEST_F(ServiceInstanceProviderTest, ShouldCreateServicesInOrder)
{
    auto provider = sb::di::ServiceCollection{}
                        .addTransient<TestInheritClass1>()
                        .addTransient<TestInheritClass1, TestInheritClass2>()
                        .addTransient<TestInheritClass1, TestInheritClass3>()
                        .addTransient<TestInheritClass1, TestInheritClass4>()
                        .addTransient<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    const auto services = provider.createServices<TestInheritClass1>();
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(services[0]->number(), 1);
    EXPECT_EQ(services[1]->number(), 2);
    EXPECT_EQ(services[2]->number(), 3);
    EXPECT_EQ(services[3]->number(), 4);
    EXPECT_EQ(services[4]->number(), 5);
}

TEST_F(ServiceInstanceProviderTest, ShouldCreateEmptyServicesForNotExisting)
{
    auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    const auto all = provider.createServices<TestClass1>();
    EXPECT_TRUE(all.empty());
}

TEST_F(ServiceInstanceProviderTest, ShouldFaildCreateServicesDueToCircularDependency)
{
    auto provider = sb::di::ServiceCollection{}
                        .addTransient<CircularDependencyUniqueA>()
                        .addTransient<CircularDependencyUniqueB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider.createServices<CircularDependencyUniqueA>(), sb::di::CircularDependencyException);
}

TEST_F(ServiceInstanceProviderTest, ShouldFailCreateSelfServices)
{
    auto provider = sb::di::ServiceCollection{}.buildServiceProvider();

    const auto services = provider.createServices<sb::di::ServiceProvider>();
    EXPECT_TRUE(services.empty());
}
