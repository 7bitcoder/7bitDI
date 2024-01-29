#include <gtest/gtest.h>
#include <memory>

#include "../../Helpers/Classes/Basic.hpp"
#include "../../Helpers/Classes/Inherit.hpp"
#include "../../Helpers/Mocks/ServiceProviderMock.hpp"
#include "SevenBit/DI/Details/Core/ServiceInstanceProvider.hpp"
#include "SevenBit/DI/Details/Core/ServiceInstanceProviderRoot.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceCollection.hpp"

class ServiceInstanceProviderTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    ServiceInstanceProviderTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceInstanceProviderTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ServiceInstanceProviderTest, ShouldGetOptions)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestClass1>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    const auto options = provider.getOptions();
    EXPECT_FALSE(options.prebuildSingletons);
    EXPECT_FALSE(options.strongDestructionOrder);
    EXPECT_TRUE(options.checkServiceGlobalUniqueness);
}

TEST_F(ServiceInstanceProviderTest, ShouldFailGetServiceDueToAlreadyRegisteredService)
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

TEST_F(ServiceInstanceProviderTest, ShouldFailGetServiceDueToAlreadyRegisteredInheritedService)
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

TEST_F(ServiceInstanceProviderTest, ShouldNotFailGetServiceDueToAlreadyRegisteredService)
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

TEST_F(ServiceInstanceProviderTest, ShouldNotFailGetServiceDueToAlreadyRegisteredInheritedService)
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

TEST_F(ServiceInstanceProviderTest, ShouldFailGetServiceDueToLifetimeMissmatchInheritedService)
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

TEST_F(ServiceInstanceProviderTest, ShouldTryGetInstance)
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

TEST_F(ServiceInstanceProviderTest, ShouldTryGetInheritedInstance)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass2, TestInheritClass3>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestInheritClass3, TestInheritClass4>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    EXPECT_TRUE(provider.tryGetInstance(typeid(TestInheritClass1)));
    EXPECT_TRUE(provider.tryGetInstance(typeid(TestInheritClass2)));
    EXPECT_FALSE(provider.tryGetInstance(typeid(TestInheritClass3)));
    EXPECT_FALSE(provider.tryGetInstance(typeid(TestInheritClass4)));
}

TEST_F(ServiceInstanceProviderTest, ShouldGetInstance)
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

TEST_F(ServiceInstanceProviderTest, ShouldGetInheritedInstance)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass2, TestInheritClass3>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestInheritClass3, TestInheritClass4>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    EXPECT_NO_THROW(provider.getInstance(typeid(TestInheritClass1)));
    EXPECT_NO_THROW(provider.getInstance(typeid(TestInheritClass2)));
    EXPECT_TRUE(provider.getInstance(typeid(TestInheritClass1)));
    EXPECT_TRUE(provider.getInstance(typeid(TestInheritClass2)));
    EXPECT_THROW(provider.getInstance(typeid(TestInheritClass3)), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.getInstance(typeid(TestInheritClass4)), sb::di::ServiceNotFoundException);
}

TEST_F(ServiceInstanceProviderTest, ShouldGetSelf)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass2, TestInheritClass3>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestInheritClass3, TestInheritClass4>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    auto &self = provider.getInstance(typeid(sb::di::ServiceProvider));
    EXPECT_EQ(self.get(), &mock);
}

TEST_F(ServiceInstanceProviderTest, ShouldGetInstances)
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

TEST_F(ServiceInstanceProviderTest, ShouldGetInstancesInOrder)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass1>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass1, TestInheritClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass1, TestInheritClass3>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass1, TestInheritClass4>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass1, TestInheritClass5>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    auto &services = *provider.tryGetInstances(typeid(TestInheritClass1));
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[0]->get())->number(), 1);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[1]->get())->number(), 2);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[2]->get())->number(), 3);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[3]->get())->number(), 4);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[4]->get())->number(), 5);
}

TEST_F(ServiceInstanceProviderTest, ShouldTryCreateInstance)
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

TEST_F(ServiceInstanceProviderTest, ShouldTryCreateInheritedInstance)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass2, TestInheritClass3>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestInheritClass3, TestInheritClass4>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    EXPECT_FALSE(provider.tryCreateInstance(typeid(TestInheritClass1)));
    EXPECT_FALSE(provider.tryCreateInstance(typeid(TestInheritClass2)));
    EXPECT_TRUE(provider.tryCreateInstance(typeid(TestInheritClass3)));
    EXPECT_FALSE(provider.tryCreateInstance(typeid(TestInheritClass4)));
}

TEST_F(ServiceInstanceProviderTest, ShouldCreateInstance)
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

TEST_F(ServiceInstanceProviderTest, ShouldCreateInheritedInstance)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestInheritClass2, TestInheritClass3>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestInheritClass3, TestInheritClass4>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    EXPECT_THROW(provider.createInstance(typeid(TestInheritClass1)), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createInstance(typeid(TestInheritClass2)), sb::di::ServiceNotFoundException);
    EXPECT_NO_THROW(provider.createInstance(typeid(TestInheritClass3)));
    EXPECT_TRUE(provider.createInstance(typeid(TestInheritClass3)));
    EXPECT_THROW(provider.createInstance(typeid(TestInheritClass4)), sb::di::ServiceNotFoundException);
}

TEST_F(ServiceInstanceProviderTest, ShouldTryCreateInstanceInPlace)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestClass1>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestClass3>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    EXPECT_FALSE(provider.tryCreateInstanceInPlace(typeid(TestClass1)));
    EXPECT_FALSE(provider.tryCreateInstanceInPlace(typeid(TestClass2)));
    EXPECT_TRUE(provider.tryCreateInstanceInPlace(typeid(TestClass3)));
    EXPECT_FALSE(provider.tryCreateInstanceInPlace(typeid(TestClass4)));
}

TEST_F(ServiceInstanceProviderTest, ShouldCreateInstanceInPlace)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestClass1>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestClass3>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    EXPECT_THROW(provider.createInstanceInPlace(typeid(TestClass1)), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createInstanceInPlace(typeid(TestClass2)), sb::di::ServiceNotFoundException);
    EXPECT_NO_THROW(provider.createInstanceInPlace(typeid(TestClass3)));
    EXPECT_TRUE(provider.createInstanceInPlace(typeid(TestClass3)));
    EXPECT_THROW(provider.createInstanceInPlace(typeid(TestClass4)), sb::di::ServiceNotFoundException);
}

TEST_F(ServiceInstanceProviderTest, ShouldCreateInstances)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeSingleton<TestClass1>());
    describers.emplace_back(sb::di::ServiceDescriber::describeScoped<TestClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestClass3>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    EXPECT_FALSE(provider.tryCreateInstances(typeid(TestClass1)));
    EXPECT_FALSE(provider.tryCreateInstances(typeid(TestClass2)));
    EXPECT_EQ(provider.tryCreateInstances(typeid(TestClass3))->size(), 1);
    EXPECT_FALSE(provider.tryCreateInstances(typeid(TestClass4)));
}

TEST_F(ServiceInstanceProviderTest, ShouldCreateInstancesInOrder)
{
    ServiceProviderMock mock;
    std::vector<sb::di::ServiceDescriptor> describers;

    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestInheritClass1>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestInheritClass1, TestInheritClass2>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestInheritClass1, TestInheritClass3>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestInheritClass1, TestInheritClass4>());
    describers.emplace_back(sb::di::ServiceDescriber::describeTransient<TestInheritClass1, TestInheritClass5>());

    sb::di::details::core::ServiceInstanceProviderRoot provider(describers.begin(), describers.end());
    provider.init(mock);

    auto services = *provider.tryCreateInstances(typeid(TestInheritClass1));
    EXPECT_EQ(services.size(), 5);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[0]->get())->number(), 1);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[1]->get())->number(), 2);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[2]->get())->number(), 3);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[3]->get())->number(), 4);
    EXPECT_EQ(static_cast<TestInheritClass1 *>(services[4]->get())->number(), 5);
}
