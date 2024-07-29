#include <gtest/gtest.h>
#include <memory>

#include "../Helpers/Classes/Basic.hpp"
#include "../Helpers/Classes/CirularDependency.hpp"
#include <SevenBit/DI/Exceptions.hpp>
#include <SevenBit/DI/ServiceCollection.hpp>

class BasicFactoryTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    BasicFactoryTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~BasicFactoryTest() override = default;

    static void TearDownTestSuite() {}
};

// buildServiceProvider Tests

TEST_F(BasicFactoryTest, ShouldFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    collection.addSingleton([] { return TestClass1{}; });
    collection.addSingleton([] { return TestClass1{}; });
    collection.addSingleton([] { return TestClass2{}; });

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = true;
    EXPECT_THROW(collection.buildServiceProvider(options), sb::di::ServiceRegisterException);
}

TEST_F(BasicFactoryTest, ShouldNotFailGetServiceDueToAlreadyRegisteredService)
{
    sb::di::ServiceCollection collection;

    collection.addSingleton([] { return TestClass1{}; });
    collection.addSingleton([] { return TestClass1{}; });
    collection.addSingleton([] { return TestClass2{}; });

    sb::di::ServiceProviderOptions options;
    options.checkServiceGlobalUniqueness = false;
    EXPECT_NO_THROW(collection.buildServiceProvider(options));
}

// tryGetService Tests

TEST_F(BasicFactoryTest, ShouldTryGetService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton([] { return TestClass1{}; })
                        .addScoped([] { return TestClass2{}; })
                        .addTransient([] { return TestClass3{}; })
                        .buildServiceProvider();

    EXPECT_TRUE(provider.tryGetService<TestClass1>());
    EXPECT_TRUE(provider.tryGetService<TestClass2>());
    EXPECT_FALSE(provider.tryGetService<TestClass3>());
    EXPECT_FALSE(provider.tryGetService<TestClass4>());
}

TEST_F(BasicFactoryTest, ShouldFailTryGetServiceDueToCircularDependency)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton([](CircularDependencyB *b) { return CircularDependencyA{b}; })
                        .addScoped([](CircularDependencyA *a) { return CircularDependencyB{a}; })
                        .buildServiceProvider();

    EXPECT_THROW(provider.tryGetService<CircularDependencyA>(), sb::di::CircularDependencyException);
}

// getService Tests

TEST_F(BasicFactoryTest, ShouldGetService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton([] { return TestClass1{}; })
                        .addScoped([] { return TestClass2{}; })
                        .addTransient([] { return TestClass3{}; })
                        .buildServiceProvider();

    EXPECT_NO_THROW(provider.getService<TestClass1>());
    EXPECT_NO_THROW(provider.getService<TestClass2>());
    EXPECT_THROW(provider.getService<TestClass3>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.getService<TestClass4>(), sb::di::ServiceNotFoundException);
}
TEST_F(BasicFactoryTest, ShouldFailGetServiceDueToCircularDependency)
{

    auto provider = sb::di::ServiceCollection{}
                        .addSingleton([](CircularDependencyB *b) { return CircularDependencyA{b}; })
                        .addScoped([](CircularDependencyA *a) { return CircularDependencyB{a}; })
                        .buildServiceProvider();

    EXPECT_THROW(provider.getService<CircularDependencyA>(), sb::di::CircularDependencyException);
}

// getServices Tests

TEST_F(BasicFactoryTest, ShouldGetServices)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton([] { return TestClass1{}; })
                        .addScoped([] { return TestClass2{}; })
                        .addTransient([] { return TestClass3{}; })
                        .buildServiceProvider();

    EXPECT_EQ(provider.getServices<TestClass1>().size(), 1);
    EXPECT_EQ(provider.getServices<TestClass2>().size(), 1);
    EXPECT_EQ(provider.getServices<TestClass3>().size(), 0);
}

TEST_F(BasicFactoryTest, ShouldFailGetServicesDueToCircularDependency)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton([](CircularDependencyB *b) { return CircularDependencyA{b}; })
                        .addScoped([](CircularDependencyA *a) { return CircularDependencyB{a}; })
                        .buildServiceProvider();

    EXPECT_THROW(provider.getServices<CircularDependencyA>(), sb::di::CircularDependencyException);
}

// tryCreateService Tests

TEST_F(BasicFactoryTest, ShouldTryCreateService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton([] { return TestClass1{}; })
                        .addScoped([] { return TestClass2{}; })
                        .addTransient([] { return TestClass3{}; })
                        .buildServiceProvider();

    EXPECT_FALSE(provider.tryCreateService<TestClass1>());
    EXPECT_FALSE(provider.tryCreateService<TestClass2>());
    EXPECT_TRUE(provider.tryCreateService<TestClass3>());
    EXPECT_FALSE(provider.tryCreateService<TestClass4>());
}

TEST_F(BasicFactoryTest, ShouldFailTryCreateServiceDueToCircularDependency)
{
    auto provider = sb::di::ServiceCollection{}
                        .addTransient([](std::unique_ptr<CircularDependencyUniqueB> b) {
                            return CircularDependencyUniqueA{std::move(b)};
                        })
                        .addTransient([](std::unique_ptr<CircularDependencyUniqueA> a) {
                            return CircularDependencyUniqueB{std::move(a)};
                        })
                        .buildServiceProvider();

    EXPECT_THROW(provider.tryCreateService<CircularDependencyUniqueA>(), sb::di::CircularDependencyException);
}

// createService Tests

TEST_F(BasicFactoryTest, ShouldCreateService)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton([] { return TestClass1{}; })
                        .addScoped([] { return TestClass2{}; })
                        .addTransient([] { return TestClass3{}; })
                        .buildServiceProvider();

    EXPECT_THROW(provider.createService<TestClass1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createService<TestClass2>(), sb::di::ServiceNotFoundException);
    EXPECT_NO_THROW(provider.createService<TestClass3>());
    EXPECT_THROW(provider.createService<TestClass4>(), sb::di::ServiceNotFoundException);
}

TEST_F(BasicFactoryTest, ShouldFailCreateServiceDueToCircularDependency)
{
    auto provider = sb::di::ServiceCollection{}
                        .addTransient([](std::unique_ptr<CircularDependencyUniqueB> b) {
                            return CircularDependencyUniqueA{std::move(b)};
                        })
                        .addTransient([](std::unique_ptr<CircularDependencyUniqueA> a) {
                            return CircularDependencyUniqueB{std::move(a)};
                        })
                        .buildServiceProvider();

    EXPECT_THROW(provider.createService<CircularDependencyUniqueA>(), sb::di::CircularDependencyException);
}

// createServiceInPlace Tests

TEST_F(BasicFactoryTest, ShouldCreateServiceInPlace)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton([] { return TestClass1{}; })
                        .addScoped([] { return TestClass2{}; })
                        .addTransient([] { return TestClass3{}; })
                        .buildServiceProvider();

    EXPECT_THROW(provider.createServiceInPlace<TestClass1>(), sb::di::ServiceNotFoundException);
    EXPECT_THROW(provider.createServiceInPlace<TestClass2>(), sb::di::ServiceNotFoundException);
    EXPECT_NO_THROW(provider.createServiceInPlace<TestClass3>());
    EXPECT_THROW(provider.createServiceInPlace<TestClass4>(), sb::di::ServiceNotFoundException);
}

TEST_F(BasicFactoryTest, ShouldFailCreateServiceInPlaceDueToCircularDependency)
{
    auto provider = sb::di::ServiceCollection{}
                        .addTransient([](std::unique_ptr<CircularDependencyUniqueB> b) {
                            return CircularDependencyUniqueA{std::move(b)};
                        })
                        .addTransient([](std::unique_ptr<CircularDependencyUniqueA> a) {
                            return CircularDependencyUniqueB{std::move(a)};
                        })
                        .buildServiceProvider();

    EXPECT_THROW(provider.createServiceInPlace<CircularDependencyUniqueA>(), sb::di::CircularDependencyException);
}

// createServices Tests

TEST_F(BasicFactoryTest, ShouldCreateServicess)
{
    auto provider = sb::di::ServiceCollection{}
                        .addSingleton([] { return TestClass1{}; })
                        .addScoped([] { return TestClass2{}; })
                        .addTransient([] { return TestClass3{}; })
                        .buildServiceProvider();

    EXPECT_EQ(provider.createServices<TestClass1>().size(), 0);
    EXPECT_EQ(provider.createServices<TestClass2>().size(), 0);
    EXPECT_EQ(provider.createServices<TestClass3>().size(), 1);
}

TEST_F(BasicFactoryTest, ShouldFaildCreateServicesDueToCircularDependency)
{
    auto provider = sb::di::ServiceCollection{}
                        .addTransient([](std::unique_ptr<CircularDependencyUniqueB> b) {
                            return CircularDependencyUniqueA{std::move(b)};
                        })
                        .addTransient([](std::unique_ptr<CircularDependencyUniqueA> a) {
                            return CircularDependencyUniqueB{std::move(a)};
                        })
                        .buildServiceProvider();

    EXPECT_THROW(provider.createServices<CircularDependencyUniqueA>(), sb::di::CircularDependencyException);
}
