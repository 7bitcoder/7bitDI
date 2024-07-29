#include <gtest/gtest.h>

#include "../Helpers/Classes/MultiInherit.hpp"
#include <SevenBit/DI/ServiceCollection.hpp>

class AliasTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    AliasTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~AliasTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(AliasTest, ShouldFailGetServiceDueToAliasMissmatchService)
{
    sb::di::ServiceCollection collection;

    collection.addSingleton<TestMultiInherit2Class1, TestMultiInheritClass2>();
    collection.addSingleton<TestMultiInherit2Class1, TestMultiInheritClass3>();
    collection.addAlias<TestMultiInherit2Class1, TestMultiInheritClass4>();

    EXPECT_THROW(collection.buildServiceProvider(), sb::di::ServiceRegisterException);
}

TEST_F(AliasTest, ShouldFailGetServiceDueToAliasMissmatchServiceOpposite)
{
    sb::di::ServiceCollection collection;

    collection.addAlias<TestMultiInherit2Class1, TestMultiInheritClass2>();
    collection.addAlias<TestMultiInherit2Class1, TestMultiInheritClass3>();
    collection.addSingleton<TestMultiInherit2Class1, TestMultiInheritClass4>();

    EXPECT_THROW(collection.buildServiceProvider(), sb::di::ServiceRegisterException);
}

TEST_F(AliasTest, ShouldTryGetAliasesService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass2>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass2>()
                        .addAlias<TestMultiInherit2Class1, TestMultiInheritClass2>()
                        .addTransient<TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass3, TestMultiInheritClass4>()
                        .addAlias<TestMultiInherit4Class4, TestMultiInheritClass4>()
                        .buildServiceProvider();

    const auto service1 = provider.tryGetService<TestMultiInheritClass1>();
    const auto service2 = provider.tryGetService<TestMultiInherit2Class1>();

    EXPECT_TRUE(service1);
    EXPECT_TRUE(service2);
    EXPECT_FALSE(provider.tryGetService<TestMultiInheritClass3>());
    EXPECT_FALSE(provider.tryGetService<TestMultiInherit4Class4>());
    EXPECT_EQ(service1->first(), 2);
    EXPECT_EQ(service1->arr[0], 1);
    EXPECT_EQ(service1->arr[8], 9);
    EXPECT_EQ(service1->end, 1);
    EXPECT_EQ(service2->second(), 22);
    EXPECT_EQ(service2->arr[0], 1);
    EXPECT_EQ(service2->arr[8], 9);
    EXPECT_EQ(service2->end, 2);
}

TEST_F(AliasTest, ShouldNotTryGetAliasesService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass3>()
                        .addAlias<TestMultiInherit2Class1, TestMultiInheritClass2>()
                        .addAlias<TestMultiInheritClass3, TestMultiInheritClass4>()
                        .addAlias<TestMultiInherit4Class4, TestMultiInheritClass4>()
                        .buildServiceProvider();

    EXPECT_FALSE(provider.tryGetService<TestMultiInheritClass1>());
    EXPECT_FALSE(provider.tryGetService<TestMultiInherit2Class1>());
    EXPECT_FALSE(provider.tryGetService<TestMultiInheritClass3>());
    EXPECT_FALSE(provider.tryGetService<TestMultiInherit4Class4>());
}

TEST_F(AliasTest, ShouldTryGetOneLastServiceSingleton)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass2, TestMultiInheritClass3>()
                        .addSingleton<TestMultiInheritClass2, TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass2>()
                        .buildServiceProvider();

    const auto service = provider.tryGetService<TestMultiInheritClass1>();

    EXPECT_TRUE(service);
    EXPECT_EQ(service->first(), 4);
    EXPECT_EQ(service->arr[0], 1);
    EXPECT_EQ(service->arr[8], 9);
    EXPECT_EQ(service->end, 1);
}

TEST_F(AliasTest, ShouldTryGetOneLastServiceScoped)
{
    auto provider = sb::di::ServiceCollection{}
                        .addScoped<TestMultiInheritClass2, TestMultiInheritClass3>()
                        .addScoped<TestMultiInheritClass2, TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass2>()
                        .buildServiceProvider();

    const auto service = provider.tryGetService<TestMultiInheritClass1>();

    EXPECT_TRUE(service);
    EXPECT_EQ(service->first(), 4);
    EXPECT_EQ(service->arr[0], 1);
    EXPECT_EQ(service->arr[8], 9);
    EXPECT_EQ(service->end, 1);
}

TEST_F(AliasTest, ShouldGetAliasesService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass2>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass2>()
                        .addAlias<TestMultiInherit2Class1, TestMultiInheritClass2>()
                        .addTransient<TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass3, TestMultiInheritClass4>()
                        .addAlias<TestMultiInherit4Class4, TestMultiInheritClass4>()
                        .buildServiceProvider();

    auto &service1 = provider.getService<TestMultiInheritClass1>();
    auto &service2 = provider.getService<TestMultiInherit2Class1>();

    EXPECT_THROW(provider.getService<TestMultiInheritClass3>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.getService<TestMultiInherit4Class4>(), sb::di::ServiceNotFoundException);
    EXPECT_EQ(service1.first(), 2);
    EXPECT_EQ(service1.arr[0], 1);
    EXPECT_EQ(service1.arr[8], 9);
    EXPECT_EQ(service1.end, 1);
    EXPECT_EQ(service2.second(), 22);
    EXPECT_EQ(service2.arr[0], 1);
    EXPECT_EQ(service2.arr[8], 9);
    EXPECT_EQ(service2.end, 2);
}

TEST_F(AliasTest, ShouldNotGetAliasesService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass3>()
                        .addAlias<TestMultiInherit2Class1, TestMultiInheritClass2>()
                        .addAlias<TestMultiInheritClass3, TestMultiInheritClass4>()
                        .addAlias<TestMultiInherit4Class4, TestMultiInheritClass4>()
                        .buildServiceProvider();

    EXPECT_THROW(provider.getService<TestMultiInheritClass1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.getService<TestMultiInherit2Class1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.getService<TestMultiInheritClass3>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.getService<TestMultiInherit4Class4>(), sb::di::ServiceNotFoundException);
}

TEST_F(AliasTest, ShouldGetOneLastServiceSingleton)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass2, TestMultiInheritClass3>()
                        .addSingleton<TestMultiInheritClass2, TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass2>()
                        .buildServiceProvider();

    auto &service = provider.getService<TestMultiInheritClass1>();

    EXPECT_EQ(service.first(), 4);
    EXPECT_EQ(service.arr[0], 1);
    EXPECT_EQ(service.arr[8], 9);
    EXPECT_EQ(service.end, 1);
}

TEST_F(AliasTest, ShouldGetOneLastServiceScoped)
{
    auto provider = sb::di::ServiceCollection{}
                        .addScoped<TestMultiInheritClass2, TestMultiInheritClass3>()
                        .addScoped<TestMultiInheritClass2, TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass2>()
                        .buildServiceProvider();

    auto &service = provider.getService<TestMultiInheritClass1>();

    EXPECT_EQ(service.first(), 4);
    EXPECT_EQ(service.arr[0], 1);
    EXPECT_EQ(service.arr[8], 9);
    EXPECT_EQ(service.end, 1);
}

TEST_F(AliasTest, ShouldGetServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass2>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass2>()
                        .addAlias<TestMultiInherit2Class1, TestMultiInheritClass2>()
                        .addTransient<TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass3, TestMultiInheritClass4>()
                        .addAlias<TestMultiInherit4Class4, TestMultiInheritClass4>()
                        .buildServiceProvider();

    auto services = provider.getServices<TestMultiInheritClass1>();

    EXPECT_EQ(provider.getServices<TestMultiInheritClass1>().size(), 1);
    EXPECT_EQ(provider.getServices<TestMultiInherit2Class1>().size(), 1);
    EXPECT_TRUE(provider.getServices<TestMultiInheritClass3>().empty());
    EXPECT_TRUE(provider.getServices<TestMultiInherit4Class4>().empty());
}

TEST_F(AliasTest, ShouldGetMultipleServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass3>()
                        .addSingleton<TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass2, TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass3>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass2>()
                        .buildServiceProvider();

    const auto services = provider.getServices<TestMultiInheritClass1>();

    EXPECT_EQ(services.size(), 3);
    EXPECT_EQ(services[0]->first(), 4);
    EXPECT_EQ(services[1]->first(), 3);
    EXPECT_EQ(services[2]->first(), 4);
    EXPECT_EQ(services[2], services[0]);
}

TEST_F(AliasTest, ShouldGetMultipleSomeWrongServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass2, TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass3>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass2>()
                        .buildServiceProvider();

    const auto services = provider.getServices<TestMultiInheritClass1>();

    EXPECT_EQ(services.size(), 2);
    EXPECT_EQ(services[0]->first(), 4);
    EXPECT_EQ(services[1]->first(), 4);
}

TEST_F(AliasTest, ShoulNotGetServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass3>()
                        .addAlias<TestMultiInherit2Class1, TestMultiInheritClass2>()
                        .addAlias<TestMultiInheritClass3, TestMultiInheritClass4>()
                        .addAlias<TestMultiInherit4Class4, TestMultiInheritClass4>()
                        .buildServiceProvider();

    auto services = provider.getServices<TestMultiInheritClass1>();

    EXPECT_TRUE(provider.getServices<TestMultiInheritClass1>().empty());
    EXPECT_TRUE(provider.getServices<TestMultiInherit2Class1>().empty());
    EXPECT_TRUE(provider.getServices<TestMultiInheritClass3>().empty());
    EXPECT_TRUE(provider.getServices<TestMultiInherit4Class4>().empty());
}

TEST_F(AliasTest, ShouldGetCastedServicesInOrder)
{
    auto provider = sb::di::ServiceCollection{}
                        .addScoped<TestMultiInheritClass2, TestMultiInheritClass3>()
                        .addScoped<TestMultiInheritClass2, TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass2>()
                        .buildServiceProvider();

    const auto services = provider.getServices<TestMultiInheritClass1>();

    EXPECT_EQ(services.size(), 2);
    EXPECT_EQ(services[0]->first(), 3);
    EXPECT_EQ(services[1]->first(), 4);
}

TEST_F(AliasTest, ShouldGetCastedMultipleServicesInOrder)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass4>()
                        .addSingleton<TestMultiInheritClass3>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass3>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass2>()
                        .buildServiceProvider();

    const auto services = provider.getServices<TestMultiInheritClass1>();

    EXPECT_EQ(services.size(), 2);
    EXPECT_EQ(services[0]->first(), 4);
    EXPECT_EQ(services[1]->first(), 3);
}

TEST_F(AliasTest, ShouldGetServicesInOrderAfterNormalGet)
{
    auto provider = sb::di::ServiceCollection{}
                        .addScoped<TestMultiInheritClass2, TestMultiInheritClass3>()
                        .addScoped<TestMultiInheritClass2, TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass2>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetService<TestMultiInheritClass1>());

    const auto services = provider.getServices<TestMultiInheritClass1>();

    EXPECT_EQ(services.size(), 2);
    EXPECT_EQ(services[0]->first(), 3);
    EXPECT_EQ(services[1]->first(), 4);
}

TEST_F(AliasTest, ShouldGetMultipleServicesInOrderAfterNormalGet)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass4>()
                        .addSingleton<TestMultiInheritClass3>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass2>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass3>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetService<TestMultiInheritClass1>());

    const auto services = provider.getServices<TestMultiInheritClass1>();

    EXPECT_EQ(services.size(), 2);
    EXPECT_EQ(services[0]->first(), 4);
    EXPECT_EQ(services[1]->first(), 3);
}

TEST_F(AliasTest, ShouldGetNotMultipleServicesInOrderAfterNormalGet)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass4>()
                        .addSingleton<TestMultiInheritClass3>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass3>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass2>()
                        .buildServiceProvider();

    EXPECT_FALSE(provider.tryGetService<TestMultiInheritClass1>());

    const auto services = provider.getServices<TestMultiInheritClass1>();

    EXPECT_EQ(services.size(), 2);
    EXPECT_EQ(services[0]->first(), 4);
    EXPECT_EQ(services[1]->first(), 3);
}

TEST_F(AliasTest, ShouldTryCreateService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass2>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass2>()
                        .addAlias<TestMultiInherit2Class1, TestMultiInheritClass2>()
                        .addTransient<TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass3, TestMultiInheritClass4>()
                        .addAlias<TestMultiInherit4Class4, TestMultiInheritClass4>()
                        .buildServiceProvider();

    const auto service3 = provider.tryCreateService<TestMultiInheritClass3>();
    const auto service4 = provider.tryCreateService<TestMultiInherit4Class4>();

    EXPECT_TRUE(service3);
    EXPECT_TRUE(service4);
    EXPECT_FALSE(provider.tryCreateService<TestMultiInheritClass1>());
    EXPECT_FALSE(provider.tryCreateService<TestMultiInherit2Class1>());
    EXPECT_FALSE(provider.tryCreateService<TestMultiInheritClass2>());
}

TEST_F(AliasTest, ShouldNotTryCreateService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass2>()
                        .addAlias<TestMultiInherit2Class1, TestMultiInheritClass2>()
                        .addAlias<TestMultiInheritClass3, TestMultiInheritClass4>()
                        .addAlias<TestMultiInherit4Class4, TestMultiInheritClass4>()
                        .buildServiceProvider();

    EXPECT_FALSE(provider.tryCreateService<TestMultiInheritClass3>());
    EXPECT_FALSE(provider.tryCreateService<TestMultiInherit4Class4>());
    EXPECT_FALSE(provider.tryCreateService<TestMultiInheritClass1>());
    EXPECT_FALSE(provider.tryCreateService<TestMultiInherit2Class1>());
    EXPECT_FALSE(provider.tryCreateService<TestMultiInheritClass2>());
}

TEST_F(AliasTest, ShouldCreateService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass2>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass2>()
                        .addAlias<TestMultiInherit2Class1, TestMultiInheritClass2>()
                        .addTransient<TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass3, TestMultiInheritClass4>()
                        .addAlias<TestMultiInherit4Class4, TestMultiInheritClass4>()
                        .buildServiceProvider();

    const auto service3 = provider.createService<TestMultiInheritClass3>();
    const auto service4 = provider.createService<TestMultiInherit4Class4>();

    EXPECT_TRUE(service3);
    EXPECT_TRUE(service4);
    EXPECT_THROW(provider.createService<TestMultiInheritClass1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createService<TestMultiInherit2Class1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createService<TestMultiInheritClass2>(), sb::di::ServiceNotFoundException);
}

TEST_F(AliasTest, ShouldNotCreateService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass2>()
                        .addAlias<TestMultiInherit2Class1, TestMultiInheritClass2>()
                        .addAlias<TestMultiInheritClass3, TestMultiInheritClass4>()
                        .addAlias<TestMultiInherit4Class4, TestMultiInheritClass4>()
                        .buildServiceProvider();

    EXPECT_THROW(provider.createService<TestMultiInheritClass3>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createService<TestMultiInherit4Class4>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createService<TestMultiInheritClass1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createService<TestMultiInherit2Class1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createService<TestMultiInheritClass2>(), sb::di::ServiceNotFoundException);
}

TEST_F(AliasTest, ShouldNotCreateServiceInPlace)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass2>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass2>()
                        .addAlias<TestMultiInherit2Class1, TestMultiInheritClass2>()
                        .addTransient<TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass3, TestMultiInheritClass4>()
                        .addAlias<TestMultiInherit4Class4, TestMultiInheritClass4>()
                        .buildServiceProvider();

    EXPECT_THROW(provider.createServiceInPlace<TestMultiInheritClass2>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestMultiInherit3Class1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestMultiInheritClass1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestMultiInherit2Class1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestMultiInheritClass3>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestMultiInherit4Class4>(), sb::di::ServiceNotFoundException);
}

TEST_F(AliasTest, ShouldCreateServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass2>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass2>()
                        .addAlias<TestMultiInherit2Class1, TestMultiInheritClass2>()
                        .addTransient<TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass3, TestMultiInheritClass4>()
                        .addAlias<TestMultiInherit4Class4, TestMultiInheritClass4>()
                        .buildServiceProvider();

    auto services = provider.getServices<TestMultiInheritClass1>();

    EXPECT_EQ(provider.createServices<TestMultiInheritClass3>().size(), 1);
    EXPECT_EQ(provider.createServices<TestMultiInherit4Class4>().size(), 1);
    EXPECT_TRUE(provider.createServices<TestMultiInheritClass1>().empty());
    EXPECT_TRUE(provider.createServices<TestMultiInherit2Class1>().empty());
}

TEST_F(AliasTest, ShouldCreateMultipleServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addTransient<TestMultiInheritClass3>()
                        .addTransient<TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass2, TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass3>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass2>()
                        .buildServiceProvider();

    const auto services = provider.createServices<TestMultiInheritClass1>();

    EXPECT_EQ(services.size(), 3);
    EXPECT_EQ(services[0]->first(), 4);
    EXPECT_EQ(services[1]->first(), 3);
    EXPECT_EQ(services[2]->first(), 4);
}

TEST_F(AliasTest, ShouldCreateMultipleSomeWrongServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addTransient<TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass2, TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass3>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass2>()
                        .buildServiceProvider();

    const auto services = provider.createServices<TestMultiInheritClass1>();

    EXPECT_EQ(services.size(), 2);
    EXPECT_EQ(services[0]->first(), 4);
    EXPECT_EQ(services[1]->first(), 4);
}

TEST_F(AliasTest, ShoulNotCreateServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass3>()
                        .addAlias<TestMultiInherit2Class1, TestMultiInheritClass2>()
                        .addAlias<TestMultiInheritClass3, TestMultiInheritClass4>()
                        .addAlias<TestMultiInherit4Class4, TestMultiInheritClass4>()
                        .buildServiceProvider();

    EXPECT_TRUE(provider.createServices<TestMultiInheritClass1>().empty());
    EXPECT_TRUE(provider.createServices<TestMultiInherit2Class1>().empty());
    EXPECT_TRUE(provider.createServices<TestMultiInheritClass3>().empty());
    EXPECT_TRUE(provider.createServices<TestMultiInherit4Class4>().empty());
}

TEST_F(AliasTest, ShouldCreateCastedServicesInOrder)
{
    auto provider = sb::di::ServiceCollection{}
                        .addTransient<TestMultiInheritClass2, TestMultiInheritClass3>()
                        .addTransient<TestMultiInheritClass2, TestMultiInheritClass4>()
                        .addAlias<TestMultiInheritClass1, TestMultiInheritClass2>()
                        .buildServiceProvider();

    const auto services = provider.createServices<TestMultiInheritClass1>();

    EXPECT_EQ(services.size(), 2);
    EXPECT_EQ(services[0]->first(), 3);
    EXPECT_EQ(services[1]->first(), 4);
}
