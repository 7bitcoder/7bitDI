#include <gtest/gtest.h>
#include <memory>

#include "../../Helpers/Classes/Basic.hpp"
#include "../../Helpers/Classes/CirularDependency.hpp"
#include "../../Helpers/Classes/Inherit.hpp"
#include "../../Helpers/Mocks/ServiceProviderMock.hpp"
#include "SevenBit/DI/Details/Containers/ServiceDescriptorList.hpp"
#include "SevenBit/DI/Details/Core/ServiceInstancesResolver.hpp"
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
    const sb::di::details::containers::ServiceDescriptorList descriptors{
        sb::di::ServiceDescriber::describeSingleton<TestClass1>()};

    const sb::di::details::core::ServiceInstancesResolver resolver{creator, descriptors};

    const auto instance = resolver.createInstance();

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_EQ(instance->getTypeId(), typeid(TestClass1));
}

TEST_F(ServiceInstanceResolverTest, ShouldCreateInheritedInstance)
{
    ServiceProviderMock mock;
    sb::di::details::core::ServiceInstanceCreator creator;
    creator.setServiceProvider(mock);
    sb::di::details::containers::ServiceDescriptorList descriptors{
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>()};
    descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
    descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    const sb::di::details::core::ServiceInstancesResolver resolver{creator, descriptors};

    const auto instance = resolver.createInstance();

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_EQ(instance->getTypeId(), typeid(TestInheritClass4));
}

TEST_F(ServiceInstanceResolverTest, ShouldCreateOneInstance)
{
    ServiceProviderMock mock;
    sb::di::details::core::ServiceInstanceCreator creator;
    creator.setServiceProvider(mock);
    const sb::di::details::containers::ServiceDescriptorList descriptors{
        sb::di::ServiceDescriber::describeSingleton<TestClass1>()};

    const sb::di::details::core::ServiceInstancesResolver resolver{creator, descriptors};

    const auto instances = resolver.createOneInstance();

    EXPECT_EQ(instances.size(), 1);
    EXPECT_TRUE(instances.isSealed());
    EXPECT_TRUE(instances.getInnerList()[0]->isValid());
    EXPECT_TRUE(instances.getInnerList()[0]->get());
    EXPECT_EQ(instances.getInnerList()[0]->getTypeId(), typeid(TestClass1));
}

TEST_F(ServiceInstanceResolverTest, ShouldCreateOneInheritedInstance)
{
    ServiceProviderMock mock;
    sb::di::details::core::ServiceInstanceCreator creator;
    creator.setServiceProvider(mock);
    sb::di::details::containers::ServiceDescriptorList descriptors{
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>()};
    descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
    descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    const sb::di::details::core::ServiceInstancesResolver resolver{creator, descriptors};

    const auto instances = resolver.createOneInstance();

    EXPECT_EQ(instances.size(), 1);
    EXPECT_FALSE(instances.isSealed());
    EXPECT_TRUE(instances.getInnerList()[0]->isValid());
    EXPECT_TRUE(instances.getInnerList()[0]->get());
    EXPECT_EQ(instances.getInnerList()[0]->getTypeId(), typeid(TestInheritClass4));
}

TEST_F(ServiceInstanceResolverTest, ShouldCreateAllInstances)
{
    ServiceProviderMock mock;
    sb::di::details::core::ServiceInstanceCreator creator;
    creator.setServiceProvider(mock);
    const sb::di::details::containers::ServiceDescriptorList descriptors{
        sb::di::ServiceDescriber::describeSingleton<TestClass1>()};

    const sb::di::details::core::ServiceInstancesResolver resolver{creator, descriptors};

    const auto instances = resolver.createAllInstances();

    EXPECT_EQ(instances.size(), 1);
    EXPECT_TRUE(instances.isSealed());
    EXPECT_TRUE(instances.getInnerList()[0]->isValid());
    EXPECT_TRUE(instances.getInnerList()[0]->get());
    EXPECT_EQ(instances.getInnerList()[0]->getTypeId(), typeid(TestClass1));
}

TEST_F(ServiceInstanceResolverTest, ShouldCreateAllInheritedInstances)
{
    ServiceProviderMock mock;
    sb::di::details::core::ServiceInstanceCreator creator;
    creator.setServiceProvider(mock);
    sb::di::details::containers::ServiceDescriptorList descriptors{
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>()};
    descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
    descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    const sb::di::details::core::ServiceInstancesResolver resolver{creator, descriptors};

    const auto instances = resolver.createAllInstances();

    EXPECT_EQ(instances.size(), 3);
    EXPECT_TRUE(instances.isSealed());
    EXPECT_TRUE(instances.getInnerList()[0]->isValid());
    EXPECT_TRUE(instances.getInnerList()[0]->get());
    EXPECT_EQ(instances.getInnerList()[0]->getTypeId(), typeid(TestInheritClass3));
    EXPECT_TRUE(instances.getInnerList()[1]->isValid());
    EXPECT_TRUE(instances.getInnerList()[1]->get());
    EXPECT_EQ(instances.getInnerList()[1]->getTypeId(), typeid(TestInheritClass5));
    EXPECT_TRUE(instances.getInnerList()[2]->isValid());
    EXPECT_TRUE(instances.getInnerList()[2]->get());
    EXPECT_EQ(instances.getInnerList()[2]->getTypeId(), typeid(TestInheritClass4));
}

TEST_F(ServiceInstanceResolverTest, ShouldCreateRestInheritedInstances)
{
    ServiceProviderMock mock;
    sb::di::details::core::ServiceInstanceCreator creator;
    creator.setServiceProvider(mock);
    sb::di::details::containers::ServiceDescriptorList descriptors{
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>()};
    descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
    descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    const sb::di::details::core::ServiceInstancesResolver resolver{creator, descriptors};

    sb::di::details::containers::ServiceInstanceList instances{
        descriptors.last().getImplementationFactory().createInstance(mock, false)};
    resolver.createRestInstances(instances);

    EXPECT_EQ(instances.size(), 3);
    EXPECT_TRUE(instances.isSealed());
    EXPECT_TRUE(instances.getInnerList()[0]->isValid());
    EXPECT_TRUE(instances.getInnerList()[0]->get());
    EXPECT_EQ(instances.getInnerList()[0]->getTypeId(), typeid(TestInheritClass3));
    EXPECT_TRUE(instances.getInnerList()[1]->isValid());
    EXPECT_TRUE(instances.getInnerList()[1]->get());
    EXPECT_EQ(instances.getInnerList()[1]->getTypeId(), typeid(TestInheritClass5));
    EXPECT_TRUE(instances.getInnerList()[2]->isValid());
    EXPECT_TRUE(instances.getInnerList()[2]->get());
    EXPECT_EQ(instances.getInnerList()[2]->getTypeId(), typeid(TestInheritClass4));
}

TEST_F(ServiceInstanceResolverTest, ShouldCreateInstanceInPlace)
{
    ServiceProviderMock mock;
    sb::di::details::core::ServiceInstanceCreator creator;
    creator.setServiceProvider(mock);
    const sb::di::details::containers::ServiceDescriptorList descriptors{
        sb::di::ServiceDescriber::describeSingleton<TestClass1>()};

    const sb::di::details::core::ServiceInstancesResolver resolver{creator, descriptors};

    const auto instance = resolver.createInstanceInPlace();

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_EQ(instance->getTypeId(), typeid(TestClass1));
}

TEST_F(ServiceInstanceResolverTest, ShouldCreateInheritedInstanceInPlace)
{
    ServiceProviderMock mock;
    sb::di::details::core::ServiceInstanceCreator creator;
    creator.setServiceProvider(mock);
    sb::di::details::containers::ServiceDescriptorList descriptors{
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>()};
    descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
    descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    const sb::di::details::core::ServiceInstancesResolver resolver{creator, descriptors};

    const auto instance = resolver.createInstanceInPlace();

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_EQ(instance->getTypeId(), typeid(TestInheritClass4));
}

TEST_F(ServiceInstanceResolverTest, ShouldCreateOneInstanceInPlace)
{
    ServiceProviderMock mock;
    sb::di::details::core::ServiceInstanceCreator creator;
    creator.setServiceProvider(mock);
    const sb::di::details::containers::ServiceDescriptorList descriptors{
        sb::di::ServiceDescriber::describeSingleton<TestClass1>()};

    const sb::di::details::core::ServiceInstancesResolver resolver{creator, descriptors};

    const auto instances = resolver.createOneInstanceInPlace();

    EXPECT_EQ(instances.size(), 1);
    EXPECT_TRUE(instances.isSealed());
    EXPECT_TRUE(instances.getInnerList()[0]->isValid());
    EXPECT_TRUE(instances.getInnerList()[0]->get());
    EXPECT_EQ(instances.getInnerList()[0]->getTypeId(), typeid(TestClass1));
}

TEST_F(ServiceInstanceResolverTest, ShouldCreateOneInheritedInstanceInPlace)
{
    ServiceProviderMock mock;
    sb::di::details::core::ServiceInstanceCreator creator;
    creator.setServiceProvider(mock);
    sb::di::details::containers::ServiceDescriptorList descriptors{
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>()};
    descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
    descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    const sb::di::details::core::ServiceInstancesResolver resolver{creator, descriptors};

    const auto instances = resolver.createOneInstanceInPlace();

    EXPECT_EQ(instances.size(), 1);
    EXPECT_FALSE(instances.isSealed());
    EXPECT_TRUE(instances.getInnerList()[0]->isValid());
    EXPECT_TRUE(instances.getInnerList()[0]->get());
    EXPECT_EQ(instances.getInnerList()[0]->getTypeId(), typeid(TestInheritClass4));
}

TEST_F(ServiceInstanceResolverTest, ShouldCreateAllInstancesInPlace)
{
    ServiceProviderMock mock;
    sb::di::details::core::ServiceInstanceCreator creator;
    creator.setServiceProvider(mock);
    const sb::di::details::containers::ServiceDescriptorList descriptors{
        sb::di::ServiceDescriber::describeSingleton<TestClass1>()};

    const sb::di::details::core::ServiceInstancesResolver resolver{creator, descriptors};

    const auto instances = resolver.createAllInstancesInPlace();

    EXPECT_EQ(instances.size(), 1);
    EXPECT_TRUE(instances.isSealed());
    EXPECT_TRUE(instances.getInnerList()[0]->isValid());
    EXPECT_TRUE(instances.getInnerList()[0]->get());
    EXPECT_EQ(instances.getInnerList()[0]->getTypeId(), typeid(TestClass1));
}

TEST_F(ServiceInstanceResolverTest, ShouldCreateAllInheritedInstancesInPlace)
{
    ServiceProviderMock mock;
    sb::di::details::core::ServiceInstanceCreator creator;
    creator.setServiceProvider(mock);
    sb::di::details::containers::ServiceDescriptorList descriptors{
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>()};
    descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
    descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    const sb::di::details::core::ServiceInstancesResolver resolver{creator, descriptors};

    const auto instances = resolver.createAllInstancesInPlace();

    EXPECT_EQ(instances.size(), 3);
    EXPECT_TRUE(instances.isSealed());
    EXPECT_TRUE(instances.getInnerList()[0]->isValid());
    EXPECT_TRUE(instances.getInnerList()[0]->get());
    EXPECT_EQ(instances.getInnerList()[0]->getTypeId(), typeid(TestInheritClass3));
    EXPECT_TRUE(instances.getInnerList()[1]->isValid());
    EXPECT_TRUE(instances.getInnerList()[1]->get());
    EXPECT_EQ(instances.getInnerList()[1]->getTypeId(), typeid(TestInheritClass5));
    EXPECT_TRUE(instances.getInnerList()[2]->isValid());
    EXPECT_TRUE(instances.getInnerList()[2]->get());
    EXPECT_EQ(instances.getInnerList()[2]->getTypeId(), typeid(TestInheritClass4));
}

TEST_F(ServiceInstanceResolverTest, ShouldCreateRestInheritedInstancesInPlace)
{
    ServiceProviderMock mock;
    sb::di::details::core::ServiceInstanceCreator creator;
    creator.setServiceProvider(mock);
    sb::di::details::containers::ServiceDescriptorList descriptors{
        sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>()};
    descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
    descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());

    const sb::di::details::core::ServiceInstancesResolver resolver{creator, descriptors};

    sb::di::details::containers::ServiceInstanceList instances{
        descriptors.last().getImplementationFactory().createInstance(mock, true)};
    resolver.createRestInstancesInPlace(instances);

    EXPECT_EQ(instances.size(), 3);
    EXPECT_TRUE(instances.isSealed());
    EXPECT_TRUE(instances.getInnerList()[0]->isValid());
    EXPECT_TRUE(instances.getInnerList()[0]->get());
    EXPECT_EQ(instances.getInnerList()[0]->getTypeId(), typeid(TestInheritClass3));
    EXPECT_TRUE(instances.getInnerList()[1]->isValid());
    EXPECT_TRUE(instances.getInnerList()[1]->get());
    EXPECT_EQ(instances.getInnerList()[1]->getTypeId(), typeid(TestInheritClass5));
    EXPECT_TRUE(instances.getInnerList()[2]->isValid());
    EXPECT_TRUE(instances.getInnerList()[2]->get());
    EXPECT_EQ(instances.getInnerList()[2]->getTypeId(), typeid(TestInheritClass4));
}

TEST_F(ServiceInstanceResolverTest, ShouldCreateAlias)
{
    ServiceProviderMock mock;
    sb::di::details::core::ServiceInstanceCreator creator;
    creator.setServiceProvider(mock);
    sb::di::details::containers::ServiceDescriptorList descriptors{
        sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass3>()};
    descriptors.add(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass4>());
    descriptors.add(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass5>());

    const sb::di::details::core::ServiceInstancesResolver resolver{creator, descriptors};

    TestInheritClass6 test;
    const sb::di::details::services::ExternalService external(&test);
    const auto instance = resolver.createAlias(external);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_EQ(instance->getTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceInstanceResolverTest, ShouldCreateOneAlias)
{
    ServiceProviderMock mock;
    sb::di::details::core::ServiceInstanceCreator creator;
    creator.setServiceProvider(mock);
    sb::di::details::containers::ServiceDescriptorList descriptors{
        sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass3>()};
    descriptors.add(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass4>());
    descriptors.add(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass5>());

    const sb::di::details::core::ServiceInstancesResolver resolver{creator, descriptors};

    TestInheritClass6 test;
    const sb::di::details::services::ExternalService external(&test);
    const auto instances = resolver.createOneAlias(external);

    EXPECT_EQ(instances.size(), 1);
    EXPECT_FALSE(instances.isSealed());
    EXPECT_TRUE(instances.getInnerList()[0]->isValid());
    EXPECT_TRUE(instances.getInnerList()[0]->get());
    EXPECT_EQ(instances.getInnerList()[0]->getTypeId(), typeid(TestInheritClass5));
}

TEST_F(ServiceInstanceResolverTest, ShouldCreateAllAliases)
{
    ServiceProviderMock mock;
    sb::di::details::core::ServiceInstanceCreator creator;
    creator.setServiceProvider(mock);
    sb::di::details::containers::ServiceDescriptorList descriptors{
        sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass2>()};

    const sb::di::details::core::ServiceInstancesResolver resolver{creator, descriptors};

    TestInheritClass3 test3;
    TestInheritClass4 test4;
    TestInheritClass5 test5;
    sb::di::details::containers::ServiceInstanceList externals{
        std::make_unique<sb::di::details::services::ExternalService<TestInheritClass3>>(&test3)};
    externals.add(std::make_unique<sb::di::details::services::ExternalService<TestInheritClass4>>(&test4));
    externals.add(std::make_unique<sb::di::details::services::ExternalService<TestInheritClass5>>(&test5));

    const auto instances = resolver.createAllAliases(externals.getInnerList());

    EXPECT_EQ(instances.size(), 3);
    EXPECT_TRUE(instances.isSealed());
    EXPECT_TRUE(instances.getInnerList()[0]->isValid());
    EXPECT_TRUE(instances.getInnerList()[0]->get());
    EXPECT_EQ(instances.getInnerList()[0]->getTypeId(), typeid(TestInheritClass2));
    EXPECT_TRUE(instances.getInnerList()[1]->isValid());
    EXPECT_TRUE(instances.getInnerList()[1]->get());
    EXPECT_EQ(instances.getInnerList()[1]->getTypeId(), typeid(TestInheritClass2));
    EXPECT_TRUE(instances.getInnerList()[2]->isValid());
    EXPECT_TRUE(instances.getInnerList()[2]->get());
    EXPECT_EQ(instances.getInnerList()[2]->getTypeId(), typeid(TestInheritClass2));
}

TEST_F(ServiceInstanceResolverTest, ShouldCreateRestAliases)
{
    ServiceProviderMock mock;
    sb::di::details::core::ServiceInstanceCreator creator;
    creator.setServiceProvider(mock);
    sb::di::details::containers::ServiceDescriptorList descriptors{
        sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass2>()};

    const sb::di::details::core::ServiceInstancesResolver resolver{creator, descriptors};

    TestInheritClass3 test3;
    TestInheritClass4 test4;
    TestInheritClass5 test5;
    sb::di::details::containers::ServiceInstanceList externals{
        std::make_unique<sb::di::details::services::ExternalService<TestInheritClass3>>(&test3)};
    externals.add(std::make_unique<sb::di::details::services::ExternalService<TestInheritClass4>>(&test4));
    externals.add(std::make_unique<sb::di::details::services::ExternalService<TestInheritClass5>>(&test5));

    sb::di::details::containers::ServiceInstanceList instances{
        std::make_unique<sb::di::details::services::ExternalService<TestInheritClass2>>(&test5)};

    auto &_ = resolver.createRestAliases(externals.getInnerList(), instances);

    EXPECT_EQ(instances.size(), 3);
    EXPECT_TRUE(instances.isSealed());
    EXPECT_TRUE(instances.getInnerList()[0]->isValid());
    EXPECT_TRUE(instances.getInnerList()[0]->get());
    EXPECT_EQ(instances.getInnerList()[0]->getTypeId(), typeid(TestInheritClass2));
    EXPECT_TRUE(instances.getInnerList()[1]->isValid());
    EXPECT_TRUE(instances.getInnerList()[1]->get());
    EXPECT_EQ(instances.getInnerList()[1]->getTypeId(), typeid(TestInheritClass2));
    EXPECT_TRUE(instances.getInnerList()[2]->isValid());
    EXPECT_TRUE(instances.getInnerList()[2]->get());
    EXPECT_EQ(instances.getInnerList()[2]->getTypeId(), typeid(TestInheritClass2));
}
