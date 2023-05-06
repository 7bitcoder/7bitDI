#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <thread>

#include "Classes/BasicInherit.hpp"
#include "Classes/BasicTest.hpp"
#include "Classes/CirularDependency.hpp"
#include "SevenBit/Exceptions.hpp"
#include "SevenBit/ServiceCollection.hpp"

class SeriviceProviderTest : public ::testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    SeriviceProviderTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~SeriviceProviderTest() {}

    static void TearDownTestSuite() {}
};

// buildServiceProvider Tests

TEST_F(SeriviceProviderTest, ShouldFailGetServiceDueToAlreadyRegisteredService)
{
    sb::ServiceCollection collection;

    collection.addSingleton<TestClass1>();
    collection.addSingleton<TestClass1>();
    collection.addSingleton<TestClass2>();

    EXPECT_THROW((collection.buildServiceProvider()), sb::ServiceAlreadyRegisteredException);
}

TEST_F(SeriviceProviderTest, ShouldFailGetServiceDueToAlreadyRegisteredInheritedService)
{
    sb::ServiceCollection collection;

    collection.addSingleton<TestInheritClass1, TestInheritClass2>();
    collection.addSingleton<TestInheritClass1, TestInheritClass2>();
    collection.addSingleton<TestInheritClass1, TestInheritClass4>();

    EXPECT_THROW((collection.buildServiceProvider()), sb::ServiceAlreadyRegisteredException);
}

TEST_F(SeriviceProviderTest, ShouldFailGetServiceDueToLifetimeMissmatchInheritedService)
{
    sb::ServiceCollection collection;

    collection.addSingleton<TestInheritClass1, TestInheritClass3>();
    collection.addSingleton<TestInheritClass1, TestInheritClass5>();
    collection.addTransient<TestInheritClass1, TestInheritClass4>();

    EXPECT_THROW((collection.buildServiceProvider()), sb::ServiceLifeTimeMissmatchException);
}

// tryGetInstance Tests

TEST_F(SeriviceProviderTest, ShouldTryGetInstance)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider->tryGetInstance(typeid(TestClass1)));
    EXPECT_TRUE(provider->tryGetInstance(typeid(TestClass2)));
    EXPECT_FALSE(provider->tryGetInstance(typeid(TestClass3)));
    EXPECT_FALSE(provider->tryGetInstance(typeid(TestClass4)));
}

TEST_F(SeriviceProviderTest, ShouldTryGetInheritedInstance)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass4>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider->tryGetInstance(typeid(TestInheritClass1)));
    EXPECT_TRUE(provider->tryGetInstance(typeid(TestInheritClass2)));
    EXPECT_FALSE(provider->tryGetInstance(typeid(TestInheritClass3)));
    EXPECT_FALSE(provider->tryGetInstance(typeid(TestInheritClass4)));
}

// tryGetService Tests

TEST_F(SeriviceProviderTest, ShouldTryGetService)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider->tryGetService<TestClass1>());
    EXPECT_TRUE(provider->tryGetService<TestClass2>());
    EXPECT_FALSE(provider->tryGetService<TestClass3>());
    EXPECT_FALSE(provider->tryGetService<TestClass4>());
}

TEST_F(SeriviceProviderTest, ShouldTryGetInheritedService)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass4>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider->tryGetService<TestInheritClass1>());
    EXPECT_TRUE(provider->tryGetService<TestInheritClass2>());
    EXPECT_FALSE(provider->tryGetService<TestInheritClass3>());
    EXPECT_FALSE(provider->tryGetService<TestInheritClass4>());
}

TEST_F(SeriviceProviderTest, ShouldTryGetOneLastServiceSingleton)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addSingleton<TestInheritClass1, TestInheritClass3>()
                        .addSingleton<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider->tryGetService<TestInheritClass1>());
    EXPECT_EQ(provider->tryGetService<TestInheritClass1>()->number(), 5);
}

TEST_F(SeriviceProviderTest, ShouldTryGetOneLastServiceScope)
{
    auto provider = sb::ServiceCollection{}
                        .addScoped<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass1, TestInheritClass3>()
                        .addScoped<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider->tryGetService<TestInheritClass1>());
    EXPECT_EQ(provider->tryGetService<TestInheritClass1>()->number(), 5);
}

TEST_F(SeriviceProviderTest, ShouldFailTryGetServiceDueToCircularDependency)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<CircularDependencyA>()
                        .addScoped<CircularDependencyB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider->tryGetService<CircularDependencyA>(), sb::CircularDependencyException);
}

TEST_F(SeriviceProviderTest, ShouldTryGetSelf)
{
    auto provider = sb::ServiceCollection{}.buildServiceProvider();

    auto self = provider->tryGetService<sb::IServiceProvider>();
    EXPECT_TRUE(self);
    EXPECT_EQ(self, provider.get());
}

// getInstance Tests

TEST_F(SeriviceProviderTest, ShouldGetInstance)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    EXPECT_NO_THROW(provider->getInstance(typeid(TestClass1)));
    EXPECT_NO_THROW(provider->getInstance(typeid(TestClass2)));
    EXPECT_TRUE(provider->getInstance(typeid(TestClass1)));
    EXPECT_TRUE(provider->getInstance(typeid(TestClass2)));
    EXPECT_THROW(provider->getInstance(typeid(TestClass3)), sb::ServiceNotFoundException);
    EXPECT_THROW(provider->getInstance(typeid(TestClass4)), sb::ServiceNotFoundException);
}

TEST_F(SeriviceProviderTest, ShouldGetInheritedInstance)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass4>()
                        .buildServiceProvider();

    EXPECT_NO_THROW(provider->getInstance(typeid(TestInheritClass1)));
    EXPECT_NO_THROW(provider->getInstance(typeid(TestInheritClass2)));
    EXPECT_TRUE(provider->getInstance(typeid(TestInheritClass1)));
    EXPECT_TRUE(provider->getInstance(typeid(TestInheritClass2)));
    EXPECT_THROW(provider->getInstance(typeid(TestInheritClass3)), sb::ServiceNotFoundException);
    EXPECT_THROW(provider->getInstance(typeid(TestInheritClass4)), sb::ServiceNotFoundException);
}

// getService Tests

TEST_F(SeriviceProviderTest, ShouldGetService)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    EXPECT_NO_THROW(provider->getService<TestClass1>());
    EXPECT_NO_THROW(provider->getService<TestClass2>());
    EXPECT_THROW(provider->getService<TestClass3>(), sb::ServiceNotFoundException);
    EXPECT_THROW(provider->getService<TestClass4>(), sb::ServiceNotFoundException);
}

TEST_F(SeriviceProviderTest, ShouldGetInheritedService)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass4>()
                        .buildServiceProvider();

    EXPECT_NO_THROW(provider->getService<TestInheritClass1>());
    EXPECT_NO_THROW(provider->getService<TestInheritClass2>());
    EXPECT_THROW(provider->getService<TestInheritClass3>(), sb::ServiceNotFoundException);
    EXPECT_THROW(provider->getService<TestInheritClass4>(), sb::ServiceNotFoundException);
}

TEST_F(SeriviceProviderTest, ShouldGetOneLastServiceSingleton)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addSingleton<TestInheritClass1, TestInheritClass3>()
                        .addSingleton<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    EXPECT_EQ(provider->getService<TestInheritClass1>().number(), 5);
}

TEST_F(SeriviceProviderTest, ShouldGetOneLastServiceScope)
{
    auto provider = sb::ServiceCollection{}
                        .addScoped<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass1, TestInheritClass3>()
                        .addScoped<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    EXPECT_EQ(provider->getService<TestInheritClass1>().number(), 5);
}

TEST_F(SeriviceProviderTest, ShouldFailGetServiceDueToCircularDependency)
{

    auto provider = sb::ServiceCollection{}
                        .addSingleton<CircularDependencyA>()
                        .addScoped<CircularDependencyB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider->getService<CircularDependencyA>(), sb::CircularDependencyException);
}

TEST_F(SeriviceProviderTest, ShouldGetSelf)
{
    auto provider = sb::ServiceCollection{}.buildServiceProvider();

    auto &self = provider->getService<sb::IServiceProvider>();
    EXPECT_EQ(&self, provider.get());
}

// getInstances Tests

TEST_F(SeriviceProviderTest, ShouldGetInstances)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    EXPECT_EQ(provider->getInstances(typeid(TestClass1)).size(), 1);
    EXPECT_EQ(provider->getInstances(typeid(TestClass2)).size(), 1);
    EXPECT_TRUE(provider->getInstances(typeid(TestClass3)).empty());
    EXPECT_TRUE(provider->getInstances(typeid(TestClass4)).empty());
}

TEST_F(SeriviceProviderTest, ShouldGetInstancesInOrder)
{
    auto provider = sb::ServiceCollection{}
                        .addScoped<TestInheritClass1>()
                        .addScoped<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass1, TestInheritClass3>()
                        .addScoped<TestInheritClass1, TestInheritClass4>()
                        .addScoped<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    auto services = provider->getInstances(typeid(TestInheritClass1));
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[0]->get())->number(), 1);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[1]->get())->number(), 2);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[2]->get())->number(), 3);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[3]->get())->number(), 4);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[4]->get())->number(), 5);
}

// getServices Tests

TEST_F(SeriviceProviderTest, ShouldGetInheritedServices)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass4>()
                        .buildServiceProvider();

    EXPECT_EQ(provider->getServices<TestInheritClass1>().size(), 1);
    EXPECT_EQ(provider->getServices<TestInheritClass2>().size(), 1);
    EXPECT_TRUE(provider->getServices<TestInheritClass3>().empty());
    EXPECT_TRUE(provider->getServices<TestInheritClass4>().empty());
}

TEST_F(SeriviceProviderTest, ShouldGetCastedServicesInOrder)
{
    auto provider = sb::ServiceCollection{}
                        .addScoped<TestInheritClass1>()
                        .addScoped<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass1, TestInheritClass3>()
                        .addScoped<TestInheritClass1, TestInheritClass4>()
                        .addScoped<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    auto services = provider->getServices<TestInheritClass1>();
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(services[0]->number(), 1);
    EXPECT_EQ(services[1]->number(), 2);
    EXPECT_EQ(services[2]->number(), 3);
    EXPECT_EQ(services[3]->number(), 4);
    EXPECT_EQ(services[4]->number(), 5);
}

TEST_F(SeriviceProviderTest, ShouldGetServicesInOrderAfterNormalGet)
{
    auto provider = sb::ServiceCollection{}
                        .addScoped<TestInheritClass1>()
                        .addScoped<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass1, TestInheritClass3>()
                        .addScoped<TestInheritClass1, TestInheritClass4>()
                        .addScoped<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider->tryGetService<TestInheritClass1>());
    auto services = provider->getServices<TestInheritClass1>();
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(services[0]->number(), 1);
    EXPECT_EQ(services[1]->number(), 2);
    EXPECT_EQ(services[2]->number(), 3);
    EXPECT_EQ(services[3]->number(), 4);
    EXPECT_EQ(services[4]->number(), 5);
    EXPECT_TRUE(provider->tryGetService<TestInheritClass1>());
}

TEST_F(SeriviceProviderTest, ShouldGetEmptyServicesForNotExisting)
{
    auto provider = sb::ServiceCollection{}.buildServiceProvider();

    auto all = provider->getServices<TestClass1>();
    EXPECT_TRUE(all.empty());
}

TEST_F(SeriviceProviderTest, ShouldFailGetServicesDueToCircularDependency)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<CircularDependencyA>()
                        .addScoped<CircularDependencyB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider->getServices<CircularDependencyA>(), sb::CircularDependencyException);
}

TEST_F(SeriviceProviderTest, ShouldGetSelfServices)
{
    auto provider = sb::ServiceCollection{}.buildServiceProvider();

    auto services = provider->getServices<sb::IServiceProvider>();
    EXPECT_EQ(services.size(), 1);
    EXPECT_EQ(services[0], provider.get());
}

// tryCreateInstance Tests

TEST_F(SeriviceProviderTest, ShouldTryCreateInstance)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    EXPECT_FALSE(provider->tryCreateInstance(typeid(TestClass1)));
    EXPECT_FALSE(provider->tryCreateInstance(typeid(TestClass2)));
    EXPECT_TRUE(provider->tryCreateInstance(typeid(TestClass3)));
    EXPECT_FALSE(provider->tryCreateInstance(typeid(TestClass4)));
}

TEST_F(SeriviceProviderTest, ShouldTryCreateInheritedInstance)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass4>()
                        .buildServiceProvider();

    EXPECT_FALSE(provider->tryCreateInstance(typeid(TestInheritClass1)));
    EXPECT_FALSE(provider->tryCreateInstance(typeid(TestInheritClass2)));
    EXPECT_TRUE(provider->tryCreateInstance(typeid(TestInheritClass3)));
    EXPECT_FALSE(provider->tryCreateInstance(typeid(TestInheritClass4)));
}

// tryCreateService Tests

TEST_F(SeriviceProviderTest, ShouldTryCreateService)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    EXPECT_FALSE(provider->tryCreateService<TestClass1>());
    EXPECT_FALSE(provider->tryCreateService<TestClass2>());
    EXPECT_TRUE(provider->tryCreateService<TestClass3>());
    EXPECT_FALSE(provider->tryCreateService<TestClass4>());
}

TEST_F(SeriviceProviderTest, ShouldTryCreateInheritedService)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass4>()
                        .buildServiceProvider();

    EXPECT_FALSE(provider->tryCreateService<TestInheritClass1>());
    EXPECT_FALSE(provider->tryCreateService<TestInheritClass2>());
    EXPECT_TRUE(provider->tryCreateService<TestInheritClass3>());
    EXPECT_FALSE(provider->tryCreateService<TestInheritClass4>());
}

TEST_F(SeriviceProviderTest, ShouldFailTryCreateServiceDueToCircularDependency)
{
    auto provider = sb::ServiceCollection{}
                        .addTransient<CircularDependencyUniqueA>()
                        .addTransient<CircularDependencyUniqueB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider->tryCreateService<CircularDependencyUniqueA>(), sb::CircularDependencyException);
}

TEST_F(SeriviceProviderTest, ShouldNotTryCreateSelf)
{
    auto provider = sb::ServiceCollection{}.buildServiceProvider();

    EXPECT_FALSE(provider->tryCreateService<sb::IServiceProvider>());
}

// createInstance Tests

TEST_F(SeriviceProviderTest, ShouldCreateInstance)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    EXPECT_THROW(provider->createInstance(typeid(TestClass1)), sb::ServiceNotFoundException);
    EXPECT_THROW(provider->createInstance(typeid(TestClass2)), sb::ServiceNotFoundException);
    EXPECT_NO_THROW(provider->createInstance(typeid(TestClass3)));
    EXPECT_TRUE(provider->createInstance(typeid(TestClass3)));
    EXPECT_THROW(provider->createInstance(typeid(TestClass4)), sb::ServiceNotFoundException);
}

TEST_F(SeriviceProviderTest, ShouldCreateInheritedInstance)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass4>()
                        .buildServiceProvider();

    EXPECT_THROW(provider->createInstance(typeid(TestInheritClass1)), sb::ServiceNotFoundException);
    EXPECT_THROW(provider->createInstance(typeid(TestInheritClass2)), sb::ServiceNotFoundException);
    EXPECT_NO_THROW(provider->createInstance(typeid(TestInheritClass3)));
    EXPECT_TRUE(provider->createInstance(typeid(TestInheritClass3)));
    EXPECT_THROW(provider->createInstance(typeid(TestInheritClass4)), sb::ServiceNotFoundException);
}

// createService Tests

TEST_F(SeriviceProviderTest, ShouldCreateService)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    EXPECT_THROW(provider->createService<TestClass1>(), sb::ServiceNotFoundException);
    EXPECT_THROW(provider->createService<TestClass2>(), sb::ServiceNotFoundException);
    EXPECT_NO_THROW(provider->createService<TestClass3>());
    EXPECT_THROW(provider->createService<TestClass4>(), sb::ServiceNotFoundException);
}

TEST_F(SeriviceProviderTest, ShouldCreateInheritedService)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass4>()
                        .buildServiceProvider();

    EXPECT_THROW(provider->createService<TestInheritClass1>(), sb::ServiceNotFoundException);
    EXPECT_THROW(provider->createService<TestInheritClass2>(), sb::ServiceNotFoundException);
    EXPECT_NO_THROW(provider->createService<TestInheritClass3>());
    EXPECT_THROW(provider->createService<TestInheritClass4>(), sb::ServiceNotFoundException);
}

TEST_F(SeriviceProviderTest, ShouldFailCreateServiceDueToCircularDependency)
{
    auto provider = sb::ServiceCollection{}
                        .addTransient<CircularDependencyUniqueA>()
                        .addTransient<CircularDependencyUniqueB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider->createService<CircularDependencyUniqueA>(), sb::CircularDependencyException);
}

TEST_F(SeriviceProviderTest, ShouldNotCreateSelfService)
{
    auto provider = sb::ServiceCollection{}.buildServiceProvider();

    EXPECT_THROW(provider->createService<sb::IServiceProvider>(), sb::ServiceNotFoundException);
}

// createInstances Tests

TEST_F(SeriviceProviderTest, ShouldCreateInstances)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider->createInstances(typeid(TestClass1)).empty());
    EXPECT_TRUE(provider->createInstances(typeid(TestClass2)).empty());
    EXPECT_EQ(provider->createInstances(typeid(TestClass3)).size(), 1);
    EXPECT_TRUE(provider->createInstances(typeid(TestClass4)).empty());
}

TEST_F(SeriviceProviderTest, ShouldCreateInstancesInOrder)
{
    auto provider = sb::ServiceCollection{}
                        .addTransient<TestInheritClass1>()
                        .addTransient<TestInheritClass1, TestInheritClass2>()
                        .addTransient<TestInheritClass1, TestInheritClass3>()
                        .addTransient<TestInheritClass1, TestInheritClass4>()
                        .addTransient<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    auto services = provider->createInstances(typeid(TestInheritClass1));
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[0]->get())->number(), 1);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[1]->get())->number(), 2);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[2]->get())->number(), 3);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[3]->get())->number(), 4);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[4]->get())->number(), 5);
}

// createServices Tests

TEST_F(SeriviceProviderTest, ShouldCreateInheritedServices)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass4>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider->createServices<TestInheritClass1>().empty());
    EXPECT_TRUE(provider->createServices<TestInheritClass2>().empty());
    EXPECT_EQ(provider->createServices<TestInheritClass3>().size(), 1);
    EXPECT_TRUE(provider->createServices<TestInheritClass4>().empty());
}

TEST_F(SeriviceProviderTest, ShouldCreateServicesInOrder)
{
    auto provider = sb::ServiceCollection{}
                        .addTransient<TestInheritClass1>()
                        .addTransient<TestInheritClass1, TestInheritClass2>()
                        .addTransient<TestInheritClass1, TestInheritClass3>()
                        .addTransient<TestInheritClass1, TestInheritClass4>()
                        .addTransient<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    auto services = provider->createServices<TestInheritClass1>();
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(services[0]->number(), 1);
    EXPECT_EQ(services[1]->number(), 2);
    EXPECT_EQ(services[2]->number(), 3);
    EXPECT_EQ(services[3]->number(), 4);
    EXPECT_EQ(services[4]->number(), 5);
}

TEST_F(SeriviceProviderTest, ShouldCreateEmptyServicesForNotExisting)
{
    auto provider = sb::ServiceCollection{}.buildServiceProvider();

    auto all = provider->createServices<TestClass1>();
    EXPECT_TRUE(all.empty());
}

TEST_F(SeriviceProviderTest, ShouldFaildCreateServicesDueToCircularDependency)
{
    auto provider = sb::ServiceCollection{}
                        .addTransient<CircularDependencyUniqueA>()
                        .addTransient<CircularDependencyUniqueB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider->createServices<CircularDependencyUniqueA>(), sb::CircularDependencyException);
}

TEST_F(SeriviceProviderTest, ShouldFailCreateSelfServices)
{
    auto provider = sb::ServiceCollection{}.buildServiceProvider();

    auto services = provider->createServices<sb::IServiceProvider>();
    EXPECT_TRUE(services.empty());
}