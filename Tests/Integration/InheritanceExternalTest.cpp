#include <gtest/gtest.h>
#include <memory>

#include "../Helpers/Classes/Inherit.hpp"
#include <SevenBit/DI/Exceptions.hpp>
#include <SevenBit/DI/ServiceCollection.hpp>

class InheritanceExternalTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    InheritanceExternalTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~InheritanceExternalTest() override = default;

    static void TearDownTestSuite() {}
};

// buildServiceProvider Tests

TEST_F(InheritanceExternalTest, ShouldFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    TestInheritClass2 test2;
    TestInheritClass4 test4;
    collection.addSingleton<TestInheritClass1>(&test2);
    collection.addSingleton<TestInheritClass1>(&test2);
    collection.addSingleton<TestInheritClass1>(&test4);

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = true;
    EXPECT_THROW(collection.buildServiceProvider(options), sb::di::ServiceAlreadyRegisteredException);
}

TEST_F(InheritanceExternalTest, ShouldNotFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    TestInheritClass2 test2;
    TestInheritClass4 test4;
    collection.addSingleton<TestInheritClass1>(&test2);
    collection.addSingleton<TestInheritClass1>(&test2);
    collection.addSingleton<TestInheritClass1>(&test4);

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    EXPECT_NO_THROW(collection.buildServiceProvider(options));
}

// tryGetService Tests

TEST_F(InheritanceExternalTest, ShouldTryGetService)
{
    TestInheritClass2 test2;
    TestInheritClass3 test3;
    TestInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1>(&test2)
                        .addSingleton<TestInheritClass2>(&test3)
                        .addSingleton<TestInheritClass3>(&test4)
                        .buildServiceProvider();

    EXPECT_EQ(provider.tryGetService<TestInheritClass1>(), &test2);
    EXPECT_EQ(provider.tryGetService<TestInheritClass2>(), &test3);
    EXPECT_EQ(provider.tryGetService<TestInheritClass3>(), &test4);
    EXPECT_FALSE(provider.tryGetService<TestInheritClass4>());
}

TEST_F(InheritanceExternalTest, ShouldTryGetOneLastServiceSingleton)
{
    TestInheritClass2 test2;
    TestInheritClass3 test3;
    TestInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1>(&test2)
                        .addSingleton<TestInheritClass1>(&test3)
                        .addSingleton<TestInheritClass1>(&test4)
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetService<TestInheritClass1>());
    EXPECT_EQ(provider.tryGetService<TestInheritClass1>(), &test4);
}

// getService Tests

TEST_F(InheritanceExternalTest, ShouldGetService)
{
    TestInheritClass2 test2;
    TestInheritClass3 test3;
    TestInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1>(&test2)
                        .addSingleton<TestInheritClass2>(&test3)
                        .addSingleton<TestInheritClass3>(&test4)
                        .buildServiceProvider();

    EXPECT_EQ(&provider.getService<TestInheritClass1>(), &test2);
    EXPECT_EQ(&provider.getService<TestInheritClass2>(), &test3);
    EXPECT_EQ(&provider.getService<TestInheritClass3>(), &test4);
    EXPECT_THROW(provider.getService<TestInheritClass4>(), sb::di::ServiceNotFoundException);
}

TEST_F(InheritanceExternalTest, ShouldGetOneLastServiceSingleton)
{
    TestInheritClass2 test2;
    TestInheritClass3 test3;
    TestInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1>(&test2)
                        .addSingleton<TestInheritClass1>(&test3)
                        .addSingleton<TestInheritClass1>(&test4)
                        .buildServiceProvider();

    EXPECT_EQ(&provider.getService<TestInheritClass1>(), &test4);
}

TEST_F(InheritanceExternalTest, ShouldGetServices)
{
    TestInheritClass2 test2;
    TestInheritClass3 test3;
    TestInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1>(&test2)
                        .addSingleton<TestInheritClass2>(&test3)
                        .addSingleton<TestInheritClass3>(&test4)
                        .buildServiceProvider();

    EXPECT_EQ(provider.getServices<TestInheritClass1>().size(), 1);
    EXPECT_EQ(provider.getServices<TestInheritClass1>()[0], &test2);
    EXPECT_EQ(provider.getServices<TestInheritClass2>().size(), 1);
    EXPECT_EQ(provider.getServices<TestInheritClass2>()[0], &test3);
    EXPECT_EQ(provider.getServices<TestInheritClass3>().size(), 1);
    EXPECT_EQ(provider.getServices<TestInheritClass3>()[0], &test4);

    EXPECT_TRUE(provider.getServices<TestInheritClass4>().empty());
}

TEST_F(InheritanceExternalTest, ShouldGetCastedServicesInOrder)
{
    TestInheritClass1 test1;
    TestInheritClass2 test2;
    TestInheritClass3 test3;
    TestInheritClass4 test4;
    TestInheritClass5 test5;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton(&test1)
                        .addSingleton<TestInheritClass1>(&test2)
                        .addSingleton<TestInheritClass1>(&test3)
                        .addSingleton<TestInheritClass1>(&test4)
                        .addSingleton<TestInheritClass1>(&test5)
                        .buildServiceProvider();

    const auto services = provider.getServices<TestInheritClass1>();
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(services[0], &test1);
    EXPECT_EQ(services[1], &test2);
    EXPECT_EQ(services[2], &test3);
    EXPECT_EQ(services[3], &test4);
    EXPECT_EQ(services[4], &test5);
}

TEST_F(InheritanceExternalTest, ShouldGetServicesInOrderAfterNormalGet)
{
    TestInheritClass1 test1;
    TestInheritClass2 test2;
    TestInheritClass3 test3;
    TestInheritClass4 test4;
    TestInheritClass5 test5;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton(&test1)
                        .addSingleton<TestInheritClass1>(&test2)
                        .addSingleton<TestInheritClass1>(&test3)
                        .addSingleton<TestInheritClass1>(&test4)
                        .addSingleton<TestInheritClass1>(&test5)
                        .buildServiceProvider();

    EXPECT_EQ(provider.tryGetService<TestInheritClass1>(), &test5);
    const auto services = provider.getServices<TestInheritClass1>();
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(services[0], &test1);
    EXPECT_EQ(services[1], &test2);
    EXPECT_EQ(services[2], &test3);
    EXPECT_EQ(services[3], &test4);
    EXPECT_EQ(services[4], &test5);
}

TEST_F(InheritanceExternalTest, ShouldTryCreateService)
{
    TestInheritClass2 test2;
    TestInheritClass3 test3;
    TestInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1>(&test2)
                        .addSingleton<TestInheritClass2>(&test3)
                        .addSingleton<TestInheritClass3>(&test4)
                        .buildServiceProvider();

    EXPECT_FALSE(provider.tryCreateService<TestInheritClass1>());
    EXPECT_FALSE(provider.tryCreateService<TestInheritClass2>());
    EXPECT_FALSE(provider.tryCreateService<TestInheritClass3>());
    EXPECT_FALSE(provider.tryCreateService<TestInheritClass4>());
}

TEST_F(InheritanceExternalTest, ShouldCreateService)
{
    TestInheritClass2 test2;
    TestInheritClass3 test3;
    TestInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1>(&test2)
                        .addSingleton<TestInheritClass2>(&test3)
                        .addSingleton<TestInheritClass3>(&test4)
                        .buildServiceProvider();

    EXPECT_THROW(provider.createService<TestInheritClass1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createService<TestInheritClass2>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createService<TestInheritClass3>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createService<TestInheritClass4>(), sb::di::ServiceNotFoundException);
}

TEST_F(InheritanceExternalTest, ShouldCreateServiceInPlace)
{
    TestInheritClass2 test2;
    TestInheritClass3 test3;
    TestInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1>(&test2)
                        .addSingleton<TestInheritClass2>(&test3)
                        .addSingleton<TestInheritClass3>(&test4)
                        .buildServiceProvider();

    EXPECT_THROW(provider.createServiceInPlace<TestInheritClass1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestInheritClass2>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestInheritClass3>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestInheritClass4>(), sb::di::ServiceNotFoundException);
}

TEST_F(InheritanceExternalTest, ShouldCreateServices)
{
    TestInheritClass2 test2;
    TestInheritClass3 test3;
    TestInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestInheritClass1>(&test2)
                        .addSingleton<TestInheritClass2>(&test3)
                        .addSingleton<TestInheritClass3>(&test4)
                        .buildServiceProvider();

    EXPECT_TRUE(provider.createServices<TestInheritClass1>().empty());
    EXPECT_TRUE(provider.createServices<TestInheritClass2>().empty());
    EXPECT_TRUE(provider.createServices<TestInheritClass3>().empty());
    EXPECT_TRUE(provider.createServices<TestInheritClass4>().empty());
}

TEST_F(InheritanceExternalTest, ShouldCreateServicesInOrder)
{
    TestInheritClass1 test1;
    TestInheritClass2 test2;
    TestInheritClass3 test3;
    TestInheritClass4 test4;
    TestInheritClass5 test5;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton(&test1)
                        .addSingleton<TestInheritClass1>(&test2)
                        .addSingleton<TestInheritClass1>(&test3)
                        .addSingleton<TestInheritClass1>(&test4)
                        .addSingleton<TestInheritClass1>(&test5)
                        .buildServiceProvider();

    const auto services = provider.createServices<TestInheritClass1>();
    EXPECT_TRUE(services.empty());
}
