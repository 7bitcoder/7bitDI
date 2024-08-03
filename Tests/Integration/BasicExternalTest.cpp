#include <gtest/gtest.h>
#include <memory>

#include "../Helpers/Classes/Basic.hpp"
#include <SevenBit/DI/Exceptions.hpp>
#include <SevenBit/DI/ServiceCollection.hpp>

class BasicExternalTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    BasicExternalTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~BasicExternalTest() override = default;

    static void TearDownTestSuite() {}
};

// buildServiceProvider Tests

TEST_F(BasicExternalTest, ShouldFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    TestClass1 test1;
    TestClass2 test2;
    collection.addSingleton(&test1);
    collection.addSingleton(&test1);
    collection.addSingleton(&test2);

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = true;
    EXPECT_THROW(collection.buildServiceProvider(options), sb::di::ServiceRegisterException);
}

TEST_F(BasicExternalTest, ShouldNotFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    TestClass1 test1;
    TestClass2 test2;
    collection.addSingleton(&test1);
    collection.addSingleton(&test1);
    collection.addSingleton(&test2);

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    EXPECT_NO_THROW(collection.buildServiceProvider(options));
}

// tryGetService Tests

TEST_F(BasicExternalTest, ShouldTryGetService)
{
    TestClass1 test1;
    TestClass2 test2;
    TestClass3 test3;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton(&test1)
                        .addSingleton(&test2)
                        .addSingleton(&test3)
                        .buildServiceProvider();

    EXPECT_EQ(provider.tryGetService<TestClass1>(), &test1);
    EXPECT_EQ(provider.tryGetService<TestClass2>(), &test2);
    EXPECT_EQ(provider.tryGetService<TestClass3>(), &test3);
    EXPECT_FALSE(provider.tryGetService<TestClass4>());
}

// getService Tests

TEST_F(BasicExternalTest, ShouldGetService)
{
    TestClass1 test1;
    TestClass2 test2;
    TestClass3 test3;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton(&test1)
                        .addSingleton(&test2)
                        .addSingleton(&test3)
                        .buildServiceProvider();

    EXPECT_EQ(&provider.getService<TestClass1>(), &test1);
    EXPECT_EQ(&provider.getService<TestClass2>(), &test2);
    EXPECT_EQ(&provider.getService<TestClass3>(), &test3);
    EXPECT_THROW(provider.getService<TestClass4>(), sb::di::ServiceNotFoundException);
}

TEST_F(BasicExternalTest, ShouldGetServices)
{
    TestClass1 test1;
    TestClass2 test2;
    TestClass3 test3;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton(&test1)
                        .addSingleton(&test2)
                        .addSingleton(&test3)
                        .buildServiceProvider();

    EXPECT_EQ(provider.getServices<TestClass1>().size(), 1);
    EXPECT_EQ(provider.getServices<TestClass1>()[0], &test1);
    EXPECT_EQ(provider.getServices<TestClass2>().size(), 1);
    EXPECT_EQ(provider.getServices<TestClass2>()[0], &test2);
    EXPECT_EQ(provider.getServices<TestClass3>().size(), 1);
    EXPECT_EQ(provider.getServices<TestClass3>()[0], &test3);
}

// tryCreateService Tests

TEST_F(BasicExternalTest, ShouldTryCreateService)
{
    TestClass1 test1;
    TestClass2 test2;
    TestClass3 test3;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton(&test1)
                        .addSingleton(&test2)
                        .addSingleton(&test3)
                        .buildServiceProvider();

    EXPECT_FALSE(provider.tryCreateService<TestClass1>());
    EXPECT_FALSE(provider.tryCreateService<TestClass2>());
    EXPECT_FALSE(provider.tryCreateService<TestClass3>());
    EXPECT_FALSE(provider.tryCreateService<TestClass4>());
}

// createService Tests

TEST_F(BasicExternalTest, ShouldCreateService)
{
    TestClass1 test1;
    TestClass2 test2;
    TestClass3 test3;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton(&test1)
                        .addSingleton(&test2)
                        .addSingleton(&test3)
                        .buildServiceProvider();

    EXPECT_THROW(provider.createService<TestClass1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createService<TestClass2>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createService<TestClass3>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createService<TestClass4>(), sb::di::ServiceNotFoundException);
}

// createServiceInPlace Tests

TEST_F(BasicExternalTest, ShouldCreateServiceInPlace)
{
    TestClass1 test1;
    TestClass2 test2;
    TestClass3 test3;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton(&test1)
                        .addSingleton(&test2)
                        .addSingleton(&test3)
                        .buildServiceProvider();

    EXPECT_THROW(provider.createServiceInPlace<TestClass1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestClass2>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestClass3>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestClass4>(), sb::di::ServiceNotFoundException);
}

// createServices Tests

TEST_F(BasicExternalTest, ShouldCreateServices)
{
    TestClass1 test1;
    TestClass2 test2;
    TestClass3 test3;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton(&test1)
                        .addSingleton(&test2)
                        .addSingleton(&test3)
                        .buildServiceProvider();

    EXPECT_EQ(provider.createServices<TestClass1>().size(), 0);
    EXPECT_EQ(provider.createServices<TestClass2>().size(), 0);
    EXPECT_EQ(provider.createServices<TestClass3>().size(), 0);
}
