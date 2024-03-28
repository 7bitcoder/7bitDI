#include <gtest/gtest.h>

#include "../../Helpers/Classes/MultiInherit.hpp"
#include "SevenBit/DI/ServiceCollection.hpp"

class AliasKeyedTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    AliasKeyedTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~AliasKeyedTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(AliasKeyedTest, ShouldFailGetServiceDueToAliasMissmatchService)
{
    sb::di::ServiceCollection collection;

    collection.addKeyedSingleton<TestMultiInherit2Class1, TestMultiInheritClass2>("key");
    collection.addKeyedSingleton<TestMultiInherit2Class1, TestMultiInheritClass3>("key");
    collection.addKeyedAlias<TestMultiInherit2Class1, TestMultiInheritClass4>("key", "key");

    EXPECT_THROW(collection.buildServiceProvider(), sb::di::ServiceAliasMismatchException);
}

TEST_F(AliasKeyedTest, ShouldFailGetServiceDueToAliasMissmatchServiceOpposite)
{
    sb::di::ServiceCollection collection;

    collection.addKeyedAlias<TestMultiInherit2Class1, TestMultiInheritClass2>("key", "key");
    collection.addKeyedAlias<TestMultiInherit2Class1, TestMultiInheritClass3>("key", "key");
    collection.addKeyedSingleton<TestMultiInherit2Class1, TestMultiInheritClass4>("key");

    EXPECT_THROW(collection.buildServiceProvider(), sb::di::ServiceAliasMismatchException);
}

TEST_F(AliasKeyedTest, ShouldTryGetAliasesService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInheritClass2>("key")
                        .addKeyedAlias<TestMultiInheritClass1, TestMultiInheritClass2>("key", "key")
                        .addKeyedAlias<TestMultiInherit2Class1, TestMultiInheritClass2>("key", "key")
                        .addKeyedTransient<TestMultiInheritClass4>("key")
                        .addKeyedAlias<TestMultiInheritClass3, TestMultiInheritClass4>("key", "key")
                        .addKeyedAlias<TestMultiInherit4Class4, TestMultiInheritClass4>("key", "key")
                        .buildServiceProvider();

    const auto service1 = provider.tryGetKeyedService<TestMultiInheritClass1>("key");
    const auto service2 = provider.tryGetKeyedService<TestMultiInherit2Class1>("key");

    EXPECT_TRUE(service1);
    EXPECT_TRUE(service2);
    EXPECT_FALSE(provider.tryGetKeyedService<TestMultiInheritClass3>("key"));
    EXPECT_FALSE(provider.tryGetKeyedService<TestMultiInherit4Class4>("key"));
    EXPECT_EQ(service1->first(), 2);
    EXPECT_EQ(service1->arr[0], 1);
    EXPECT_EQ(service1->arr[8], 9);
    EXPECT_EQ(service1->end, 1);
    EXPECT_EQ(service2->second(), 22);
    EXPECT_EQ(service2->arr[0], 1);
    EXPECT_EQ(service2->arr[8], 9);
    EXPECT_EQ(service2->end, 2);
}

TEST_F(AliasKeyedTest, ShouldNotTryGetAliasesService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedAlias<TestMultiInheritClass1, TestMultiInheritClass3>("key", "key")
                        .addKeyedAlias<TestMultiInherit2Class1, TestMultiInheritClass2>("key", "key")
                        .addKeyedAlias<TestMultiInheritClass3, TestMultiInheritClass4>("key", "key")
                        .addKeyedAlias<TestMultiInherit4Class4, TestMultiInheritClass4>("key", "key")
                        .buildServiceProvider();

    EXPECT_FALSE(provider.tryGetKeyedService<TestMultiInheritClass1>("key"));
    EXPECT_FALSE(provider.tryGetKeyedService<TestMultiInherit2Class1>("key"));
    EXPECT_FALSE(provider.tryGetKeyedService<TestMultiInheritClass3>("key"));
    EXPECT_FALSE(provider.tryGetKeyedService<TestMultiInherit4Class4>("key"));
}

TEST_F(AliasKeyedTest, ShouldTryGetOneLastServiceSingleton)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInheritClass2, TestMultiInheritClass3>("key")
                        .addKeyedSingleton<TestMultiInheritClass2, TestMultiInheritClass4>("key")
                        .addKeyedAlias<TestMultiInheritClass1, TestMultiInheritClass2>("key", "key")
                        .buildServiceProvider();

    const auto service = provider.tryGetKeyedService<TestMultiInheritClass1>("key");

    EXPECT_TRUE(service);
    EXPECT_EQ(service->first(), 4);
    EXPECT_EQ(service->arr[0], 1);
    EXPECT_EQ(service->arr[8], 9);
    EXPECT_EQ(service->end, 1);
}

TEST_F(AliasKeyedTest, ShouldTryGetOneLastServiceScoped)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedScoped<TestMultiInheritClass2, TestMultiInheritClass3>("key")
                        .addKeyedScoped<TestMultiInheritClass2, TestMultiInheritClass4>("key")
                        .addKeyedAlias<TestMultiInheritClass1, TestMultiInheritClass2>("key", "key")
                        .buildServiceProvider();

    const auto service = provider.tryGetKeyedService<TestMultiInheritClass1>("key");

    EXPECT_TRUE(service);
    EXPECT_EQ(service->first(), 4);
    EXPECT_EQ(service->arr[0], 1);
    EXPECT_EQ(service->arr[8], 9);
    EXPECT_EQ(service->end, 1);
}

TEST_F(AliasKeyedTest, ShouldGetAliasesService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInheritClass2>("key")
                        .addKeyedAlias<TestMultiInheritClass1, TestMultiInheritClass2>("key", "key")
                        .addKeyedAlias<TestMultiInherit2Class1, TestMultiInheritClass2>("key", "key")
                        .addKeyedTransient<TestMultiInheritClass4>("key")
                        .addKeyedAlias<TestMultiInheritClass3, TestMultiInheritClass4>("key", "key")
                        .addKeyedAlias<TestMultiInherit4Class4, TestMultiInheritClass4>("key", "key")
                        .buildServiceProvider();

    auto &service1 = provider.getKeyedService<TestMultiInheritClass1>("key");
    auto &service2 = provider.getKeyedService<TestMultiInherit2Class1>("key");

    EXPECT_THROW(provider.getKeyedService<TestMultiInheritClass3>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.getKeyedService<TestMultiInherit4Class4>("key"), sb::di::ServiceNotFoundException);
    EXPECT_EQ(service1.first(), 2);
    EXPECT_EQ(service1.arr[0], 1);
    EXPECT_EQ(service1.arr[8], 9);
    EXPECT_EQ(service1.end, 1);
    EXPECT_EQ(service2.second(), 22);
    EXPECT_EQ(service2.arr[0], 1);
    EXPECT_EQ(service2.arr[8], 9);
    EXPECT_EQ(service2.end, 2);
}

TEST_F(AliasKeyedTest, ShouldNotGetAliasesService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedAlias<TestMultiInheritClass1, TestMultiInheritClass3>("key", "key")
                        .addKeyedAlias<TestMultiInherit2Class1, TestMultiInheritClass2>("key", "key")
                        .addKeyedAlias<TestMultiInheritClass3, TestMultiInheritClass4>("key", "key")
                        .addKeyedAlias<TestMultiInherit4Class4, TestMultiInheritClass4>("key", "key")
                        .buildServiceProvider();

    EXPECT_THROW(provider.getKeyedService<TestMultiInheritClass1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.getKeyedService<TestMultiInherit2Class1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.getKeyedService<TestMultiInheritClass3>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.getKeyedService<TestMultiInherit4Class4>("key"), sb::di::ServiceNotFoundException);
}

TEST_F(AliasKeyedTest, ShouldGetOneLastServiceSingleton)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInheritClass2, TestMultiInheritClass3>("key")
                        .addKeyedSingleton<TestMultiInheritClass2, TestMultiInheritClass4>("key")
                        .addKeyedAlias<TestMultiInheritClass1, TestMultiInheritClass2>("key", "key")
                        .buildServiceProvider();

    auto &service = provider.getKeyedService<TestMultiInheritClass1>("key");

    EXPECT_EQ(service.first(), 4);
    EXPECT_EQ(service.arr[0], 1);
    EXPECT_EQ(service.arr[8], 9);
    EXPECT_EQ(service.end, 1);
}

TEST_F(AliasKeyedTest, ShouldGetOneLastServiceScoped)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedScoped<TestMultiInheritClass2, TestMultiInheritClass3>("key")
                        .addKeyedScoped<TestMultiInheritClass2, TestMultiInheritClass4>("key")
                        .addKeyedAlias<TestMultiInheritClass1, TestMultiInheritClass2>("key", "key")
                        .buildServiceProvider();

    auto &service = provider.getKeyedService<TestMultiInheritClass1>("key");

    EXPECT_EQ(service.first(), 4);
    EXPECT_EQ(service.arr[0], 1);
    EXPECT_EQ(service.arr[8], 9);
    EXPECT_EQ(service.end, 1);
}

TEST_F(AliasKeyedTest, ShouldGetServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInheritClass2>("key")
                        .addKeyedAlias<TestMultiInheritClass1, TestMultiInheritClass2>("key", "key")
                        .addKeyedAlias<TestMultiInherit2Class1, TestMultiInheritClass2>("key", "key")
                        .addKeyedTransient<TestMultiInheritClass4>("key")
                        .addKeyedAlias<TestMultiInheritClass3, TestMultiInheritClass4>("key", "key")
                        .addKeyedAlias<TestMultiInherit4Class4, TestMultiInheritClass4>("key", "key")
                        .buildServiceProvider();

    auto services = provider.getKeyedServices<TestMultiInheritClass1>("key");

    EXPECT_EQ(provider.getKeyedServices<TestMultiInheritClass1>("key").size(), 1);
    EXPECT_EQ(provider.getKeyedServices<TestMultiInherit2Class1>("key").size(), 1);
    EXPECT_TRUE(provider.getKeyedServices<TestMultiInheritClass3>("key").empty());
    EXPECT_TRUE(provider.getKeyedServices<TestMultiInherit4Class4>("key").empty());
}

TEST_F(AliasKeyedTest, ShoulNotGetServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedAlias<TestMultiInheritClass1, TestMultiInheritClass3>("key", "key")
                        .addKeyedAlias<TestMultiInherit2Class1, TestMultiInheritClass2>("key", "key")
                        .addKeyedAlias<TestMultiInheritClass3, TestMultiInheritClass4>("key", "key")
                        .addKeyedAlias<TestMultiInherit4Class4, TestMultiInheritClass4>("key", "key")
                        .buildServiceProvider();

    auto services = provider.getKeyedServices<TestMultiInheritClass1>("key");

    EXPECT_TRUE(provider.getKeyedServices<TestMultiInheritClass1>("key").empty());
    EXPECT_TRUE(provider.getKeyedServices<TestMultiInherit2Class1>("key").empty());
    EXPECT_TRUE(provider.getKeyedServices<TestMultiInheritClass3>("key").empty());
    EXPECT_TRUE(provider.getKeyedServices<TestMultiInherit4Class4>("key").empty());
}

TEST_F(AliasKeyedTest, ShouldGetCastedServicesInOrder)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedScoped<TestMultiInheritClass2, TestMultiInheritClass3>("key")
                        .addKeyedScoped<TestMultiInheritClass2, TestMultiInheritClass4>("key")
                        .addKeyedAlias<TestMultiInheritClass1, TestMultiInheritClass2>("key", "key")
                        .buildServiceProvider();

    const auto services = provider.getKeyedServices<TestMultiInheritClass1>("key");

    EXPECT_EQ(services.size(), 2);
    EXPECT_EQ(services[0]->first(), 3);
    EXPECT_EQ(services[1]->first(), 4);
}

TEST_F(AliasKeyedTest, ShouldGetServicesInOrderAfterNormalGet)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedScoped<TestMultiInheritClass2, TestMultiInheritClass3>("key")
                        .addKeyedScoped<TestMultiInheritClass2, TestMultiInheritClass4>("key")
                        .addKeyedAlias<TestMultiInheritClass1, TestMultiInheritClass2>("key", "key")
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetKeyedService<TestMultiInheritClass1>("key"));
    const auto services = provider.getKeyedServices<TestMultiInheritClass1>("key");
    EXPECT_EQ(services.size(), 2);
    EXPECT_EQ(services[0]->first(), 3);
    EXPECT_EQ(services[1]->first(), 4);
}

TEST_F(AliasKeyedTest, ShouldTryCreateService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInheritClass2>("key")
                        .addKeyedAlias<TestMultiInheritClass1, TestMultiInheritClass2>("key", "key")
                        .addKeyedAlias<TestMultiInherit2Class1, TestMultiInheritClass2>("key", "key")
                        .addKeyedTransient<TestMultiInheritClass4>("key")
                        .addKeyedAlias<TestMultiInheritClass3, TestMultiInheritClass4>("key", "key")
                        .addKeyedAlias<TestMultiInherit4Class4, TestMultiInheritClass4>("key", "key")
                        .buildServiceProvider();

    const auto service3 = provider.tryCreateKeyedService<TestMultiInheritClass3>("key");
    const auto service4 = provider.tryCreateKeyedService<TestMultiInherit4Class4>("key");

    EXPECT_TRUE(service3);
    EXPECT_TRUE(service4);
    EXPECT_FALSE(provider.tryCreateKeyedService<TestMultiInheritClass1>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestMultiInherit2Class1>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestMultiInheritClass2>("key"));
}

TEST_F(AliasKeyedTest, ShouldNotTryCreateService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedAlias<TestMultiInheritClass1, TestMultiInheritClass2>("key", "key")
                        .addKeyedAlias<TestMultiInherit2Class1, TestMultiInheritClass2>("key", "key")
                        .addKeyedAlias<TestMultiInheritClass3, TestMultiInheritClass4>("key", "key")
                        .addKeyedAlias<TestMultiInherit4Class4, TestMultiInheritClass4>("key", "key")
                        .buildServiceProvider();

    EXPECT_FALSE(provider.tryCreateKeyedService<TestMultiInheritClass3>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestMultiInherit4Class4>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestMultiInheritClass1>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestMultiInherit2Class1>("key"));
    EXPECT_FALSE(provider.tryCreateKeyedService<TestMultiInheritClass2>("key"));
}

TEST_F(AliasKeyedTest, ShouldCreateService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInheritClass2>("key")
                        .addKeyedAlias<TestMultiInheritClass1, TestMultiInheritClass2>("key", "key")
                        .addKeyedAlias<TestMultiInherit2Class1, TestMultiInheritClass2>("key", "key")
                        .addKeyedTransient<TestMultiInheritClass4>("key")
                        .addKeyedAlias<TestMultiInheritClass3, TestMultiInheritClass4>("key", "key")
                        .addKeyedAlias<TestMultiInherit4Class4, TestMultiInheritClass4>("key", "key")
                        .buildServiceProvider();

    const auto service3 = provider.createKeyedService<TestMultiInheritClass3>("key");
    const auto service4 = provider.createKeyedService<TestMultiInherit4Class4>("key");

    EXPECT_TRUE(service3);
    EXPECT_TRUE(service4);
    EXPECT_THROW(provider.createKeyedService<TestMultiInheritClass1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedService<TestMultiInherit2Class1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedService<TestMultiInheritClass2>("key"), sb::di::ServiceNotFoundException);
}

TEST_F(AliasKeyedTest, ShouldNotCreateService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedAlias<TestMultiInheritClass1, TestMultiInheritClass2>("key", "key")
                        .addKeyedAlias<TestMultiInherit2Class1, TestMultiInheritClass2>("key", "key")
                        .addKeyedAlias<TestMultiInheritClass3, TestMultiInheritClass4>("key", "key")
                        .addKeyedAlias<TestMultiInherit4Class4, TestMultiInheritClass4>("key", "key")
                        .buildServiceProvider();

    EXPECT_THROW(provider.createKeyedService<TestMultiInheritClass3>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedService<TestMultiInherit4Class4>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedService<TestMultiInheritClass1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedService<TestMultiInherit2Class1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedService<TestMultiInheritClass2>("key"), sb::di::ServiceNotFoundException);
}

TEST_F(AliasKeyedTest, ShouldNotCreateServiceInPlace)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInheritClass2>("key")
                        .addKeyedAlias<TestMultiInheritClass1, TestMultiInheritClass2>("key", "key")
                        .addKeyedAlias<TestMultiInherit2Class1, TestMultiInheritClass2>("key", "key")
                        .addKeyedTransient<TestMultiInheritClass4>("key")
                        .addKeyedAlias<TestMultiInheritClass3, TestMultiInheritClass4>("key", "key")
                        .addKeyedAlias<TestMultiInherit4Class4, TestMultiInheritClass4>("key", "key")
                        .buildServiceProvider();

    EXPECT_THROW(provider.createKeyedServiceInPlace<TestMultiInheritClass2>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestMultiInherit3Class1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestMultiInheritClass1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestMultiInherit2Class1>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestMultiInheritClass3>("key"), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createKeyedServiceInPlace<TestMultiInherit4Class4>("key"), sb::di::ServiceNotFoundException);
}

TEST_F(AliasKeyedTest, ShouldCreateServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedSingleton<TestMultiInheritClass2>("key")
                        .addKeyedAlias<TestMultiInheritClass1, TestMultiInheritClass2>("key", "key")
                        .addKeyedAlias<TestMultiInherit2Class1, TestMultiInheritClass2>("key", "key")
                        .addKeyedTransient<TestMultiInheritClass4>("key")
                        .addKeyedAlias<TestMultiInheritClass3, TestMultiInheritClass4>("key", "key")
                        .addKeyedAlias<TestMultiInherit4Class4, TestMultiInheritClass4>("key", "key")
                        .buildServiceProvider();

    auto services = provider.getKeyedServices<TestMultiInheritClass1>("key");

    EXPECT_EQ(provider.createKeyedServices<TestMultiInheritClass3>("key").size(), 1);
    EXPECT_EQ(provider.createKeyedServices<TestMultiInherit4Class4>("key").size(), 1);
    EXPECT_TRUE(provider.createKeyedServices<TestMultiInheritClass1>("key").empty());
    EXPECT_TRUE(provider.createKeyedServices<TestMultiInherit2Class1>("key").empty());
}

TEST_F(AliasKeyedTest, ShoulNotCreateServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedAlias<TestMultiInheritClass1, TestMultiInheritClass3>("key", "key")
                        .addKeyedAlias<TestMultiInherit2Class1, TestMultiInheritClass2>("key", "key")
                        .addKeyedAlias<TestMultiInheritClass3, TestMultiInheritClass4>("key", "key")
                        .addKeyedAlias<TestMultiInherit4Class4, TestMultiInheritClass4>("key", "key")
                        .buildServiceProvider();

    EXPECT_TRUE(provider.createKeyedServices<TestMultiInheritClass1>("key").empty());
    EXPECT_TRUE(provider.createKeyedServices<TestMultiInherit2Class1>("key").empty());
    EXPECT_TRUE(provider.createKeyedServices<TestMultiInheritClass3>("key").empty());
    EXPECT_TRUE(provider.createKeyedServices<TestMultiInherit4Class4>("key").empty());
}

TEST_F(AliasKeyedTest, ShouldCreateCastedServicesInOrder)
{
    auto provider = sb::di::ServiceCollection{}
                        .addKeyedTransient<TestMultiInheritClass2, TestMultiInheritClass3>("key")
                        .addKeyedTransient<TestMultiInheritClass2, TestMultiInheritClass4>("key")
                        .addKeyedAlias<TestMultiInheritClass1, TestMultiInheritClass2>("key", "key")
                        .buildServiceProvider();

    const auto services = provider.createKeyedServices<TestMultiInheritClass1>("key");

    EXPECT_EQ(services.size(), 2);
    EXPECT_EQ(services[0]->first(), 3);
    EXPECT_EQ(services[1]->first(), 4);
}
