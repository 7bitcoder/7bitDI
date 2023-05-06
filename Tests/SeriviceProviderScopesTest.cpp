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

class SeriviceProviderScopesTest : public ::testing::Test
{
  protected:
    sb::ServiceCollection collection;

    static void SetUpTestSuite() {}

    SeriviceProviderScopesTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~SeriviceProviderScopesTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(SeriviceProviderScopesTest, ShouldGetServices)
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

TEST_F(SeriviceProviderScopesTest, ShouldGetCastedServices)
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

TEST_F(SeriviceProviderScopesTest, ShouldGetInheritedServices)
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

TEST_F(SeriviceProviderScopesTest, ShouldGetCastedInheritedServices)
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

TEST_F(SeriviceProviderScopesTest, ShouldFailGetServiceDueToCircularDependency)
{

    auto provider = sb::ServiceCollection{}
                        .addSingleton<CircularDependencyA>()
                        .addScoped<CircularDependencyB>()
                        .buildServiceProvider();

    EXPECT_THROW(provider->getService<CircularDependencyA>(), sb::CircularDependencyException);
}

TEST_F(SeriviceProviderScopesTest, ShouldFailGetServiceDueToAlreadyRegisteredService)
{
    sb::ServiceCollection collection;
    
    collection.addSingleton<TestClass1>();
    collection.addSingleton<TestClass1>();
    collection.addSingleton<TestClass2>();

    EXPECT_THROW((collection.buildServiceProvider()), sb::ServiceAlreadyRegisteredException);
}

TEST_F(SeriviceProviderScopesTest, ShouldFailGetServiceDueToAlreadyRegisteredInheritedService)
{
    collection.addSingleton<TestInheritClass1, TestInheritClass3>();
    collection.addSingleton<TestInheritClass1, TestInheritClass3>();
    collection.addSingleton<TestInheritClass1, TestInheritClass4>();

    EXPECT_THROW((collection.buildServiceProvider()), sb::ServiceAlreadyRegisteredException);
}

TEST_F(SeriviceProviderScopesTest, ShouldFailGetServiceDueToLifetimeMissmatchInheritedService)
{
    collection.addSingleton<TestInheritClass1, TestInheritClass3>();
    collection.addSingleton<TestInheritClass1, TestInheritClass5>();
    collection.addTransient<TestInheritClass1, TestInheritClass4>();

    EXPECT_THROW((collection.buildServiceProvider()), sb::ServiceLifeTimeMissmatchException);
}

TEST_F(SeriviceProviderScopesTest, ShouldGetRequiredServices)
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

TEST_F(SeriviceProviderScopesTest, ShouldGetRequiredCastedServices)
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

TEST_F(SeriviceProviderScopesTest, ShouldGetRequiredInheritedServices)
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

TEST_F(SeriviceProviderScopesTest, ShouldGetRequiredCastedInheritedServices)
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

TEST_F(SeriviceProviderScopesTest, ShouldGetSelf)
{
    auto provider = sb::ServiceCollection{}.buildServiceProvider();

    auto self = provider->getService<sb::IServiceProvider>();
    EXPECT_TRUE(self);
    EXPECT_EQ(self, provider.get());
}

TEST_F(SeriviceProviderScopesTest, ShouldGetRequiredSelf)
{
    auto provider = sb::ServiceCollection{}.buildServiceProvider();

    auto &self = provider->getRequiredService<sb::IServiceProvider>();
    EXPECT_EQ(&self, provider.get());
}

TEST_F(SeriviceProviderScopesTest, ShouldCreateServices)
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

TEST_F(SeriviceProviderScopesTest, ShouldCreateCastedServices)
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

TEST_F(SeriviceProviderScopesTest, ShouldCreateInheritedServices)
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

TEST_F(SeriviceProviderScopesTest, ShouldCreateCastedInheritedServices)
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


TEST_F(SeriviceProviderScopesTest, ShouldCreateRequiredServices)
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

TEST_F(SeriviceProviderScopesTest, ShouldCreateRequiredCastedServices)
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

TEST_F(SeriviceProviderScopesTest, ShouldCreateRequiredInheritedServices)
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

TEST_F(SeriviceProviderScopesTest, ShouldCreateRequiredCastedInheritedServices)
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

TEST_F(SeriviceProviderScopesTest, ShouldNotCreateSelf)
{
    auto provider = sb::ServiceCollection{}.buildServiceProvider();

    EXPECT_FALSE(provider->createService<sb::IServiceProvider>());
}

TEST_F(SeriviceProviderScopesTest, ShouldNotCreateRequiredSelf)
{
    auto provider = sb::ServiceCollection{}.buildServiceProvider();

    EXPECT_THROW(provider->createRequiredService<sb::IServiceProvider>(), sb::ServiceNotFoundException);
}


TEST_F(SeriviceProviderScopesTest, GetOneLastServiceSingleton)
{
    auto provider = sb::ServiceCollection{}
                        .addSingleton<TestInheritClass1, TestInheritClass2>()
                        .addSingleton<TestInheritClass1, TestInheritClass3>()
                        .addSingleton<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    EXPECT_EQ(provider->getRequiredService<TestInheritClass1>().number(), 5);
}

TEST_F(SeriviceProviderScopesTest, GetOneLastServiceScope)
{
    auto provider = sb::ServiceCollection{}
                        .addScoped<TestInheritClass1, TestInheritClass2>()
                        .addScoped<TestInheritClass1, TestInheritClass3>()
                        .addScoped<TestInheritClass1, TestInheritClass5>()
                        .buildServiceProvider();

    EXPECT_EQ(provider->getRequiredService<TestInheritClass1>().number(), 5);
}

TEST_F(SeriviceProviderScopesTest, ShouldGetMultipleServicesInOrder)
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

TEST_F(SeriviceProviderScopesTest, ShouldGetMultipleServicesInOrderAfterNormalGet)
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

TEST_F(SeriviceProviderScopesTest, ShouldGetEmptyMultipleServicesForNotExisting)
{
    auto provider = sb::ServiceCollection{}.buildServiceProvider();

    auto all = provider->getServices<TestClass1>();
    EXPECT_TRUE(all.empty());
}

TEST_F(SeriviceProviderScopesTest, GetServiceCorrectReferences)
{
    collection.addSingleton<SimpleClass>();
    collection.addScoped<OneRefClass>();

    auto provider = collection.buildServiceProvider();

    auto simpleClass = provider->getService<SimpleClass>();
    EXPECT_TRUE(simpleClass);
    auto scoped = provider->getService<OneRefClass>();
    EXPECT_TRUE(scoped);
    EXPECT_TRUE(scoped->simpleClass);
    EXPECT_EQ(scoped->simpleClass, simpleClass);
    EXPECT_EQ(provider->getService<SimpleClass>(), simpleClass);
}

TEST_F(SeriviceProviderScopesTest, TransientGetServiceForbidden)
{
    collection.addTransient<TransientClass>();

    auto provider = collection.buildServiceProvider();

    EXPECT_NO_THROW(provider->createService<TransientClass>());
    EXPECT_TRUE(provider->createService<TransientClass>());
}

TEST_F(SeriviceProviderScopesTest, TransientReferencesAreUnique)
{
    collection.addTransient<TransientClass>();
    collection.addScoped<TransientRefClass>();

    auto provider = collection.buildServiceProvider();

    auto scoped = provider->getService<TransientRefClass>();
    EXPECT_TRUE(scoped);
    EXPECT_NE(scoped->transient, provider->createService<TransientClass>());
    EXPECT_NE(provider->createService<TransientClass>(), provider->createService<TransientClass>());
}

TEST_F(SeriviceProviderScopesTest, CreateInheritedTransientServices)
{
    collection.addTransient<IInheranceClass, InheranceClassA>();
    collection.addTransient<IInheranceClass, InheranceClassB>();
    collection.addTransient<IInheranceClass, LongInheritanceC>();

    auto provider = collection.buildServiceProvider();

    auto last = provider->createService<IInheranceClass>();
    EXPECT_EQ(last->getName(), "C");
}

TEST_F(SeriviceProviderScopesTest, CreateAllInheritedTransientServices)
{
    collection.addTransient<IInheranceClass, InheranceClassA>();
    collection.addTransient<IInheranceClass, InheranceClassB>();
    collection.addTransient<IInheranceClass, LongInheritanceC>();

    auto provider = collection.buildServiceProvider();

    provider->createServices<IInheranceClass>();
    auto all = provider->createServices<IInheranceClass>();
    EXPECT_EQ(all.size(), 3);
    EXPECT_EQ(all[0]->getName(), "A");
    EXPECT_EQ(all[1]->getName(), "B");
    EXPECT_EQ(all[2]->getName(), "C");
}

TEST_F(SeriviceProviderScopesTest, CreateAllTransientServicesMustBeUnique)
{
    collection.addTransient<IInheranceClass, InheranceClassA>();
    collection.addTransient<IInheranceClass, InheranceClassB>();
    collection.addTransient<IInheranceClass, LongInheritanceC>();
    collection.addScoped<InheritanceTransientAllRef>();

    auto provider = collection.buildServiceProvider();

    provider->createServices<IInheranceClass>();
    auto all = provider->createServices<IInheranceClass>();
    auto &ref = provider->getRequiredService<InheritanceTransientAllRef>();
    auto &allRef = ref.inheritances;

    // contains diff references
    EXPECT_EQ(all.size(), allRef.size());
    EXPECT_NE(all[2], allRef[0]);
    EXPECT_NE(all[1], allRef[1]);
    EXPECT_NE(all[0], allRef[2]);
}

TEST_F(SeriviceProviderScopesTest, MakeScopedProvider)
{
    collection.addSingleton<IInheranceClass, InheranceClassA>();
    collection.addSingleton<IInheranceClass, InheranceClassB>();
    collection.addSingleton<IInheranceClass, LongInheritanceC>();
    collection.addScoped<InheritanceTransientAllRef>();
    collection.addScoped<SimpleClass>();
    collection.addScoped<OneRefClass>();
    collection.addTransient<TransientClass>();

    auto provider = collection.buildServiceProvider();

    auto scoped = provider->createScope();

    scoped->getService<IInheranceClass>();
    provider->getService<IInheranceClass>();
    EXPECT_EQ(scoped->getService<IInheranceClass>(), provider->getService<IInheranceClass>());
    EXPECT_NE(scoped->getService<OneRefClass>(), provider->getService<OneRefClass>());
    EXPECT_NE(scoped->createService<TransientClass>(), provider->createService<TransientClass>());
}