#include <algorithm>
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <thread>

#include "SevenBit/ServiceCollection.hpp"
#include "SevenBit/ServiceProvider.hpp"

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
        SimpleClass *_a;

        OneRefClass(SimpleClass *a) { _a = a; }
    };

    struct TransientClass
    {
        int ff = 100;
        TransientClass() {}
    };

    struct IInheranceClass
    {
        virtual int get() = 0;
        virtual SimpleClass *getSimple() = 0;
        virtual const OneRefClass *getOneRef() = 0;
        virtual ~IInheranceClass() {}
    };

    struct InheranceClassA final : public IInheranceClass
    {
        int ff = 100;
        SimpleClass *_a;
        const OneRefClass &_b;

        InheranceClassA(SimpleClass *a, OneRefClass *b) : _b(*b) { _a = a; }

        int get() final { return ff; }
        SimpleClass *getSimple() { return _a; }
        const OneRefClass *getOneRef() { return &_b; }
    };

    struct InheranceClassB : public IInheranceClass
    {
        int ff = 100;
        SimpleClass *_a;
        const OneRefClass &_b;

        InheranceClassB(SimpleClass *a, OneRefClass *b) : _b(*b) { _a = a; }

        int get() final { return ff; }
        SimpleClass *getSimple() { return _a; }
        const OneRefClass *getOneRef() { return &_b; }
    };

    struct LongInheritanceC : public InheranceClassB
    {
        LongInheritanceC(SimpleClass *a, OneRefClass *b) : InheranceClassB(a, b) {}
    };

} // namespace

class SeriviceCollectionTest : public ::testing::Test
{
  protected:
    sb::ServiceCollection collection;

    static void SetUpTestSuite() {}

    SeriviceCollectionTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~SeriviceCollectionTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(SeriviceCollectionTest, AddService) { EXPECT_NO_THROW((collection.addSingleton<SimpleClass>())); }

TEST_F(SeriviceCollectionTest, AddMultipleServices)
{
    collection.addSingleton<SimpleClass>();
    collection.addScoped<OneRefClass>();
    collection.addTransient<TransientClass>();

    EXPECT_TRUE(collection.contains<SimpleClass>());
    EXPECT_TRUE(collection.contains<OneRefClass>());
    EXPECT_TRUE(collection.contains<TransientClass>());
}

TEST_F(SeriviceCollectionTest, AddInheritedServices)
{
    collection.addScoped<IInheranceClass, InheranceClassA>();
    collection.addScoped<IInheranceClass, InheranceClassB>();
    collection.addScoped<IInheranceClass, LongInheritanceC>();

    EXPECT_TRUE(collection.contains<IInheranceClass>());
}

TEST_F(SeriviceCollectionTest, AddFailDueToAlreadyRegisteredService)
{
    // collection.addScoped<IInheranceClass, InheranceClassA>();
    // collection.addScoped<IInheranceClass, InheranceClassB>();

    // EXPECT_ANY_THROW((collection.addScoped<IInheranceClass, InheranceClassB>()));
}

TEST_F(SeriviceCollectionTest, AddFailDueToScopeMissmatchService)
{
    // collection.addScoped<IInheranceClass, InheranceClassA>();
    // collection.addScoped<IInheranceClass, InheranceClassB>();

    // EXPECT_THROW((collection.addSingleton<IInheranceClass, LongInheritanceC>()), sb::ServiceScopeMissmatchException);
}

TEST_F(SeriviceCollectionTest, RemoveMultipleServices)
{
    collection.addSingleton<SimpleClass>();
    collection.addScoped<OneRefClass>();
    collection.addTransient<TransientClass>();

    collection.removeAll<OneRefClass>();
    collection.removeAll<SimpleClass>();

    EXPECT_FALSE(collection.contains<SimpleClass>());
    EXPECT_FALSE(collection.contains<OneRefClass>());
    EXPECT_TRUE(collection.contains<TransientClass>());
}

TEST_F(SeriviceCollectionTest, ShouldNotCompileDueToNotInheritedServices)
{
    // collection.addTransient<OneRefClass, TransientClass>();
}

TEST_F(SeriviceCollectionTest, AddServiceFactory) { EXPECT_NO_THROW((collection.addSingleton<SimpleClass>())); }

TEST_F(SeriviceCollectionTest, AddMultipleServicesFactory)
{
    collection.addSingleton([](auto &sp) { return std::make_unique<SimpleClass>(); });
    collection.addScoped([](auto &sp) { return std::make_unique<OneRefClass>(nullptr); });
    collection.addTransient([](auto &sp) { return std::make_unique<TransientClass>(); });

    EXPECT_TRUE(collection.contains<SimpleClass>());
    EXPECT_TRUE(collection.contains<OneRefClass>());
    EXPECT_TRUE(collection.contains<TransientClass>());
}

TEST_F(SeriviceCollectionTest, AddInheritedServicesFactory)
{
    collection.addScoped<IInheranceClass>(
        [](sb::IServiceProvider &provider) { return std::make_unique<InheranceClassA>(nullptr, nullptr); });
    collection.addScoped<IInheranceClass>(
        [](sb::IServiceProvider &provider) { return std::make_unique<InheranceClassB>(nullptr, nullptr); });
    collection.addScoped<IInheranceClass>(
        [](sb::IServiceProvider &provider) { return std::make_unique<LongInheritanceC>(nullptr, nullptr); });

    EXPECT_TRUE(collection.contains<IInheranceClass>());
}

TEST_F(SeriviceCollectionTest, AddFailDueToAlreadyRegisteredServiceFactory)
{

    // collection.addScoped<IInheranceClass, InheranceClassA>(
    //     [](sb::IServiceProvider &provider) { return std::make_unique<InheranceClassA>(nullptr, nullptr); });
    // collection.addScoped<IInheranceClass, InheranceClassB>(
    //     [](sb::IServiceProvider &provider) { return std::make_unique<InheranceClassB>(nullptr, nullptr); });

    // EXPECT_ANY_THROW((collection.addSingleton<IInheranceClass, InheranceClassB>(
    //     [](sb::IServiceProvider &provider) { return std::make_unique<InheranceClassB>(nullptr, nullptr); })));
}

TEST_F(SeriviceCollectionTest, AddFailDueToScopeMissmatchServiceFactory)
{
    // collection.addScoped<IInheranceClass, InheranceClassA>(
    //     [](sb::IServiceProvider &provider) { return std::make_unique<InheranceClassA>(nullptr, nullptr); });
    // collection.addScoped<IInheranceClass, InheranceClassB>(
    //     [](sb::IServiceProvider &provider) { return std::make_unique<InheranceClassB>(nullptr, nullptr); });

    // EXPECT_THROW((collection.addSingleton<IInheranceClass, LongInheritanceC>([](sb::ServiceProvider &provider) {
    //                  return std::make_unique<LongInheritanceC>(nullptr, nullptr);
    //              })),
    //              sb::ServiceScopeMissmatchException);
}

TEST_F(SeriviceCollectionTest, RemoveMultipleServicesFactory)
{
    // collection.addSingleton<SimpleClass>([](sb::ServiceProvider &provider) { return std::make_unique<SimpleClass>();
    // }); collection.addScoped<OneRefClass>(
    //     [](sb::ServiceProvider &provider) { return std::make_unique<OneRefClass>(nullptr); });
    // collection.addTransient<TransientClass>(
    //     [](sb::ServiceProvider &provider) { return std::make_unique<TransientClass>(); });

    // collection.removeAll<OneRefClass>();
    // collection.removeAll<SimpleClass>();

    // EXPECT_FALSE(collection.contains<SimpleClass>());
    // EXPECT_FALSE(collection.contains<OneRefClass>());
    // EXPECT_TRUE(collection.contains<TransientClass>());
}