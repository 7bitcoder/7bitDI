#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <thread>

#include "Classes/BasicInherit.hpp"
#include "Classes/BasicTest.hpp"
#include "Classes/CirularDependency.hpp"
#include "SevenBit/Exceptions.hpp"
#include "SevenBit/ServiceCollection.hpp"

namespace
{

    struct SimpleClass
    {
        int gg = 100;

        SimpleClass() {}
    };

    struct OneRefClass
    {
        int ff = 100;
        SimpleClass *simpleClass;

        OneRefClass(SimpleClass *s) { simpleClass = s; }
    };

    struct TransientClass
    {
        int ff = 100;
        TransientClass() {}
    };

    struct TransientRefClass
    {
        int ff = 100;
        std::unique_ptr<TransientClass> transient;
        TransientRefClass(std::unique_ptr<TransientClass> t) { transient = std::move(t); }
    };

    struct IInheranceClass
    {
        virtual std::string getName() = 0;
        virtual ~IInheranceClass() {}
    };

    struct InheranceClassA final : public IInheranceClass
    {
        InheranceClassA() {}

        std::string getName() { return "A"; }
    };

    struct InheranceClassB : public IInheranceClass
    {
        InheranceClassB() {}

        std::string getName() { return "B"; }
    };

    struct LongInheritanceC : public InheranceClassB
    {
        LongInheritanceC() {}

        std::string getName() { return "C"; }
    };

    struct InheritanceRef
    {

        IInheranceClass *inheritance;
        InheritanceRef(IInheranceClass *i) { inheritance = i; }
    };

    struct InheritanceAllRef
    {

        std::vector<IInheranceClass *> inheritances;
        InheritanceAllRef(std::vector<IInheranceClass *> i) { inheritances = i; }
    };

    struct InheritanceTransientAllRef
    {

        std::vector<std::unique_ptr<IInheranceClass>> inheritances;
        InheritanceTransientAllRef(std::vector<std::unique_ptr<IInheranceClass>> i) { inheritances = std::move(i); }
    };
} // namespace

class SeriviceProviderTest : public ::testing::Test
{
  protected:
    sb::ServiceCollection collection;

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
    collection.addSingleton<TestInheritClass1, TestInheritClass3>();
    collection.addSingleton<TestInheritClass1, TestInheritClass3>();
    collection.addSingleton<TestInheritClass1, TestInheritClass4>();

    EXPECT_THROW((collection.buildServiceProvider()), sb::ServiceAlreadyRegisteredException);
}

TEST_F(SeriviceProviderTest, ShouldFailGetServiceDueToLifetimeMissmatchInheritedService)
{
    collection.addSingleton<TestInheritClass1, TestInheritClass3>();
    collection.addSingleton<TestInheritClass1, TestInheritClass5>();
    collection.addTransient<TestInheritClass1, TestInheritClass4>();

    EXPECT_THROW((collection.buildServiceProvider()), sb::ServiceLifeTimeMissmatchException);
}

// getService Tests

TEST_F(SeriviceProviderTest, ShouldGetServices)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider->getService(typeid(TestClass1)));
    EXPECT_TRUE(provider->getService(typeid(TestClass2)));
    EXPECT_FALSE(provider->getService(typeid(TestClass3)));
    EXPECT_FALSE(provider->getService(typeid(TestClass4)));
}

TEST_F(SeriviceProviderTest, ShouldGetCastedServices)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider->getService<TestClass1>());
    EXPECT_TRUE(provider->getService<TestClass2>());
    EXPECT_FALSE(provider->getService<TestClass3>());
    EXPECT_FALSE(provider->getService<TestClass4>());
}

TEST_F(SeriviceProviderTest, ShouldGetInheritedServices)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass3>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider->getService(typeid(TestInheritClass1)));
    EXPECT_TRUE(provider->getService(typeid(TestInheritClass2)));
    EXPECT_FALSE(provider->getService(typeid(TestInheritClass3)));
    EXPECT_FALSE(provider->getService(typeid(TestInheritClass4)));
}

TEST_F(SeriviceProviderTest, ShouldGetCastedInheritedServices)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass3>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider->getService<TestInheritClass1>());
    EXPECT_TRUE(provider->getService<TestInheritClass2>());
    EXPECT_FALSE(provider->getService<TestInheritClass3>());
    EXPECT_FALSE(provider->getService<TestInheritClass4>());
}

TEST_F(SeriviceProviderTest, ShouldGetOneLastServiceSingleton)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addSingleton<TestInheritClass1, TestInheritClass3>()
                        .addSingleton<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider->getService<TestInheritClass1>());
    EXPECT_EQ(provider->getService<TestInheritClass1>()->number(), 5);
}

TEST_F(SeriviceProviderTest, ShouldGetOneLastServiceScope)
{
    auto provider = sb::ServiceCollection{}
                        .addScoped<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass1, TestInheritClass3>()
                        .addScoped<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider->getService<TestInheritClass1>());
    EXPECT_EQ(provider->getService<TestInheritClass1>()->number(), 5);
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

    auto self = provider->getService<sb::IServiceProvider>();
    EXPECT_TRUE(self);
    EXPECT_EQ(self, provider.get());
}

// getRequiredService Tests

TEST_F(SeriviceProviderTest, ShouldGetRequiredServices)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    EXPECT_NO_THROW(provider->getRequiredService(typeid(TestClass1)));
    EXPECT_NO_THROW(provider->getRequiredService(typeid(TestClass2)));
    EXPECT_TRUE(provider->getRequiredService(typeid(TestClass1)));
    EXPECT_TRUE(provider->getRequiredService(typeid(TestClass2)));
    EXPECT_THROW(provider->getRequiredService(typeid(TestClass3)), sb::ServiceNotFoundException);
    EXPECT_THROW(provider->getRequiredService(typeid(TestClass4)), sb::ServiceNotFoundException);
}

TEST_F(SeriviceProviderTest, ShouldGetRequiredCastedServices)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    EXPECT_NO_THROW(provider->getRequiredService<TestClass1>());
    EXPECT_NO_THROW(provider->getRequiredService<TestClass2>());
    EXPECT_THROW(provider->getRequiredService<TestClass3>(), sb::ServiceNotFoundException);
    EXPECT_THROW(provider->getRequiredService<TestClass4>(), sb::ServiceNotFoundException);
}

TEST_F(SeriviceProviderTest, ShouldGetRequiredInheritedServices)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass3>()
                        .buildServiceProvider();

    EXPECT_NO_THROW(provider->getRequiredService(typeid(TestInheritClass1)));
    EXPECT_NO_THROW(provider->getRequiredService(typeid(TestInheritClass2)));
    EXPECT_TRUE(provider->getRequiredService(typeid(TestInheritClass1)));
    EXPECT_TRUE(provider->getRequiredService(typeid(TestInheritClass2)));
    EXPECT_THROW(provider->getRequiredService(typeid(TestInheritClass3)), sb::ServiceNotFoundException);
    EXPECT_THROW(provider->getRequiredService(typeid(TestInheritClass4)), sb::ServiceNotFoundException);
}

TEST_F(SeriviceProviderTest, ShouldGetRequiredCastedInheritedServices)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass3>()
                        .buildServiceProvider();

    EXPECT_NO_THROW(provider->getRequiredService<TestInheritClass1>());
    EXPECT_NO_THROW(provider->getRequiredService<TestInheritClass2>());
    EXPECT_THROW(provider->getRequiredService<TestInheritClass3>(), sb::ServiceNotFoundException);
    EXPECT_THROW(provider->getRequiredService<TestInheritClass4>(), sb::ServiceNotFoundException);
}

TEST_F(SeriviceProviderTest, ShouldGetRequiredOneLastServiceSingleton)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addSingleton<TestInheritClass1, TestInheritClass3>()
                        .addSingleton<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    EXPECT_EQ(provider->getRequiredService<TestInheritClass1>().number(), 5);
}

TEST_F(SeriviceProviderTest, ShouldGetRequiredOneLastServiceScope)
{
    auto provider = sb::ServiceCollection{}
                        .addScoped<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass1, TestInheritClass3>()
                        .addScoped<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    EXPECT_EQ(provider->getRequiredService<TestInheritClass1>().number(), 5);
}

TEST_F(SeriviceProviderTest, ShouldFailGetRequiredServiceDueToCircularDependency)
{

    auto provider = sb::ServiceCollection{}
                        .addSingleton<CircularDependencyA>()
                        .addScoped<CircularDependencyB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider->getRequiredService<CircularDependencyA>(), sb::CircularDependencyException);
}

TEST_F(SeriviceProviderTest, ShouldGetRequiredSelf)
{
    auto provider = sb::ServiceCollection{}.buildServiceProvider();

    auto &self = provider->getRequiredService<sb::IServiceProvider>();
    EXPECT_EQ(&self, provider.get());
}


// getServices Tests

TEST_F(SeriviceProviderTest, ShouldGetMultipleServices)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    EXPECT_EQ(provider->getServices(typeid(TestClass1)).size(), 1);
    EXPECT_EQ(provider->getServices(typeid(TestClass2)).size(), 1);
    EXPECT_TRUE(provider->getServices(typeid(TestClass3)).empty());
    EXPECT_TRUE(provider->getServices(typeid(TestClass4)).empty());
}

TEST_F(SeriviceProviderTest, ShouldGetMultipleServicesInOrder)
{
    auto provider = sb::ServiceCollection{}
                        .addScoped<TestInheritClass1>()
                        .addScoped<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass1, TestInheritClass3>()
                        .addScoped<TestInheritClass1, TestInheritClass4>()
                        .addScoped<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    auto services = provider->getServices(typeid(TestInheritClass1));
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(static_cast<TestInheritClass1*>(services[0])->number(), 1);
    EXPECT_EQ(static_cast<TestInheritClass1*>(services[1])->number(), 2);
    EXPECT_EQ(static_cast<TestInheritClass1*>(services[2])->number(), 3);
    EXPECT_EQ(static_cast<TestInheritClass1*>(services[3])->number(), 4);
    EXPECT_EQ(static_cast<TestInheritClass1*>(services[4])->number(), 5);
}

TEST_F(SeriviceProviderTest, ShouldGetMultipleInheritedServices)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass3>()
                        .buildServiceProvider();

    EXPECT_EQ(provider->getServices<TestInheritClass1>().size(), 1);
    EXPECT_EQ(provider->getServices<TestInheritClass2>().size(), 1);
    EXPECT_TRUE(provider->getServices<TestInheritClass3>().empty());
    EXPECT_TRUE(provider->getServices<TestInheritClass4>().empty());
}

TEST_F(SeriviceProviderTest, ShouldGetMultipleCastedServicesInOrder)
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

TEST_F(SeriviceProviderTest, ShouldGetMultipleServicesInOrderAfterNormalGet)
{
    auto provider = sb::ServiceCollection{}
                        .addScoped<TestInheritClass1>()
                        .addScoped<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass1, TestInheritClass3>()
                        .addScoped<TestInheritClass1, TestInheritClass4>()
                        .addScoped<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider->getService<TestInheritClass1>());
    auto services = provider->getServices<TestInheritClass1>();
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(services[0]->number(), 1);
    EXPECT_EQ(services[1]->number(), 2);
    EXPECT_EQ(services[2]->number(), 3);
    EXPECT_EQ(services[3]->number(), 4);
    EXPECT_EQ(services[4]->number(), 5);
    EXPECT_TRUE(provider->getService<TestInheritClass1>());
}

TEST_F(SeriviceProviderTest, ShouldGetEmptyMultipleServicesForNotExisting)
{
    auto provider = sb::ServiceCollection{}.buildServiceProvider();

    auto all = provider->getServices<TestClass1>();
    EXPECT_TRUE(all.empty());
}

TEST_F(SeriviceProviderTest, ShouldFailGetMultipleServicesDueToCircularDependency)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<CircularDependencyA>()
                        .addScoped<CircularDependencyB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider->getServices<CircularDependencyA>(), sb::CircularDependencyException);
}

TEST_F(SeriviceProviderTest, ShouldGetSelfMultipleServices)
{
    auto provider = sb::ServiceCollection{}.buildServiceProvider();

    auto services = provider->getServices<sb::IServiceProvider>();
    EXPECT_EQ(services.size(), 1);
    EXPECT_EQ(services[0], provider.get());
}

// createService Tests

TEST_F(SeriviceProviderTest, ShouldCreateServices)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    EXPECT_FALSE(provider->createService(typeid(TestClass1)));
    EXPECT_FALSE(provider->createService(typeid(TestClass2)));
    EXPECT_TRUE(provider->createService(typeid(TestClass3)));
    EXPECT_FALSE(provider->createService(typeid(TestClass4)));
}

TEST_F(SeriviceProviderTest, ShouldCreateCastedServices)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    EXPECT_FALSE(provider->createService<TestClass1>());
    EXPECT_FALSE(provider->createService<TestClass2>());
    EXPECT_TRUE(provider->createService<TestClass3>());
    EXPECT_FALSE(provider->createService<TestClass4>());
}

TEST_F(SeriviceProviderTest, ShouldCreateInheritedServices)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass3>()
                        .buildServiceProvider();

    EXPECT_FALSE(provider->createService(typeid(TestInheritClass1)));
    EXPECT_FALSE(provider->createService(typeid(TestInheritClass2)));
    EXPECT_TRUE(provider->createService(typeid(TestInheritClass3)));
    EXPECT_FALSE(provider->createService(typeid(TestInheritClass4)));
}

TEST_F(SeriviceProviderTest, ShouldCreateCastedInheritedServices)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass3>()
                        .buildServiceProvider();

    EXPECT_FALSE(provider->createService<TestInheritClass1>());
    EXPECT_FALSE(provider->createService<TestInheritClass2>());
    EXPECT_TRUE(provider->createService<TestInheritClass3>());
    EXPECT_FALSE(provider->createService<TestInheritClass4>());
}

TEST_F(SeriviceProviderTest, ShouldFailCreateServiceDueToCircularDependency)
{
    auto provider = sb::ServiceCollection{}
                        .addTransient<CircularDependencyA>()
                        .addTransient<CircularDependencyB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider->createService<CircularDependencyA>(), sb::CircularDependencyException);
}

TEST_F(SeriviceProviderTest, ShouldNotCreateSelf)
{
    auto provider = sb::ServiceCollection{}.buildServiceProvider();

    EXPECT_FALSE(provider->createService<sb::IServiceProvider>());
}

// createRequiredService Tests

TEST_F(SeriviceProviderTest, ShouldCreateRequiredServices)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    EXPECT_THROW(provider->createRequiredService(typeid(TestClass1)), sb::ServiceNotFoundException);
    EXPECT_THROW(provider->createRequiredService(typeid(TestClass2)), sb::ServiceNotFoundException);
    EXPECT_NO_THROW(provider->createRequiredService(typeid(TestClass3)));
    EXPECT_TRUE(provider->createRequiredService(typeid(TestClass3)));
    EXPECT_THROW(provider->createRequiredService(typeid(TestClass4)), sb::ServiceNotFoundException);
}

TEST_F(SeriviceProviderTest, ShouldCreateRequiredCastedServices)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    EXPECT_THROW(provider->createRequiredService<TestClass1>(), sb::ServiceNotFoundException);
    EXPECT_THROW(provider->createRequiredService<TestClass2>(), sb::ServiceNotFoundException);
    EXPECT_NO_THROW(provider->createRequiredService<TestClass3>());
    EXPECT_THROW(provider->createRequiredService<TestClass4>(), sb::ServiceNotFoundException);
}

TEST_F(SeriviceProviderTest, ShouldCreateRequiredInheritedServices)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass3>()
                        .buildServiceProvider();

    EXPECT_THROW(provider->createRequiredService(typeid(TestInheritClass1)), sb::ServiceNotFoundException);
    EXPECT_THROW(provider->createRequiredService(typeid(TestInheritClass2)), sb::ServiceNotFoundException);
    EXPECT_NO_THROW(provider->createRequiredService(typeid(TestInheritClass3)));
    EXPECT_TRUE(provider->createRequiredService(typeid(TestInheritClass3)));
    EXPECT_THROW(provider->createRequiredService(typeid(TestInheritClass4)), sb::ServiceNotFoundException);
}

TEST_F(SeriviceProviderTest, ShouldCreateRequiredCastedInheritedServices)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass3>()
                        .buildServiceProvider();

    EXPECT_THROW(provider->createRequiredService<TestInheritClass1>(), sb::ServiceNotFoundException);
    EXPECT_THROW(provider->createRequiredService<TestInheritClass2>(), sb::ServiceNotFoundException);
    EXPECT_NO_THROW(provider->createRequiredService<TestInheritClass3>());
    EXPECT_THROW(provider->createRequiredService<TestInheritClass4>(), sb::ServiceNotFoundException);
}

TEST_F(SeriviceProviderTest, ShouldFailCreateRequiredServiceDueToCircularDependency)
{
    auto provider = sb::ServiceCollection{}
                        .addTransient<CircularDependencyA>()
                        .addTransient<CircularDependencyB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider->createRequiredService<CircularDependencyA>(), sb::CircularDependencyException);
}

TEST_F(SeriviceProviderTest, ShouldNotCreateRequiredSelf)
{
    auto provider = sb::ServiceCollection{}.buildServiceProvider();

    EXPECT_THROW(provider->createRequiredService<sb::IServiceProvider>(), sb::ServiceNotFoundException);
}

// createServices Tests

TEST_F(SeriviceProviderTest, ShouldCreateMultipleServices)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestClass1>()
                        .addScoped<TestClass2>()
                        .addTransient<TestClass3>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider->createServices(typeid(TestClass1)).empty());
    EXPECT_TRUE(provider->createServices(typeid(TestClass2)).empty());
    EXPECT_EQ(provider->createServices(typeid(TestClass3)).size(), 1);
    EXPECT_TRUE(provider->createServices(typeid(TestClass4)).empty());
}

TEST_F(SeriviceProviderTest, ShouldCreateMultipleServicesInOrder)
{
    auto provider = sb::ServiceCollection{}
                        .addTransient<TestInheritClass1>()
                        .addTransient<TestInheritClass1, TestInheritClass2>()
                        .addTransient<TestInheritClass1, TestInheritClass3>()
                        .addTransient<TestInheritClass1, TestInheritClass4>()
                        .addTransient<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    auto services = provider->createServices(typeid(TestInheritClass1));
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(static_cast<TestInheritClass1*>(services[0])->number(), 1);
    EXPECT_EQ(static_cast<TestInheritClass1*>(services[1])->number(), 2);
    EXPECT_EQ(static_cast<TestInheritClass1*>(services[2])->number(), 3);
    EXPECT_EQ(static_cast<TestInheritClass1*>(services[3])->number(), 4);
    EXPECT_EQ(static_cast<TestInheritClass1*>(services[4])->number(), 5);
}

TEST_F(SeriviceProviderTest, ShouldCreateMultipleInheritedServices)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass2, TestInheritClass3>()
                        .addTransient<TestInheritClass3, TestInheritClass3>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider->createServices<TestInheritClass1>().empty());
    EXPECT_TRUE(provider->createServices<TestInheritClass2>().empty());
    EXPECT_EQ(provider->createServices<TestInheritClass3>().size(), 1);
    EXPECT_TRUE(provider->createServices<TestInheritClass4>().empty());
}

TEST_F(SeriviceProviderTest, ShouldCreateMultipleCastedServicesInOrder)
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

TEST_F(SeriviceProviderTest, ShouldCreateEmptyMultipleServicesForNotExisting)
{
    auto provider = sb::ServiceCollection{}.buildServiceProvider();

    auto all = provider->createServices<TestClass1>();
    EXPECT_TRUE(all.empty());
}

TEST_F(SeriviceProviderTest, ShouldFaildCreateMultipleServicesDueToCircularDependency)
{
    auto provider = sb::ServiceCollection{}
                        .addTransient<CircularDependencyA>()
                        .addTransient<CircularDependencyB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider->createServices<CircularDependencyA>(), sb::CircularDependencyException);
}

TEST_F(SeriviceProviderTest, ShouldFailCreateSelfMultipleServices)
{
    auto provider = sb::ServiceCollection{}.buildServiceProvider();

    auto services = provider->createServices<sb::IServiceProvider>();
    EXPECT_TRUE(services.empty());
}