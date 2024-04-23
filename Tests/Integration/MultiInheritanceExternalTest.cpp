#include <gtest/gtest.h>
#include <memory>

#include "../Helpers/Classes/MultiInherit.hpp"
#include <SevenBit/DI/ServiceCollection.hpp>

class MultiInheritanceExternalTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    MultiInheritanceExternalTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~MultiInheritanceExternalTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(MultiInheritanceExternalTest, ShouldTryGetService)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    TestMultiInheritClass2 test2;
    TestMultiInheritClass3 test3;
    TestMultiInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass1>(&test2)
                        .addSingleton<TestMultiInherit2Class1>(&test2)
                        .addSingleton<TestMultiInheritClass2>(&test3)
                        .addSingleton<TestMultiInherit3Class1>(&test3)
                        .addSingleton<TestMultiInheritClass3>(&test4)
                        .addSingleton<TestMultiInherit4Class4>(&test4)
                        .buildServiceProvider(options);

    EXPECT_EQ(provider.tryGetService<TestMultiInheritClass1>(), &test2);
    EXPECT_EQ(provider.tryGetService<TestMultiInherit2Class1>(), &test2);
    EXPECT_EQ(provider.tryGetService<TestMultiInheritClass2>(), &test3);
    EXPECT_EQ(provider.tryGetService<TestMultiInherit3Class1>(), &test3);
    EXPECT_EQ(provider.tryGetService<TestMultiInheritClass3>(), &test4);
    EXPECT_EQ(provider.tryGetService<TestMultiInherit4Class4>(), &test4);
}

TEST_F(MultiInheritanceExternalTest, ShouldTryGetOneLastServiceSingleton)
{
    TestMultiInheritClass2 test2;
    TestMultiInheritClass3 test3;
    TestMultiInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInherit2Class1>(&test2)
                        .addSingleton<TestMultiInherit2Class1>(&test3)
                        .addSingleton<TestMultiInherit2Class1>(&test4)
                        .buildServiceProvider();

    const auto service = provider.tryGetService<TestMultiInherit2Class1>();

    EXPECT_TRUE(service);
    EXPECT_EQ(service, &test4);
}

TEST_F(MultiInheritanceExternalTest, ShouldGetService)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    TestMultiInheritClass2 test2;
    TestMultiInheritClass3 test3;
    TestMultiInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass1>(&test2)
                        .addSingleton<TestMultiInherit2Class1>(&test2)
                        .addSingleton<TestMultiInheritClass2>(&test3)
                        .addSingleton<TestMultiInherit3Class1>(&test3)
                        .addSingleton<TestMultiInheritClass3>(&test4)
                        .addSingleton<TestMultiInherit4Class4>(&test4)
                        .buildServiceProvider(options);

    EXPECT_EQ(&provider.getService<TestMultiInheritClass1>(), &test2);
    EXPECT_EQ(&provider.getService<TestMultiInherit2Class1>(), &test2);
    EXPECT_EQ(&provider.getService<TestMultiInheritClass2>(), &test3);
    EXPECT_EQ(&provider.getService<TestMultiInherit3Class1>(), &test3);
    EXPECT_EQ(&provider.getService<TestMultiInheritClass3>(), &test4);
    EXPECT_EQ(&provider.getService<TestMultiInherit4Class4>(), &test4);
}

TEST_F(MultiInheritanceExternalTest, ShouldGetOneLastServiceSingleton)
{
    TestMultiInheritClass2 test2;
    TestMultiInheritClass3 test3;
    TestMultiInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInherit2Class1>(&test2)
                        .addSingleton<TestMultiInherit2Class1>(&test3)
                        .addSingleton<TestMultiInherit2Class1>(&test4)
                        .buildServiceProvider();

    EXPECT_EQ(&provider.getService<TestMultiInherit2Class1>(), &test4);
}

TEST_F(MultiInheritanceExternalTest, ShouldGetServices)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    TestMultiInheritClass2 test2;
    TestMultiInheritClass3 test3;
    TestMultiInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass1>(&test2)
                        .addSingleton<TestMultiInherit2Class1>(&test2)
                        .addSingleton<TestMultiInheritClass2>(&test3)
                        .addSingleton<TestMultiInherit3Class1>(&test3)
                        .addSingleton<TestMultiInheritClass3>(&test4)
                        .addSingleton<TestMultiInherit4Class4>(&test4)
                        .buildServiceProvider(options);

    EXPECT_EQ(provider.getServices<TestMultiInheritClass1>().size(), 1);
    EXPECT_EQ(provider.getServices<TestMultiInheritClass1>()[0], &test2);
    EXPECT_EQ(provider.getServices<TestMultiInherit2Class1>().size(), 1);
    EXPECT_EQ(provider.getServices<TestMultiInherit2Class1>()[0], &test2);
    EXPECT_EQ(provider.getServices<TestMultiInheritClass2>().size(), 1);
    EXPECT_EQ(provider.getServices<TestMultiInheritClass2>()[0], &test3);
    EXPECT_EQ(provider.getServices<TestMultiInherit3Class1>().size(), 1);
    EXPECT_EQ(provider.getServices<TestMultiInherit3Class1>()[0], &test3);
    EXPECT_EQ(provider.getServices<TestMultiInheritClass3>().size(), 1);
    EXPECT_EQ(provider.getServices<TestMultiInheritClass3>()[0], &test4);
    EXPECT_EQ(provider.getServices<TestMultiInherit4Class4>().size(), 1);
    EXPECT_EQ(provider.getServices<TestMultiInherit4Class4>()[0], &test4);
}

TEST_F(MultiInheritanceExternalTest, ShouldGetCastedServicesInOrder)
{
    TestMultiInheritClass2 test2;
    TestMultiInheritClass3 test3;
    TestMultiInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInherit2Class1>(&test2)
                        .addSingleton<TestMultiInherit2Class1>(&test3)
                        .addSingleton<TestMultiInherit2Class1>(&test4)
                        .buildServiceProvider();

    const auto services = provider.getServices<TestMultiInherit2Class1>();

    EXPECT_EQ(services.size(), 3);
    EXPECT_EQ(services[0], &test2);
    EXPECT_EQ(services[1], &test3);
    EXPECT_EQ(services[2], &test4);
}

TEST_F(MultiInheritanceExternalTest, ShouldGetServicesInOrderAfterNormalGet)
{
    TestMultiInheritClass2 test2;
    TestMultiInheritClass3 test3;
    TestMultiInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInherit2Class1>(&test2)
                        .addSingleton<TestMultiInherit2Class1>(&test3)
                        .addSingleton<TestMultiInherit2Class1>(&test4)
                        .buildServiceProvider();

    EXPECT_EQ(provider.tryGetService<TestMultiInherit2Class1>(), &test4);
    const auto services = provider.getServices<TestMultiInherit2Class1>();
    EXPECT_EQ(services.size(), 3);
    EXPECT_EQ(services[0], &test2);
    EXPECT_EQ(services[1], &test3);
    EXPECT_EQ(services[2], &test4);
}

TEST_F(MultiInheritanceExternalTest, ShouldTryCreateService)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    TestMultiInheritClass2 test2;
    TestMultiInheritClass3 test3;
    TestMultiInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass1>(&test2)
                        .addSingleton<TestMultiInherit2Class1>(&test2)
                        .addSingleton<TestMultiInheritClass2>(&test3)
                        .addSingleton<TestMultiInherit3Class1>(&test3)
                        .addSingleton<TestMultiInheritClass3>(&test4)
                        .addSingleton<TestMultiInherit4Class4>(&test4)
                        .buildServiceProvider(options);

    EXPECT_FALSE(provider.tryCreateService<TestMultiInheritClass2>());
    EXPECT_FALSE(provider.tryCreateService<TestMultiInherit3Class1>());
    EXPECT_FALSE(provider.tryCreateService<TestMultiInheritClass1>());
    EXPECT_FALSE(provider.tryCreateService<TestMultiInherit2Class1>());
    EXPECT_FALSE(provider.tryCreateService<TestMultiInheritClass3>());
    EXPECT_FALSE(provider.tryCreateService<TestMultiInherit4Class4>());
}

TEST_F(MultiInheritanceExternalTest, ShouldCreateService)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    TestMultiInheritClass2 test2;
    TestMultiInheritClass3 test3;
    TestMultiInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass1>(&test2)
                        .addSingleton<TestMultiInherit2Class1>(&test2)
                        .addSingleton<TestMultiInheritClass2>(&test3)
                        .addSingleton<TestMultiInherit3Class1>(&test3)
                        .addSingleton<TestMultiInheritClass3>(&test4)
                        .addSingleton<TestMultiInherit4Class4>(&test4)
                        .buildServiceProvider(options);

    EXPECT_THROW(provider.createService<TestMultiInheritClass2>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createService<TestMultiInherit3Class1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createService<TestMultiInheritClass1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createService<TestMultiInherit2Class1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createService<TestMultiInheritClass3>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createService<TestMultiInherit4Class4>(), sb::di::ServiceNotFoundException);
}

TEST_F(MultiInheritanceExternalTest, ShouldNotCreateServiceInPlace)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    TestMultiInheritClass2 test2;
    TestMultiInheritClass3 test3;
    TestMultiInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass1>(&test2)
                        .addSingleton<TestMultiInherit2Class1>(&test2)
                        .addSingleton<TestMultiInheritClass2>(&test3)
                        .addSingleton<TestMultiInherit3Class1>(&test3)
                        .addSingleton<TestMultiInheritClass3>(&test4)
                        .addSingleton<TestMultiInherit4Class4>(&test4)
                        .buildServiceProvider(options);

    EXPECT_THROW(provider.createServiceInPlace<TestMultiInheritClass2>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestMultiInherit3Class1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestMultiInheritClass1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestMultiInherit2Class1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestMultiInheritClass3>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestMultiInherit4Class4>(), sb::di::ServiceNotFoundException);
}

TEST_F(MultiInheritanceExternalTest, ShouldCreateServices)
{
    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    TestMultiInheritClass2 test2;
    TestMultiInheritClass3 test3;
    TestMultiInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInheritClass1>(&test2)
                        .addSingleton<TestMultiInherit2Class1>(&test2)
                        .addSingleton<TestMultiInheritClass2>(&test3)
                        .addSingleton<TestMultiInherit3Class1>(&test3)
                        .addSingleton<TestMultiInheritClass3>(&test4)
                        .addSingleton<TestMultiInherit4Class4>(&test4)
                        .buildServiceProvider(options);

    EXPECT_TRUE(provider.createServices<TestMultiInheritClass1>().empty());
    EXPECT_TRUE(provider.createServices<TestMultiInherit2Class1>().empty());
    EXPECT_TRUE(provider.createServices<TestMultiInheritClass2>().empty());
    EXPECT_TRUE(provider.createServices<TestMultiInherit3Class1>().empty());
    EXPECT_TRUE(provider.createServices<TestMultiInheritClass3>().empty());
    EXPECT_TRUE(provider.createServices<TestMultiInherit4Class4>().empty());
}

TEST_F(MultiInheritanceExternalTest, ShouldCreateServicesInOrder)
{
    TestMultiInheritClass2 test2;
    TestMultiInheritClass3 test3;
    TestMultiInheritClass4 test4;
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton<TestMultiInherit2Class1>(&test2)
                        .addSingleton<TestMultiInherit2Class1>(&test3)
                        .addSingleton<TestMultiInherit2Class1>(&test4)
                        .buildServiceProvider();

    EXPECT_TRUE(provider.createServices<TestMultiInherit2Class1>().empty());
}
