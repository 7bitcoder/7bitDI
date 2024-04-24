#include <gtest/gtest.h>
#include <memory>

#include "../../Helpers/Classes/Basic.hpp"
#include "../../Helpers/Classes/CirularDependency.hpp"
#include "../../Helpers/Classes/Inherit.hpp"
#include "../../Helpers/Mocks/ServiceProviderMock.hpp"
#include <SevenBit/DI/Details/Core/ServiceInstancesCreator.hpp>
#include <SevenBit/DI/ServiceDescriber.hpp>

class ServiceInstancesCreatorTest : public testing::Test
{
  protected:
    static void SetUpTestSuite() {}

    ServiceInstancesCreatorTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceInstancesCreatorTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ServiceInstancesCreatorTest, ShouldFailForNullProvider)
{
    ServiceProviderMock mock;
    sb::di::details::ServiceInstancesCreator creator;

    const auto descriptor = sb::di::ServiceDescriber::describeSingleton<TestClass1>();

    EXPECT_THROW(creator.create(descriptor), sb::di::NullPointerException);
}

TEST_F(ServiceInstancesCreatorTest, ShouldFailForInvalidInstance)
{
    ServiceProviderMock mock;
    sb::di::details::ServiceInstancesCreator creator;
    creator.setServiceProvider(mock);

    const auto descriptor = sb::di::ServiceDescriber::describeSingleton<TestClass1, TestClass1>(nullptr);

    EXPECT_THROW(creator.create(descriptor), sb::di::InvalidServiceException);
}

TEST_F(ServiceInstancesCreatorTest, ShouldFailFoWrongDescriptor)
{
    ServiceProviderMock mock;
    sb::di::details::ServiceInstancesCreator creator;
    creator.setServiceProvider(mock);

    const auto descriptor = sb::di::ServiceDescriber::describeAlias<TestInheritClass3, TestInheritClass5>();

    EXPECT_THROW(creator.create(descriptor), sb::di::InjectorException);
}

TEST_F(ServiceInstancesCreatorTest, ShouldFailForCircularDependency)
{
    ServiceProviderMock mock;
    sb::di::details::ServiceInstancesCreator creator;
    creator.setServiceProvider(mock);

    EXPECT_CALL(mock.getMock(), tryGetInstance(sb::di::TypeId{typeid(CircularDependencyA)}))
        .WillOnce(testing::Invoke([&](sb::di::TypeId typeId) {
            const auto descriptor = sb::di::ServiceDescriber::describeSingleton<CircularDependencyA>();
            creator.create(descriptor);
            return nullptr;
        }));

    EXPECT_CALL(mock.getMock(), tryGetInstance(sb::di::TypeId{typeid(CircularDependencyB)}))
        .WillOnce(testing::Invoke([&](sb::di::TypeId typeId) {
            const auto descriptor = sb::di::ServiceDescriber::describeSingleton<CircularDependencyB>();
            creator.create(descriptor);
            return nullptr;
        }));

    const auto descriptor = sb::di::ServiceDescriber::describeSingleton<CircularDependencyA>();

    EXPECT_THROW(creator.create(descriptor), sb::di::CircularDependencyException);
}

TEST_F(ServiceInstancesCreatorTest, ShouldCreateInstance)
{
    ServiceProviderMock mock;
    sb::di::details::ServiceInstancesCreator creator;
    creator.setServiceProvider(mock);

    const auto instance = creator.create(sb::di::ServiceDescriber::describeSingleton<TestClass1>());

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance.isValid());
    EXPECT_TRUE(instance.getAs<void>());
    EXPECT_EQ(instance.tryGetImplementation()->getTypeId(), typeid(TestClass1));
}

TEST_F(ServiceInstancesCreatorTest, ShouldCreateInheritedInstance)
{
    ServiceProviderMock mock;
    sb::di::details::ServiceInstancesCreator creator;
    creator.setServiceProvider(mock);

    const auto instance =
        creator.create(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance.isValid());
    EXPECT_TRUE(instance.getAs<void>());
    EXPECT_EQ(instance.tryGetImplementation()->getTypeId(), typeid(TestInheritClass4));
}

TEST_F(ServiceInstancesCreatorTest, ShouldCreateAllInstances)
{
    ServiceProviderMock mock;
    sb::di::details::ServiceInstancesCreator creator;
    creator.setServiceProvider(mock);

    const sb::di::details::ServiceDescriptorList descriptors{sb::di::ServiceDescriber::describeSingleton<TestClass1>()};
    const auto instances = creator.createAll(descriptors);

    EXPECT_EQ(instances.size(), 1);
    EXPECT_FALSE(instances.isSealed());
    EXPECT_TRUE(instances[0].isValid());
    EXPECT_TRUE(instances[0].getAs<void>());
    EXPECT_EQ(instances[0].tryGetImplementation()->getTypeId(), typeid(TestClass1));
}

TEST_F(ServiceInstancesCreatorTest, ShouldCreateAllInheritedInstances)
{
    ServiceProviderMock mock;
    sb::di::details::ServiceInstancesCreator creator;
    creator.setServiceProvider(mock);

    sb::di::details::ServiceDescriptorList descriptors{
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>()};
    descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
    descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    const auto instances = creator.createAll(descriptors);

    EXPECT_EQ(instances.size(), 3);
    EXPECT_FALSE(instances.isSealed());
    EXPECT_TRUE(instances[0].isValid());
    EXPECT_TRUE(instances[0].getAs<void>());
    EXPECT_EQ(instances[0].tryGetImplementation()->getTypeId(), typeid(TestInheritClass3));
    EXPECT_TRUE(instances[1].isValid());
    EXPECT_TRUE(instances[1].getAs<void>());
    EXPECT_EQ(instances[1].tryGetImplementation()->getTypeId(), typeid(TestInheritClass5));
    EXPECT_TRUE(instances[2].isValid());
    EXPECT_TRUE(instances[2].getAs<void>());
    EXPECT_EQ(instances[2].tryGetImplementation()->getTypeId(), typeid(TestInheritClass4));
}

TEST_F(ServiceInstancesCreatorTest, ShouldCreateEmptyInstances)
{
    ServiceProviderMock mock;
    sb::di::details::ServiceInstancesCreator creator;
    creator.setServiceProvider(mock);

    const sb::di::details::ServiceDescriptorList descriptors;
    const auto instances = creator.createAll(descriptors);

    EXPECT_TRUE(instances.empty());
    EXPECT_FALSE(instances.isSealed());
}

TEST_F(ServiceInstancesCreatorTest, ShouldCreateSkippedInheritedInstances)
{
    ServiceProviderMock mock;
    sb::di::details::ServiceInstancesCreator creator;
    creator.setServiceProvider(mock);

    sb::di::details::ServiceDescriptorList descriptors{
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>()};
    descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
    descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    auto instances = creator.createAll(descriptors, 1);

    EXPECT_EQ(instances.size(), 2);
    EXPECT_FALSE(instances.isSealed());
    EXPECT_TRUE(instances[0].isValid());
    EXPECT_TRUE(instances[0].getAs<void>());
    EXPECT_EQ(instances[0].tryGetImplementation()->getTypeId(), typeid(TestInheritClass3));
    EXPECT_TRUE(instances[1].isValid());
    EXPECT_TRUE(instances[1].getAs<void>());
    EXPECT_EQ(instances[1].tryGetImplementation()->getTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceInstancesCreatorTest, ShouldCreateInstanceInPlace)
{
    ServiceProviderMock mock;
    sb::di::details::ServiceInstancesCreator creator;
    creator.setServiceProvider(mock);

    const auto instance = creator.createInPlace(sb::di::ServiceDescriber::describeSingleton<TestClass1>());

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance.isValid());
    EXPECT_TRUE(instance.getAs<void>());
    EXPECT_EQ(instance.tryGetImplementation()->getTypeId(), typeid(TestClass1));
}

TEST_F(ServiceInstancesCreatorTest, ShouldCreateInheritedInstanceInPlace)
{
    ServiceProviderMock mock;
    sb::di::details::ServiceInstancesCreator creator;
    creator.setServiceProvider(mock);

    const auto instance =
        creator.createInPlace(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance.isValid());
    EXPECT_TRUE(instance.getAs<void>());
    EXPECT_EQ(instance.tryGetImplementation()->getTypeId(), typeid(TestInheritClass4));
}

TEST_F(ServiceInstancesCreatorTest, ShouldCreateAllInstancesInPlace)
{
    ServiceProviderMock mock;
    sb::di::details::ServiceInstancesCreator creator;
    creator.setServiceProvider(mock);

    const sb::di::details::ServiceDescriptorList descriptors{sb::di::ServiceDescriber::describeSingleton<TestClass1>()};
    const auto instances = creator.createAllInPlace(descriptors);

    EXPECT_EQ(instances.size(), 1);
    EXPECT_FALSE(instances.isSealed());
    EXPECT_TRUE(instances[0].isValid());
    EXPECT_TRUE(instances[0].getAs<void>());
    EXPECT_EQ(instances[0].tryGetImplementation()->getTypeId(), typeid(TestClass1));
}

TEST_F(ServiceInstancesCreatorTest, ShouldCreateAllInheritedInstancesInPlace)
{
    ServiceProviderMock mock;
    sb::di::details::ServiceInstancesCreator creator;
    creator.setServiceProvider(mock);

    sb::di::details::ServiceDescriptorList descriptors{
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>()};
    descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
    descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
    const auto instances = creator.createAllInPlace(descriptors);

    EXPECT_EQ(instances.size(), 3);
    EXPECT_FALSE(instances.isSealed());
    EXPECT_TRUE(instances[0].isValid());
    EXPECT_TRUE(instances[0].getAs<void>());
    EXPECT_EQ(instances[0].tryGetImplementation()->getTypeId(), typeid(TestInheritClass3));
    EXPECT_TRUE(instances[1].isValid());
    EXPECT_TRUE(instances[1].getAs<void>());
    EXPECT_EQ(instances[1].tryGetImplementation()->getTypeId(), typeid(TestInheritClass5));
    EXPECT_TRUE(instances[2].isValid());
    EXPECT_TRUE(instances[2].getAs<void>());
    EXPECT_EQ(instances[2].tryGetImplementation()->getTypeId(), typeid(TestInheritClass4));
}

TEST_F(ServiceInstancesCreatorTest, ShouldCreateSkippedInheritedInstancesInPlace)
{
    ServiceProviderMock mock;
    sb::di::details::ServiceInstancesCreator creator;
    creator.setServiceProvider(mock);

    sb::di::details::ServiceDescriptorList descriptors{
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>()};
    descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
    descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    auto instances = creator.createAll(descriptors, 1);

    EXPECT_EQ(instances.size(), 2);
    EXPECT_FALSE(instances.isSealed());
    EXPECT_TRUE(instances[0].isValid());
    EXPECT_TRUE(instances[0].getAs<void>());
    EXPECT_EQ(instances[0].tryGetImplementation()->getTypeId(), typeid(TestInheritClass3));
    EXPECT_TRUE(instances[1].isValid());
    EXPECT_TRUE(instances[1].getAs<void>());
    EXPECT_EQ(instances[1].tryGetImplementation()->getTypeId(), typeid(TestInheritClass5));
}
