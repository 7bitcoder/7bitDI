#include <gtest/gtest.h>
#include <memory>

#include "../../Helpers/Classes/Basic.hpp"
#include "../../Helpers/Classes/CirularDependency.hpp"
#include "../../Helpers/Classes/Inherit.hpp"
#include "../../Helpers/Mocks/ServiceProviderMock.hpp"
#include "SevenBit/DI/Details/Core/ServiceInstanceCreator.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceCollection.hpp"

class ServiceInstanceResolverTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    ServiceInstanceResolverTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceInstanceResolverTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ServiceInstanceResolverTest, ShouldCreateInstance)
{
    ServiceProviderMock mock;
    sb::di::details::core::ServiceInstanceCreator creator;
    creator.setServiceProvider(mock);

    const auto descriptor = sb::di::ServiceDescriber::describeSingleton<TestClass1>();

    const auto instance = creator.createInstance(descriptor, false);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_EQ(instance->getTypeId(), typeid(TestClass1));
}

TEST_F(ServiceInstanceResolverTest, ShouldCreateInstanceAlias)
{
    sb::di::details::core::ServiceInstanceCreator creator;

    const auto descriptor = sb::di::ServiceDescriber::describeSingleton<TestClass1>();

    TestInheritClass5 test;
    const sb::di::details::services::ExternalService external{&test};

    const auto instance = creator.createInstanceAlias(typeid(TestInheritClass3), &external);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_EQ(instance->getTypeId(), typeid(TestInheritClass3));
}

TEST_F(ServiceInstanceResolverTest, ShouldFailForNullProvider)
{
    ServiceProviderMock mock;
    sb::di::details::core::ServiceInstanceCreator creator;

    const auto descriptor = sb::di::ServiceDescriber::describeSingleton<TestClass1>();

    EXPECT_THROW(creator.createInstance(descriptor, false), sb::di::NullPointerException);
}

TEST_F(ServiceInstanceResolverTest, ShouldFailForInvalidInstance)
{
    ServiceProviderMock mock;
    sb::di::details::core::ServiceInstanceCreator creator;
    creator.setServiceProvider(mock);

    const auto descriptor = sb::di::ServiceDescriber::describeSingleton<TestClass1, TestClass1>(nullptr);

    EXPECT_THROW(creator.createInstance(descriptor, false), sb::di::InvalidServiceException);
}

TEST_F(ServiceInstanceResolverTest, ShouldFailForCirculatDependency)
{
    ServiceProviderMock mock;
    sb::di::details::core::ServiceInstanceCreator creator;
    creator.setServiceProvider(mock);

    EXPECT_CALL(mock.getMock(), tryGetInstance(sb::di::TypeId{typeid(CircularDependencyA)}))
        .WillOnce(testing::Invoke([&](sb::di::TypeId typeId) {
            const auto descriptor = sb::di::ServiceDescriber::describeSingleton<CircularDependencyA>();
            creator.createInstance(descriptor, false);
            return nullptr;
        }));

    EXPECT_CALL(mock.getMock(), tryGetInstance(sb::di::TypeId{typeid(CircularDependencyB)}))
        .WillOnce(testing::Invoke([&](sb::di::TypeId typeId) {
            const auto descriptor = sb::di::ServiceDescriber::describeSingleton<CircularDependencyB>();
            creator.createInstance(descriptor, false);
            return nullptr;
        }));

    const auto descriptor = sb::di::ServiceDescriber::describeSingleton<CircularDependencyA>();

    EXPECT_THROW(creator.createInstance(descriptor, false), sb::di::CircularDependencyException);
}

TEST_F(ServiceInstanceResolverTest, ShouldFailCreateInstanceAlias)
{
    sb::di::details::core::ServiceInstanceCreator creator;

    const auto descriptor = sb::di::ServiceDescriber::describeSingleton<TestClass1>();

    TestInheritClass5 *test = nullptr;
    const sb::di::details::services::ExternalService external{test};

    EXPECT_THROW(creator.createInstanceAlias(typeid(TestInheritClass3), &external), sb::di::InvalidServiceException);
}
