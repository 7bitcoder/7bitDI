#include <gtest/gtest.h>
#include <memory>

#include "../Helpers/Classes/Basic.hpp"
#include "../Helpers/Classes/CirularDependency.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceCollection.hpp"

class BasicUniqFactoryTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    BasicUniqFactoryTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~BasicUniqFactoryTest() override = default;

    static void TearDownTestSuite() {}
};

// buildServiceProvider Tests

TEST_F(BasicUniqFactoryTest, ShouldFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    collection.addSingleton([] { return std::make_unique<TestClass1>(); });
    collection.addSingleton([] { return std::make_unique<TestClass1>(); });
    collection.addSingleton([] { return std::make_unique<TestClass2>(); });

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = true;
    EXPECT_THROW(collection.buildServiceProvider(options), sb::di::ServiceAlreadyRegisteredException);
}

TEST_F(BasicUniqFactoryTest, ShouldNotFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    collection.addSingleton([] { return std::make_unique<TestClass1>(); });
    collection.addSingleton([] { return std::make_unique<TestClass1>(); });
    collection.addSingleton([] { return std::make_unique<TestClass2>(); });

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    EXPECT_NO_THROW(collection.buildServiceProvider(options));
}

// tryGetService Tests

TEST_F(BasicUniqFactoryTest, ShouldTryGetService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton([] { return std::make_unique<TestClass1>(); })
                        .addScoped([] { return std::make_unique<TestClass2>(); })
                        .addTransient([] { return std::make_unique<TestClass3>(); })
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetService<TestClass1>());
    EXPECT_TRUE(provider.tryGetService<TestClass2>());
    EXPECT_FALSE(provider.tryGetService<TestClass3>());
    EXPECT_FALSE(provider.tryGetService<TestClass4>());
}

TEST_F(BasicUniqFactoryTest, ShouldFailTryGetServiceDueToCircularDependency)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton([](CircularDependencyB *b) { return std::make_unique<CircularDependencyA>(b); })
                        .addScoped([](CircularDependencyA *a) { return std::make_unique<CircularDependencyB>(a); })
                        .buildServiceProvider();

    EXPECT_THROW(provider.tryGetService<CircularDependencyA>(), sb::di::CircularDependencyException);
}

// getService Tests

TEST_F(BasicUniqFactoryTest, ShouldGetService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton([] { return std::make_unique<TestClass1>(); })
                        .addScoped([] { return std::make_unique<TestClass2>(); })
                        .addTransient([] { return std::make_unique<TestClass3>(); })
                        .buildServiceProvider();

    EXPECT_NO_THROW(provider.getService<TestClass1>());
    EXPECT_NO_THROW(provider.getService<TestClass2>());
    EXPECT_THROW(provider.getService<TestClass3>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.getService<TestClass4>(), sb::di::ServiceNotFoundException);
}
TEST_F(BasicUniqFactoryTest, ShouldFailGetServiceDueToCircularDependency)
{

    auto provider = sb::di::ServiceCollection{}
                        .addSingleton([](CircularDependencyB *b) { return std::make_unique<CircularDependencyA>(b); })
                        .addScoped([](CircularDependencyA *a) { return std::make_unique<CircularDependencyB>(a); })
                        .buildServiceProvider();

    EXPECT_THROW(provider.getService<CircularDependencyA>(), sb::di::CircularDependencyException);
}

// getServices Tests

TEST_F(BasicUniqFactoryTest, ShouldGetServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton([] { return std::make_unique<TestClass1>(); })
                        .addScoped([] { return std::make_unique<TestClass2>(); })
                        .addTransient([] { return std::make_unique<TestClass3>(); })
                        .buildServiceProvider();

    EXPECT_EQ(provider.getServices<TestClass1>().size(), 1);
    EXPECT_EQ(provider.getServices<TestClass2>().size(), 1);
    EXPECT_EQ(provider.getServices<TestClass3>().size(), 0);
}

TEST_F(BasicUniqFactoryTest, ShouldFailGetServicesDueToCircularDependency)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton([](CircularDependencyB *b) { return std::make_unique<CircularDependencyA>(b); })
                        .addScoped([](CircularDependencyA *a) { return std::make_unique<CircularDependencyB>(a); })
                        .buildServiceProvider();

    EXPECT_THROW(provider.getServices<CircularDependencyA>(), sb::di::CircularDependencyException);
}

// tryCreateService Tests

TEST_F(BasicUniqFactoryTest, ShouldTryCreateService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton([] { return std::make_unique<TestClass1>(); })
                        .addScoped([] { return std::make_unique<TestClass2>(); })
                        .addTransient([] { return std::make_unique<TestClass3>(); })
                        .buildServiceProvider();

    EXPECT_FALSE(provider.tryCreateService<TestClass1>());
    EXPECT_FALSE(provider.tryCreateService<TestClass2>());
    EXPECT_TRUE(provider.tryCreateService<TestClass3>());
    EXPECT_FALSE(provider.tryCreateService<TestClass4>());
}

TEST_F(BasicUniqFactoryTest, ShouldFailTryCreateServiceDueToCircularDependency)
{
    auto provider = sb::di::ServiceCollection{}
                        .addTransient([](std::unique_ptr<CircularDependencyUniqueB> b) {
                            return std::make_unique<CircularDependencyUniqueA>(std::move(b));
                        })
                        .addTransient([](std::unique_ptr<CircularDependencyUniqueA> a) {
                            return std::make_unique<CircularDependencyUniqueB>(std::move(a));
                        })
                        .buildServiceProvider();

    EXPECT_THROW(provider.tryCreateService<CircularDependencyUniqueA>(), sb::di::CircularDependencyException);
}

// createService Tests

TEST_F(BasicUniqFactoryTest, ShouldCreateService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton([] { return std::make_unique<TestClass1>(); })
                        .addScoped([] { return std::make_unique<TestClass2>(); })
                        .addTransient([] { return std::make_unique<TestClass3>(); })
                        .buildServiceProvider();

    EXPECT_THROW(provider.createService<TestClass1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createService<TestClass2>(), sb::di::ServiceNotFoundException);
    EXPECT_NO_THROW(provider.createService<TestClass3>());
    EXPECT_THROW(provider.createService<TestClass4>(), sb::di::ServiceNotFoundException);
}

TEST_F(BasicUniqFactoryTest, ShouldFailCreateServiceDueToCircularDependency)
{
    auto provider = sb::di::ServiceCollection{}
                        .addTransient([](std::unique_ptr<CircularDependencyUniqueB> b) {
                            return std::make_unique<CircularDependencyUniqueA>(std::move(b));
                        })
                        .addTransient([](std::unique_ptr<CircularDependencyUniqueA> a) {
                            return std::make_unique<CircularDependencyUniqueB>(std::move(a));
                        })
                        .buildServiceProvider();

    EXPECT_THROW(provider.createService<CircularDependencyUniqueA>(), sb::di::CircularDependencyException);
}

// createServiceInPlace Tests

TEST_F(BasicUniqFactoryTest, ShouldCreateServiceInPlace)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton([] { return std::make_unique<TestClass1>(); })
                        .addScoped([] { return std::make_unique<TestClass2>(); })
                        .addTransient([] { return std::make_unique<TestClass3>(); })
                        .buildServiceProvider();

    EXPECT_THROW(provider.createServiceInPlace<TestClass1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestClass2>(), sb::di::ServiceNotFoundException);
    EXPECT_NO_THROW(provider.createServiceInPlace<TestClass3>());
    EXPECT_THROW(provider.createServiceInPlace<TestClass4>(), sb::di::ServiceNotFoundException);
}

TEST_F(BasicUniqFactoryTest, ShouldFailCreateServiceInPlaceDueToCircularDependency)
{
    auto provider = sb::di::ServiceCollection{}
                        .addTransient([](std::unique_ptr<CircularDependencyUniqueB> b) {
                            return std::make_unique<CircularDependencyUniqueA>(std::move(b));
                        })
                        .addTransient([](std::unique_ptr<CircularDependencyUniqueA> a) {
                            return std::make_unique<CircularDependencyUniqueB>(std::move(a));
                        })
                        .buildServiceProvider();

    EXPECT_THROW(provider.createServiceInPlace<CircularDependencyUniqueA>(), sb::di::CircularDependencyException);
}

// createServices Tests

TEST_F(BasicUniqFactoryTest, ShouldCreateServicess)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton([] { return std::make_unique<TestClass1>(); })
                        .addScoped([] { return std::make_unique<TestClass2>(); })
                        .addTransient([] { return std::make_unique<TestClass3>(); })
                        .buildServiceProvider();

    EXPECT_EQ(provider.createServices<TestClass1>().size(), 0);
    EXPECT_EQ(provider.createServices<TestClass2>().size(), 0);
    EXPECT_EQ(provider.createServices<TestClass3>().size(), 1);
}

TEST_F(BasicUniqFactoryTest, ShouldFaildCreateServicesDueToCircularDependency)
{
    auto provider = sb::di::ServiceCollection{}
                        .addTransient([](std::unique_ptr<CircularDependencyUniqueB> b) {
                            return std::make_unique<CircularDependencyUniqueA>(std::move(b));
                        })
                        .addTransient([](std::unique_ptr<CircularDependencyUniqueA> a) {
                            return std::make_unique<CircularDependencyUniqueB>(std::move(a));
                        })
                        .buildServiceProvider();

    EXPECT_THROW(provider.createServices<CircularDependencyUniqueA>(), sb::di::CircularDependencyException);
}
