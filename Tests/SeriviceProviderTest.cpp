#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <thread>

#include "SevenBit/Injector.hpp"

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

    struct CircularDependencyB;

    struct CircularDependencyA
    {
        CircularDependencyA(CircularDependencyB *b) {}
    };

    struct CircularDependencyB
    {
        CircularDependencyB(CircularDependencyA *a) {}
    };
} // namespace

class SeriviceProviderTest : public ::testing::Test
{
  protected:
    sb::ServiceCollection collection;
    sb::ServiceContainer singletons;
    sb::ServiceProvider provider = collection.buildServiceProvider();

    static void SetUpTestSuite() {}

    SeriviceProviderTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~SeriviceProviderTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(SeriviceProviderTest, AddService) { EXPECT_NO_THROW((collection.addSingleton<SimpleClass>())); }

TEST_F(SeriviceProviderTest, GetService)
{
    collection.addSingleton<SimpleClass>();
    collection.addScoped<OneRefClass>();

    EXPECT_TRUE(provider.getService<SimpleClass>());
    EXPECT_TRUE(provider.getService<OneRefClass>());
    EXPECT_FALSE(provider.getService<TransientRefClass>());
}

TEST_F(SeriviceProviderTest, CircularDependency)
{
    collection.addSingleton<CircularDependencyA>();
    collection.addScoped<CircularDependencyB>();

    EXPECT_THROW(provider.getService<CircularDependencyA>(), sb::CircularDependencyException);
}

TEST_F(SeriviceProviderTest, GetRequiredService)
{
    collection.addSingleton<SimpleClass>();
    collection.addScoped<OneRefClass>();
    collection.addTransient<TransientClass>();

    EXPECT_NO_THROW(provider.getRequiredService<SimpleClass>());
    EXPECT_NO_THROW(provider.getRequiredService<OneRefClass>());
    EXPECT_THROW(provider.getRequiredService<TransientRefClass>(), sb::ServiceNotRegisteredException);
}

TEST_F(SeriviceProviderTest, CreateService)
{
    collection.addTransient<TransientClass>();

    EXPECT_TRUE(provider.createService<TransientClass>());
    EXPECT_NO_THROW(provider.createService<TransientClass>());
}

TEST_F(SeriviceProviderTest, GetServiceCorrectReferences)
{
    collection.addSingleton<SimpleClass>();
    collection.addScoped<OneRefClass>();

    auto simpleClass = provider.getService<SimpleClass>();
    EXPECT_TRUE(simpleClass);
    auto scoped = provider.getService<OneRefClass>();
    EXPECT_TRUE(scoped);
    EXPECT_TRUE(scoped->simpleClass);
    EXPECT_EQ(scoped->simpleClass, simpleClass);
    EXPECT_EQ(provider.getService<SimpleClass>(), simpleClass);
}

TEST_F(SeriviceProviderTest, NotTransientCreationForbidden)
{
    collection.addSingleton<SimpleClass>();
    collection.addScoped<OneRefClass>();

    EXPECT_THROW(provider.createService<SimpleClass>(), sb::NotTransientException);
    EXPECT_THROW(provider.createService<OneRefClass>(), sb::NotTransientException);
}

TEST_F(SeriviceProviderTest, TransientGetServiceForbidden)
{
    collection.addTransient<TransientClass>();

    EXPECT_NO_THROW(provider.createService<TransientClass>());
    EXPECT_TRUE(provider.createService<TransientClass>());
    EXPECT_THROW(provider.getService<TransientClass>(), sb::TransientForbidException);
}

TEST_F(SeriviceProviderTest, TransientReferencesAreUnique)
{
    collection.addTransient<TransientClass>();
    collection.addScoped<TransientRefClass>();

    auto scoped = provider.getService<TransientRefClass>();
    EXPECT_TRUE(scoped);
    EXPECT_NE(scoped->transient, provider.createService<TransientClass>());
    EXPECT_NE(provider.createService<TransientClass>(), provider.createService<TransientClass>());
}

TEST_F(SeriviceProviderTest, GetSelf)
{
    auto self = provider.getService<sb::ServiceProvider>();
    EXPECT_TRUE(self);
    EXPECT_EQ(self, &provider);
}

TEST_F(SeriviceProviderTest, GetSelfRef)
{
    auto &self = provider.getRequiredService<sb::ServiceProvider>();
    EXPECT_EQ(&self, &provider);
}

TEST_F(SeriviceProviderTest, GetOneServiceInServicesScoped)
{
    collection.addScoped<SimpleClass>();

    auto &service = provider.getRequiredService<SimpleClass>();
    auto services = provider.getServices<SimpleClass>();
    EXPECT_EQ(services.size(), 1);
    EXPECT_EQ(services[0], &service);
}

TEST_F(SeriviceProviderTest, GetOneServiceInServicesTransient)
{
    collection.addTransient<SimpleClass>();

    auto services = provider.createServices<SimpleClass>();
    auto service = provider.createService<SimpleClass>();
    EXPECT_EQ(services.size(), 1);
    EXPECT_NE(services[0], service);
}

TEST_F(SeriviceProviderTest, GetOneServiceInServicesSingeleton)
{
    collection.addSingleton<SimpleClass>();

    auto services = provider.getServices<SimpleClass>();
    auto &service = provider.getRequiredService<SimpleClass>();
    EXPECT_EQ(services.size(), 1);
    EXPECT_EQ(services[0], &service);
}

TEST_F(SeriviceProviderTest, GetLastInheritedService)
{
    collection.addScoped<IInheranceClass, InheranceClassA>();
    collection.addScoped<IInheranceClass, InheranceClassB>();
    collection.addScoped<IInheranceClass, LongInheritanceC>();

    auto &last = provider.getRequiredService<IInheranceClass>();
    auto &last2 = provider.getRequiredService<IInheranceClass>();
    EXPECT_EQ(last.getName(), "C");
    EXPECT_EQ(last2.getName(), "C");
    EXPECT_EQ(&last, &last2);
}

TEST_F(SeriviceProviderTest, GetAllInheritedServices)
{
    collection.addScoped<IInheranceClass, InheranceClassA>();
    collection.addScoped<IInheranceClass, InheranceClassB>();
    collection.addScoped<IInheranceClass, LongInheritanceC>();

    provider.getService<IInheranceClass>();
    auto all = provider.getServices<IInheranceClass>();
    EXPECT_EQ(all.size(), 3);
    EXPECT_EQ(all[0]->getName(), "C");
    EXPECT_EQ(all[1]->getName(), "B");
    EXPECT_EQ(all[2]->getName(), "A");
}

TEST_F(SeriviceProviderTest, GetAllInheritedServicesNotExisting)
{
    auto all = provider.getServices<TransientClass>();
    EXPECT_TRUE(all.empty());
}

TEST_F(SeriviceProviderTest, GetAllInheritedServicesForbidTransient)
{
    collection.addTransient<TransientClass>();

    EXPECT_THROW(provider.getServices<TransientClass>(), sb::TransientForbidException);
}

TEST_F(SeriviceProviderTest, GetAllInheritedServicesInjected)
{
    collection.addScoped<IInheranceClass, InheranceClassA>();
    collection.addScoped<IInheranceClass, InheranceClassB>();
    collection.addScoped<IInheranceClass, LongInheritanceC>();
    collection.addScoped<InheritanceAllRef>();

    provider.getService<IInheranceClass>();
    auto all = provider.getServices<IInheranceClass>();
    auto &ref = provider.getRequiredService<InheritanceAllRef>();
    auto &allRef = ref.inheritances;

    // contains same references
    EXPECT_EQ(all.size(), allRef.size());
    EXPECT_EQ(all[0], allRef[0]);
    EXPECT_EQ(all[1], allRef[1]);
    EXPECT_EQ(all[2], allRef[2]);
}

TEST_F(SeriviceProviderTest, CreateInheritedTransientServices)
{
    collection.addTransient<IInheranceClass, InheranceClassA>();
    collection.addTransient<IInheranceClass, InheranceClassB>();
    collection.addTransient<IInheranceClass, LongInheritanceC>();

    auto last = provider.createService<IInheranceClass>();
    EXPECT_EQ(last->getName(), "C");
}

TEST_F(SeriviceProviderTest, CreateAllInheritedTransientServices)
{
    collection.addTransient<IInheranceClass, InheranceClassA>();
    collection.addTransient<IInheranceClass, InheranceClassB>();
    collection.addTransient<IInheranceClass, LongInheritanceC>();

    provider.createServices<IInheranceClass>();
    auto all = provider.createServices<IInheranceClass>();
    EXPECT_EQ(all.size(), 3);
    EXPECT_EQ(all[0]->getName(), "C");
    EXPECT_EQ(all[1]->getName(), "B");
    EXPECT_EQ(all[2]->getName(), "A");
}

TEST_F(SeriviceProviderTest, CreateAllTransientServicesMustBeUnique)
{
    collection.addTransient<IInheranceClass, InheranceClassA>();
    collection.addTransient<IInheranceClass, InheranceClassB>();
    collection.addTransient<IInheranceClass, LongInheritanceC>();
    collection.addScoped<InheritanceTransientAllRef>();

    provider.createServices<IInheranceClass>();
    auto all = provider.createServices<IInheranceClass>();
    auto &ref = provider.getRequiredService<InheritanceTransientAllRef>();
    auto &allRef = ref.inheritances;

    // contains diff references
    EXPECT_EQ(all.size(), allRef.size());
    EXPECT_NE(all[2], allRef[0]);
    EXPECT_NE(all[1], allRef[1]);
    EXPECT_NE(all[0], allRef[2]);
}

TEST_F(SeriviceProviderTest, MakeScopedProvider)
{
    collection.addSingleton<IInheranceClass, InheranceClassA>();
    collection.addSingleton<IInheranceClass, InheranceClassB>();
    collection.addSingleton<IInheranceClass, LongInheritanceC>();
    collection.addScoped<InheritanceTransientAllRef>();
    collection.addScoped<SimpleClass>();
    collection.addScoped<OneRefClass>();
    collection.addTransient<TransientClass>();

    auto scoped = provider.createScoped();

    scoped.getService<IInheranceClass>();
    provider.getService<IInheranceClass>();
    EXPECT_EQ(scoped.getService<IInheranceClass>(), provider.getService<IInheranceClass>());
    EXPECT_NE(scoped.getService<OneRefClass>(), provider.getService<OneRefClass>());
    EXPECT_NE(scoped.createService<TransientClass>(), provider.createService<TransientClass>());
}