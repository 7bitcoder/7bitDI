#include <gtest/gtest.h>
#include <memory>

#include "../Helpers/Classes/Inherit.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceCollection.hpp"

class InheritanceUniqFactoryTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    InheritanceUniqFactoryTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~InheritanceUniqFactoryTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(InheritanceUniqFactoryTest, ShouldFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    collection.addSingleton<TestInheritClass1>([] { return std::make_unique<TestInheritClass2>(); });
    collection.addSingleton<TestInheritClass1>([] { return std::make_unique<TestInheritClass2>(); });
    collection.addSingleton<TestInheritClass1>([] { return std::make_unique<TestInheritClass4>(); });

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = true;
    EXPECT_THROW(collection.buildServiceProvider(options), sb::di::ServiceAlreadyRegisteredException);
}

TEST_F(InheritanceUniqFactoryTest, ShouldNotFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    collection.addSingleton<TestInheritClass1>([] { return std::make_unique<TestInheritClass2>(); });
    collection.addSingleton<TestInheritClass1>([] { return std::make_unique<TestInheritClass2>(); });
    collection.addSingleton<TestInheritClass1>([] { return std::make_unique<TestInheritClass4>(); });

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    EXPECT_NO_THROW(collection.buildServiceProvider(options));
}

TEST_F(InheritanceUniqFactoryTest, ShouldFailGetServiceDueToLifetimeMissmatchService)
{
    sb::di::ServiceCollection collection;

    collection.addSingleton<TestInheritClass1>([] { return std::make_unique<TestInheritClass3>(); });
    collection.addSingleton<TestInheritClass1>([] { return std::make_unique<TestInheritClass5>(); });
    collection.addTransient<TestInheritClass1>([] { return std::make_unique<TestInheritClass4>(); });

    EXPECT_THROW(collection.buildServiceProvider(), sb::di::ServiceLifeTimeMismatchException);
}

TEST_F(InheritanceUniqFactoryTest, ShouldTryGetService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1>([] { return std::make_unique<TestInheritClass2>(); })
                        .addScoped<TestInheritClass2>([] { return std::make_unique<TestInheritClass3>(); })
                        .addTransient<TestInheritClass3>([] { return std::make_unique<TestInheritClass4>(); })
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetService<TestInheritClass1>());
    EXPECT_TRUE(provider.tryGetService<TestInheritClass2>());
    EXPECT_FALSE(provider.tryGetService<TestInheritClass3>());
    EXPECT_FALSE(provider.tryGetService<TestInheritClass4>());
}

TEST_F(InheritanceUniqFactoryTest, ShouldTryGetOneLastServiceSingleton)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1>([] { return std::make_unique<TestInheritClass2>(); })
                        .addSingleton<TestInheritClass1>([] { return std::make_unique<TestInheritClass3>(); })
                        .addSingleton<TestInheritClass1>([] { return std::make_unique<TestInheritClass5>(); })
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetService<TestInheritClass1>());
    EXPECT_EQ(provider.tryGetService<TestInheritClass1>()->number(), 5);
}

TEST_F(InheritanceUniqFactoryTest, ShouldTryGetOneLastServiceScope)
{
    auto provider = sb::di::ServiceCollection{}
                        .addScoped<TestInheritClass1>([] { return std::make_unique<TestInheritClass2>(); })
                        .addScoped<TestInheritClass1>([] { return std::make_unique<TestInheritClass3>(); })
                        .addScoped<TestInheritClass1>([] { return std::make_unique<TestInheritClass5>(); })
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetService<TestInheritClass1>());
    EXPECT_EQ(provider.tryGetService<TestInheritClass1>()->number(), 5);
}

TEST_F(InheritanceUniqFactoryTest, ShouldGetService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1>([] { return std::make_unique<TestInheritClass2>(); })
                        .addScoped<TestInheritClass2>([] { return std::make_unique<TestInheritClass3>(); })
                        .addTransient<TestInheritClass3>([] { return std::make_unique<TestInheritClass4>(); })
                        .buildServiceProvider();

    EXPECT_NO_THROW(provider.getService<TestInheritClass1>());
    EXPECT_NO_THROW(provider.getService<TestInheritClass2>());
    EXPECT_THROW(provider.getService<TestInheritClass3>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.getService<TestInheritClass4>(), sb::di::ServiceNotFoundException);
}

TEST_F(InheritanceUniqFactoryTest, ShouldGetOneLastServiceSingleton)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1>([] { return std::make_unique<TestInheritClass2>(); })
                        .addSingleton<TestInheritClass1>([] { return std::make_unique<TestInheritClass3>(); })
                        .addSingleton<TestInheritClass1>([] { return std::make_unique<TestInheritClass5>(); })
                        .buildServiceProvider();

    EXPECT_EQ(provider.getService<TestInheritClass1>().number(), 5);
}

TEST_F(InheritanceUniqFactoryTest, ShouldGetOneLastServiceScope)
{
    auto provider = sb::di::ServiceCollection{}
                        .addScoped<TestInheritClass1>([] { return std::make_unique<TestInheritClass2>(); })
                        .addScoped<TestInheritClass1>([] { return std::make_unique<TestInheritClass3>(); })
                        .addScoped<TestInheritClass1>([] { return std::make_unique<TestInheritClass5>(); })
                        .buildServiceProvider();

    EXPECT_EQ(provider.getService<TestInheritClass1>().number(), 5);
}

TEST_F(InheritanceUniqFactoryTest, ShouldGetServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1>([] { return std::make_unique<TestInheritClass2>(); })
                        .addScoped<TestInheritClass2>([] { return std::make_unique<TestInheritClass3>(); })
                        .addTransient<TestInheritClass3>([] { return std::make_unique<TestInheritClass4>(); })
                        .buildServiceProvider();

    EXPECT_EQ(provider.getServices<TestInheritClass1>().size(), 1);
    EXPECT_EQ(provider.getServices<TestInheritClass2>().size(), 1);
    EXPECT_TRUE(provider.getServices<TestInheritClass3>().empty());
    EXPECT_TRUE(provider.getServices<TestInheritClass4>().empty());
}

TEST_F(InheritanceUniqFactoryTest, ShouldGetCastedServicesInOrder)
{
    auto provider = sb::di::ServiceCollection{}
                        .addScoped([] { return TestInheritClass1{}; })
                        .addScoped<TestInheritClass1>([] { return std::make_unique<TestInheritClass2>(); })
                        .addScoped<TestInheritClass1>([] { return std::make_unique<TestInheritClass3>(); })
                        .addScoped<TestInheritClass1>([] { return std::make_unique<TestInheritClass4>(); })
                        .addScoped<TestInheritClass1>([] { return std::make_unique<TestInheritClass5>(); })
                        .buildServiceProvider();

    const auto services = provider.getServices<TestInheritClass1>();
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(services[0]->number(), 1);
    EXPECT_EQ(services[1]->number(), 2);
    EXPECT_EQ(services[2]->number(), 3);
    EXPECT_EQ(services[3]->number(), 4);
    EXPECT_EQ(services[4]->number(), 5);
}

TEST_F(InheritanceUniqFactoryTest, ShouldGetServicesInOrderAfterNormalGet)
{
    auto provider = sb::di::ServiceCollection{}
                        .addScoped([] { return TestInheritClass1{}; })
                        .addScoped<TestInheritClass1>([] { return std::make_unique<TestInheritClass2>(); })
                        .addScoped<TestInheritClass1>([] { return std::make_unique<TestInheritClass3>(); })
                        .addScoped<TestInheritClass1>([] { return std::make_unique<TestInheritClass4>(); })
                        .addScoped<TestInheritClass1>([] { return std::make_unique<TestInheritClass5>(); })
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

TEST_F(InheritanceUniqFactoryTest, ShouldTryCreateService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1>([] { return std::make_unique<TestInheritClass2>(); })
                        .addScoped<TestInheritClass2>([] { return std::make_unique<TestInheritClass3>(); })
                        .addTransient<TestInheritClass3>([] { return std::make_unique<TestInheritClass4>(); })
                        .buildServiceProvider();

    EXPECT_FALSE(provider.tryCreateService<TestInheritClass1>());
    EXPECT_FALSE(provider.tryCreateService<TestInheritClass2>());
    EXPECT_TRUE(provider.tryCreateService<TestInheritClass3>());
    EXPECT_FALSE(provider.tryCreateService<TestInheritClass4>());
}

TEST_F(InheritanceUniqFactoryTest, ShouldCreateService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1>([] { return std::make_unique<TestInheritClass2>(); })
                        .addScoped<TestInheritClass2>([] { return std::make_unique<TestInheritClass3>(); })
                        .addTransient<TestInheritClass3>([] { return std::make_unique<TestInheritClass4>(); })
                        .buildServiceProvider();

    EXPECT_THROW(provider.createService<TestInheritClass1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createService<TestInheritClass2>(), sb::di::ServiceNotFoundException);
    EXPECT_NO_THROW(provider.createService<TestInheritClass3>());
    EXPECT_THROW(provider.createService<TestInheritClass4>(), sb::di::ServiceNotFoundException);
}

TEST_F(InheritanceUniqFactoryTest, ShouldCreateServiceInPlace)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1>([] { return std::make_unique<TestInheritClass2>(); })
                        .addScoped<TestInheritClass2>([] { return std::make_unique<TestInheritClass3>(); })
                        .addTransient<TestInheritClass3>([] { return std::make_unique<TestInheritClass4>(); })
                        .buildServiceProvider();

    EXPECT_THROW(provider.createServiceInPlace<TestInheritClass1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestInheritClass2>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestInheritClass3>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestInheritClass4>(), sb::di::ServiceNotFoundException);
}

TEST_F(InheritanceUniqFactoryTest, ShouldCreateServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1>([] { return std::make_unique<TestInheritClass2>(); })
                        .addScoped<TestInheritClass2>([] { return std::make_unique<TestInheritClass3>(); })
                        .addTransient<TestInheritClass3>([] { return std::make_unique<TestInheritClass4>(); })
                        .buildServiceProvider();

    EXPECT_TRUE(provider.createServices<TestInheritClass1>().empty());
    EXPECT_TRUE(provider.createServices<TestInheritClass2>().empty());
    EXPECT_EQ(provider.createServices<TestInheritClass3>().size(), 1);
    EXPECT_TRUE(provider.createServices<TestInheritClass4>().empty());
}

TEST_F(InheritanceUniqFactoryTest, ShouldCreateServicesInOrder)
{
    auto provider = sb::di::ServiceCollection{}
                        .addTransient([] { return TestInheritClass1{}; })
                        .addTransient<TestInheritClass1>([] { return std::make_unique<TestInheritClass2>(); })
                        .addTransient<TestInheritClass1>([] { return std::make_unique<TestInheritClass3>(); })
                        .addTransient<TestInheritClass1>([] { return std::make_unique<TestInheritClass4>(); })
                        .addTransient<TestInheritClass1>([] { return std::make_unique<TestInheritClass5>(); })
                        .buildServiceProvider();

    const auto services = provider.createServices<TestInheritClass1>();
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(services[0]->number(), 1);
    EXPECT_EQ(services[1]->number(), 2);
    EXPECT_EQ(services[2]->number(), 3);
    EXPECT_EQ(services[3]->number(), 4);
    EXPECT_EQ(services[4]->number(), 5);
}
