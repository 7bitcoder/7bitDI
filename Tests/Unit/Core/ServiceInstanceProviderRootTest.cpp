#include <gtest/gtest.h>
#include <memory>

#include "../../Helpers/Classes/Basic.hpp"
#include "../../Helpers/Classes/Inherit.hpp"
#include "../../Helpers/Mocks/ServiceProviderMock.hpp"
#include "SevenBit/DI/Details/Core/ServiceInstanceProviderRoot.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceDescriber.hpp"

class ServiceInstanceProviderRootTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    ServiceInstanceProviderRootTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceInstanceProviderRootTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ServiceInstanceProviderRootTest, ShouldGetOptions)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestClass1>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    const auto &options = provider.getOptions();
    EXPECT_FALSE(options.prebuildSingletons);
    EXPECT_FALSE(options.strongDestructionOrder);
    EXPECT_TRUE(options.checkServiceGlobalUniqueness);
}

TEST_F(ServiceInstanceProviderRootTest, ShouldFailGetServiceDueToAlreadyRegisteredService)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestClass1>());
    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestClass1>());
    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestClass2>());

    auto act = [&] {
        sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
        provider.init(mock);
    };

    EXPECT_THROW(act(), sb::di::ServiceAlreadyRegisteredException);
}

TEST_F(ServiceInstanceProviderRootTest, ShouldFailGetServiceDueToAlreadyRegisteredInheritedService)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    auto act = [&] {
        sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
        provider.init(mock);
    };

    EXPECT_THROW(act(), sb::di::ServiceAlreadyRegisteredException);
}

TEST_F(ServiceInstanceProviderRootTest, ShouldFailGetServiceDueToAliasMissmatchInheritedService)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
    describers.emplace_back(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass3>());

    auto act = [&] {
        sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
        provider.init(mock);
    };

    EXPECT_THROW(act(), sb::di::ServiceAliasMismatchException);
}

TEST_F(ServiceInstanceProviderRootTest, ShouldFailGetServiceDueToAliasMissmatchOpositeInheritedService)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass4>());
    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());

    auto act = [&] {
        sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
        provider.init(mock);
    };

    EXPECT_THROW(act(), sb::di::ServiceAliasMismatchException);
}

TEST_F(ServiceInstanceProviderRootTest, ShouldNotFailGetServiceDueToAlreadyRegisteredService)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestClass1>());
    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestClass1>());
    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestClass2>());

    auto act = [&] {
        sb::di::ServiceProviderOptions options;
        options.checkServiceGlobalUniqueness = false;

        sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end(), options);
        provider.init(mock);
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceInstanceProviderRootTest, ShouldNotFailGetServiceDueToAlreadyRegisteredInheritedService)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    auto act = [&] {
        sb::di::ServiceProviderOptions options;
        options.checkServiceGlobalUniqueness = false;

        sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end(), options);
        provider.init(mock);
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceInstanceProviderRootTest, ShouldFailGetServiceDueToLifetimeMissmatchInheritedService)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass1, TestInheritClass5>());
    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    auto act = [&] {
        sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
        provider.init(mock);
    };

    EXPECT_THROW(act(), sb::di::ServiceLifeTimeMismatchException);
}

TEST_F(ServiceInstanceProviderRootTest, ShouldPrebuildSingletons)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestClass1>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestClass3>());

    sb::di::ServiceProviderOptions options;
    options.prebuildSingletons = true;
    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end(), options);
    provider.init(mock);

    EXPECT_TRUE(provider.tryGetInstance(typeid(TestClass1)));
    EXPECT_TRUE(provider.tryGetInstance(typeid(TestClass2)));
    EXPECT_FALSE(provider.tryGetInstance(typeid(TestClass3)));
    EXPECT_FALSE(provider.tryGetInstance(typeid(TestClass4)));
}

TEST_F(ServiceInstanceProviderRootTest, ShouldTryGetInstance)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestClass1>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestClass3>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    EXPECT_TRUE(provider.tryGetInstance(typeid(TestClass1)));
    EXPECT_TRUE(provider.tryGetInstance(typeid(TestClass2)));
    EXPECT_FALSE(provider.tryGetInstance(typeid(TestClass3)));
    EXPECT_FALSE(provider.tryGetInstance(typeid(TestClass4)));
}

TEST_F(ServiceInstanceProviderRootTest, ShouldTryGetInheritedInstance)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass4>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass2, TestInheritClass3>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestInheritClass3, TestInheritClass4>());
    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass4, TestInheritClass5>());
    describers.emplace_back(sb::di::ServiceDescriber::describeAlias<TestInheritClass5, TestInheritClass7>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    EXPECT_TRUE(provider.tryGetInstance(typeid(TestInheritClass1)));
    EXPECT_TRUE(provider.tryGetInstance(typeid(TestInheritClass2)));
    EXPECT_TRUE(provider.tryGetInstance(typeid(TestInheritClass4)));
    EXPECT_FALSE(provider.tryGetInstance(typeid(TestInheritClass3)));
    EXPECT_FALSE(provider.tryGetInstance(typeid(TestInheritClass5)));
    EXPECT_FALSE(provider.tryGetInstance(typeid(TestInheritClass6)));
}

TEST_F(ServiceInstanceProviderRootTest, ShouldGetInstance)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestClass1>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestClass3>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    EXPECT_NO_THROW(provider.getInstance(typeid(TestClass1)));
    EXPECT_NO_THROW(provider.getInstance(typeid(TestClass2)));
    EXPECT_TRUE(provider.getInstance(typeid(TestClass1)));
    EXPECT_TRUE(provider.getInstance(typeid(TestClass2)));
    EXPECT_THROW(provider.getInstance(typeid(TestClass3)), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.getInstance(typeid(TestClass4)), sb::di::ServiceNotFoundException);
}

TEST_F(ServiceInstanceProviderRootTest, ShouldGetInheritedInstance)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass4>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass2, TestInheritClass3>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestInheritClass3, TestInheritClass4>());
    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass4, TestInheritClass5>());
    describers.emplace_back(sb::di::ServiceDescriber::describeAlias<TestInheritClass5, TestInheritClass7>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    EXPECT_NO_THROW(provider.getInstance(typeid(TestInheritClass1)));
    EXPECT_NO_THROW(provider.getInstance(typeid(TestInheritClass2)));
    EXPECT_NO_THROW(provider.getInstance(typeid(TestInheritClass4)));
    EXPECT_TRUE(provider.getInstance(typeid(TestInheritClass1)));
    EXPECT_TRUE(provider.getInstance(typeid(TestInheritClass2)));
    EXPECT_TRUE(provider.getInstance(typeid(TestInheritClass4)));
    EXPECT_THROW(provider.getInstance(typeid(TestInheritClass3)), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.getInstance(typeid(TestInheritClass5)), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.getInstance(typeid(TestInheritClass6)), sb::di::ServiceNotFoundException);
}

TEST_F(ServiceInstanceProviderRootTest, ShouldGetSelf)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass4>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass2, TestInheritClass3>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestInheritClass3, TestInheritClass4>());
    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass4, TestInheritClass5>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    auto &self = provider.getInstance(typeid(sb::di::ServiceProvider));
    EXPECT_EQ(self.getAs<void>(), &mock);
}

TEST_F(ServiceInstanceProviderRootTest, ShouldGetInstances)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestClass1>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestClass3>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    EXPECT_EQ(provider.tryGetInstances(typeid(TestClass1))->size(), 1);
    EXPECT_EQ(provider.tryGetInstances(typeid(TestClass2))->size(), 1);
    EXPECT_FALSE(provider.tryGetInstances(typeid(TestClass3)));
    EXPECT_FALSE(provider.tryGetInstances(typeid(TestClass4)));
}

TEST_F(ServiceInstanceProviderRootTest, ShouldGetInstancesInOrder)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass1>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass1, TestInheritClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass1, TestInheritClass3>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass1, TestInheritClass4>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass1, TestInheritClass5>());
    describers.emplace_back(sb::di::ServiceDescriber::describeAlias<TestInheritClass5, TestInheritClass7>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    auto &services = *provider.tryGetInstances(typeid(TestInheritClass1));
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[0].getAs<void>())->number(), 1);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[1].getAs<void>())->number(), 2);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[2].getAs<void>())->number(), 3);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[3].getAs<void>())->number(), 4);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[4].getAs<void>())->number(), 5);
    EXPECT_FALSE(provider.tryGetInstances(typeid(TestInheritClass5)));
    EXPECT_FALSE(provider.tryGetInstances(typeid(TestInheritClass4)));
}

TEST_F(ServiceInstanceProviderRootTest, ShouldGetAliasInstancesInOrder)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass2, TestInheritClass3>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass2, TestInheritClass4>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass2, TestInheritClass5>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass2, TestInheritClass6>());
    describers.emplace_back(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass2>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    auto &services = *provider.tryGetInstances(typeid(TestInheritClass1));
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[0].getAs<void>())->number(), 2);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[1].getAs<void>())->number(), 3);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[2].getAs<void>())->number(), 4);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[3].getAs<void>())->number(), 5);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[4].getAs<void>())->number(), 6);
}

TEST_F(ServiceInstanceProviderRootTest, ShouldTryCreateInstance)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestClass1>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestClass3>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    EXPECT_FALSE(provider.tryCreateInstance(typeid(TestClass1)));
    EXPECT_FALSE(provider.tryCreateInstance(typeid(TestClass2)));
    EXPECT_TRUE(provider.tryCreateInstance(typeid(TestClass3)));
    EXPECT_FALSE(provider.tryCreateInstance(typeid(TestClass4)));
}

TEST_F(ServiceInstanceProviderRootTest, ShouldTryCreateInheritedInstance)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass3>());
    describers.emplace_back(sb::di::ServiceDescriber::describeAlias<TestInheritClass5, TestInheritClass6>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass2, TestInheritClass3>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestInheritClass3, TestInheritClass4>());
    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass6, TestInheritClass7>());
    describers.emplace_back(sb::di::ServiceDescriber::describeAlias<TestInheritClass7, TestInheritClass8>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    EXPECT_TRUE(provider.tryCreateInstance(typeid(TestInheritClass1)));
    EXPECT_FALSE(provider.tryCreateInstance(typeid(TestInheritClass2)));
    EXPECT_TRUE(provider.tryCreateInstance(typeid(TestInheritClass3)));
    EXPECT_FALSE(provider.tryCreateInstance(typeid(TestInheritClass4)));
    EXPECT_FALSE(provider.tryCreateInstance(typeid(TestInheritClass5)));
    EXPECT_FALSE(provider.tryCreateInstance(typeid(TestInheritClass6)));
    EXPECT_FALSE(provider.tryCreateInstance(typeid(TestInheritClass7)));
    EXPECT_FALSE(provider.tryCreateInstance(typeid(TestInheritClass8)));
}

TEST_F(ServiceInstanceProviderRootTest, ShouldCreateInstance)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestClass1>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestClass3>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    EXPECT_THROW(provider.createInstance(typeid(TestClass1)), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createInstance(typeid(TestClass2)), sb::di::ServiceNotFoundException);
    EXPECT_NO_THROW(provider.createInstance(typeid(TestClass3)));
    EXPECT_TRUE(provider.createInstance(typeid(TestClass3)));
    EXPECT_THROW(provider.createInstance(typeid(TestClass4)), sb::di::ServiceNotFoundException);
}

TEST_F(ServiceInstanceProviderRootTest, ShouldCreateInheritedInstance)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass3>());
    describers.emplace_back(sb::di::ServiceDescriber::describeAlias<TestInheritClass5, TestInheritClass6>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass2, TestInheritClass3>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestInheritClass3, TestInheritClass4>());
    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass6, TestInheritClass7>());
    describers.emplace_back(sb::di::ServiceDescriber::describeAlias<TestInheritClass7, TestInheritClass8>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    EXPECT_TRUE(provider.createInstance(typeid(TestInheritClass1)));
    EXPECT_THROW(provider.createInstance(typeid(TestInheritClass2)), sb::di::ServiceNotFoundException);
    EXPECT_TRUE(provider.createInstance(typeid(TestInheritClass3)));
    EXPECT_THROW(provider.createInstance(typeid(TestInheritClass4)), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createInstance(typeid(TestInheritClass5)), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createInstance(typeid(TestInheritClass6)), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createInstance(typeid(TestInheritClass7)), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createInstance(typeid(TestInheritClass8)), sb::di::ServiceNotFoundException);
}

TEST_F(ServiceInstanceProviderRootTest, ShouldTryCreateInstanceInPlace)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestClass1>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestClass3>());
    describers.emplace_back(sb::di::ServiceDescriber::describeAlias<TestInheritClass5, TestInheritClass7>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    EXPECT_FALSE(provider.tryCreateInstanceInPlace(typeid(TestClass1)));
    EXPECT_FALSE(provider.tryCreateInstanceInPlace(typeid(TestClass2)));
    EXPECT_TRUE(provider.tryCreateInstanceInPlace(typeid(TestClass3)));
    EXPECT_FALSE(provider.tryCreateInstanceInPlace(typeid(TestClass4)));
    EXPECT_FALSE(provider.tryCreateInstanceInPlace(typeid(TestInheritClass5)));
}

TEST_F(ServiceInstanceProviderRootTest, ShouldCreateInstanceInPlace)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestClass1>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestClass3>());
    describers.emplace_back(sb::di::ServiceDescriber::describeAlias<TestInheritClass5, TestInheritClass7>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    EXPECT_THROW(provider.createInstanceInPlace(typeid(TestClass1)), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createInstanceInPlace(typeid(TestClass2)), sb::di::ServiceNotFoundException);
    EXPECT_NO_THROW(provider.createInstanceInPlace(typeid(TestClass3)));
    EXPECT_TRUE(provider.createInstanceInPlace(typeid(TestClass3)));
    EXPECT_THROW(provider.createInstanceInPlace(typeid(TestClass4)), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createInstanceInPlace(typeid(TestInheritClass5)), sb::di::ServiceNotFoundException);
}

TEST_F(ServiceInstanceProviderRootTest, ShouldCreateInstances)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestClass1>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestClass3>());
    describers.emplace_back(sb::di::ServiceDescriber::describeAlias<TestInheritClass5, TestInheritClass7>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    EXPECT_FALSE(provider.tryCreateInstances(typeid(TestClass1)));
    EXPECT_FALSE(provider.tryCreateInstances(typeid(TestClass2)));
    EXPECT_EQ(provider.tryCreateInstances(typeid(TestClass3))->size(), 1);
    EXPECT_FALSE(provider.tryCreateInstances(typeid(TestClass4)));
    EXPECT_FALSE(provider.tryCreateInstances(typeid(TestInheritClass5)));
    EXPECT_FALSE(provider.tryCreateInstances(typeid(TestInheritClass6)));
}

TEST_F(ServiceInstanceProviderRootTest, ShouldCreateInstancesInOrder)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestInheritClass1>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestInheritClass1, TestInheritClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestInheritClass1, TestInheritClass3>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestInheritClass1, TestInheritClass4>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestInheritClass1, TestInheritClass5>());
    describers.emplace_back(sb::di::ServiceDescriber::describeAlias<TestInheritClass5, TestInheritClass7>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    auto services = *provider.tryCreateInstances(typeid(TestInheritClass1));
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[0].getAs<void>())->number(), 1);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[1].getAs<void>())->number(), 2);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[2].getAs<void>())->number(), 3);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[3].getAs<void>())->number(), 4);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[4].getAs<void>())->number(), 5);
    EXPECT_FALSE(provider.tryCreateInstances(typeid(TestInheritClass5)));
    EXPECT_FALSE(provider.tryCreateInstances(typeid(TestInheritClass6)));
}

TEST_F(ServiceInstanceProviderRootTest, ShouldCreateAliasInstancesInOrder)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestInheritClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestInheritClass2, TestInheritClass3>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestInheritClass2, TestInheritClass4>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestInheritClass2, TestInheritClass5>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestInheritClass2, TestInheritClass6>());
    describers.emplace_back(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass2>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    auto services = *provider.tryCreateInstances(typeid(TestInheritClass1));
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[0].getAs<void>())->number(), 2);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[1].getAs<void>())->number(), 3);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[2].getAs<void>())->number(), 4);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[3].getAs<void>())->number(), 5);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[4].getAs<void>())->number(), 6);
}
