// #include <gtest/gtest.h>
// #include <memory>
//
// #include "../../Helpers/Classes/Basic.hpp"
// #include "../../Helpers/Classes/Inherit.hpp"
// #include "../../Helpers/Mocks/ServiceProviderMock.hpp"
// #include "SevenBit/DI/Details/Core/ServiceInstancesCreator.hpp"
// #include "SevenBit/DI/ServiceDescriber.hpp"
//
// class ServiceInstanceResolverTest : public testing::Test
// {
//   protected:
//     static void SetUpTestSuite() {}
//
//     ServiceInstanceResolverTest() {}
//
//     void SetUp() override {}
//
//     void TearDown() override {}
//
//     ~ServiceInstanceResolverTest() override = default;
//
//     static void TearDownTestSuite() {}
// };
//
// TEST_F(ServiceInstanceResolverTest, ShouldCreateInstance)
// {
//     ServiceProviderMock mock;
//     sb::di::details::ServiceInstanceCreator2 creator;
//     creator.setServiceProvider(mock);
//     const sb::di::details::ServiceDescriptorList
//     descriptors{sb::di::ServiceDescriber::describeSingleton<TestClass1>()};
//
//     const sb::di::details::ServiceInstancesResolver resolver{creator, descriptors};
//
//     const auto instance = resolver.create();
//
//     EXPECT_TRUE(instance);
//     EXPECT_TRUE(instance.isValid());
//     EXPECT_TRUE(instance.getAs<void>());
//     EXPECT_EQ(instance.tryGetImplementation()->getTypeId(), typeid(TestClass1));
// }
//
// TEST_F(ServiceInstanceResolverTest, ShouldCreateInheritedInstance)
// {
//     ServiceProviderMock mock;
//     sb::di::details::ServiceInstanceCreator2 creator;
//     creator.setServiceProvider(mock);
//     sb::di::details::ServiceDescriptorList descriptors{
//         sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>()};
//     descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
//     descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
//
//     const sb::di::details::ServiceInstancesResolver resolver{creator, descriptors};
//
//     const auto instance = resolver.create();
//
//     EXPECT_TRUE(instance);
//     EXPECT_TRUE(instance.isValid());
//     EXPECT_TRUE(instance.getAs<void>());
//     EXPECT_EQ(instance.tryGetImplementation()->getTypeId(), typeid(TestInheritClass4));
// }
//
// TEST_F(ServiceInstanceResolverTest, ShouldCreateOneInstance)
// {
//     ServiceProviderMock mock;
//     sb::di::details::ServiceInstanceCreator2 creator;
//     creator.setServiceProvider(mock);
//     const sb::di::details::ServiceDescriptorList
//     descriptors{sb::di::ServiceDescriber::describeSingleton<TestClass1>()};
//
//     const sb::di::details::ServiceInstancesResolver resolver{creator, descriptors};
//
//     const auto instances = resolver.createOne();
//
//     EXPECT_EQ(instances.size(), 1);
//     EXPECT_TRUE(instances.isSealed());
//     EXPECT_TRUE(instances.getInnerList()[0].isValid());
//     EXPECT_TRUE(instances.getInnerList()[0].getAs<void>());
//     EXPECT_EQ(instances.getInnerList()[0].tryGetImplementation()->getTypeId(), typeid(TestClass1));
// }
//
// TEST_F(ServiceInstanceResolverTest, ShouldCreateOneInheritedInstance)
// {
//     ServiceProviderMock mock;
//     sb::di::details::ServiceInstanceCreator2 creator;
//     creator.setServiceProvider(mock);
//     sb::di::details::ServiceDescriptorList descriptors{
//         sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>()};
//     descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
//     descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
//
//     const sb::di::details::ServiceInstancesResolver resolver{creator, descriptors};
//
//     const auto instances = resolver.createOne();
//
//     EXPECT_EQ(instances.size(), 1);
//     EXPECT_FALSE(instances.isSealed());
//     EXPECT_TRUE(instances.getInnerList()[0].isValid());
//     EXPECT_TRUE(instances.getInnerList()[0].getAs<void>());
//     EXPECT_EQ(instances.getInnerList()[0].tryGetImplementation()->getTypeId(), typeid(TestInheritClass4));
// }
//
// TEST_F(ServiceInstanceResolverTest, ShouldCreateAllInstances)
// {
//     ServiceProviderMock mock;
//     sb::di::details::ServiceInstanceCreator2 creator;
//     creator.setServiceProvider(mock);
//     const sb::di::details::ServiceDescriptorList
//     descriptors{sb::di::ServiceDescriber::describeSingleton<TestClass1>()};
//
//     const sb::di::details::ServiceInstancesResolver resolver{creator, descriptors};
//
//     const auto instances = resolver.createAll();
//
//     EXPECT_EQ(instances.size(), 1);
//     EXPECT_TRUE(instances.isSealed());
//     EXPECT_TRUE(instances.getInnerList()[0].isValid());
//     EXPECT_TRUE(instances.getInnerList()[0].getAs<void>());
//     EXPECT_EQ(instances.getInnerList()[0].tryGetImplementation()->getTypeId(), typeid(TestClass1));
// }
//
// TEST_F(ServiceInstanceResolverTest, ShouldCreateAllInheritedInstances)
// {
//     ServiceProviderMock mock;
//     sb::di::details::ServiceInstanceCreator2 creator;
//     creator.setServiceProvider(mock);
//     sb::di::details::ServiceDescriptorList descriptors{
//         sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>()};
//     descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
//     descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
//
//     const sb::di::details::ServiceInstancesResolver resolver{creator, descriptors};
//
//     const auto instances = resolver.createAll();
//
//     EXPECT_EQ(instances.size(), 3);
//     EXPECT_TRUE(instances.isSealed());
//     EXPECT_TRUE(instances.getInnerList()[0].isValid());
//     EXPECT_TRUE(instances.getInnerList()[0].getAs<void>());
//     EXPECT_EQ(instances.getInnerList()[0].tryGetImplementation()->getTypeId(), typeid(TestInheritClass3));
//     EXPECT_TRUE(instances.getInnerList()[1].isValid());
//     EXPECT_TRUE(instances.getInnerList()[1].getAs<void>());
//     EXPECT_EQ(instances.getInnerList()[1].tryGetImplementation()->getTypeId(), typeid(TestInheritClass5));
//     EXPECT_TRUE(instances.getInnerList()[2].isValid());
//     EXPECT_TRUE(instances.getInnerList()[2].getAs<void>());
//     EXPECT_EQ(instances.getInnerList()[2].tryGetImplementation()->getTypeId(), typeid(TestInheritClass4));
// }
//
// TEST_F(ServiceInstanceResolverTest, ShouldCreateRestInheritedInstances)
// {
//     ServiceProviderMock mock;
//     sb::di::details::ServiceInstanceCreator2 creator;
//     creator.setServiceProvider(mock);
//     sb::di::details::ServiceDescriptorList descriptors{
//         sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>()};
//     descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
//     descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
//
//     const sb::di::details::ServiceInstancesResolver resolver{creator, descriptors};
//
//     sb::di::details::ServiceInstanceList instances{
//         sb::di::ServiceInstance{descriptors.last().getImplementationFactory()->createInstance(mock, false)}};
//     resolver.createRest(instances);
//
//     EXPECT_EQ(instances.size(), 3);
//     EXPECT_TRUE(instances.isSealed());
//     EXPECT_TRUE(instances.getInnerList()[0].isValid());
//     EXPECT_TRUE(instances.getInnerList()[0].getAs<void>());
//     EXPECT_EQ(instances.getInnerList()[0].tryGetImplementation()->getTypeId(), typeid(TestInheritClass3));
//     EXPECT_TRUE(instances.getInnerList()[1].isValid());
//     EXPECT_TRUE(instances.getInnerList()[1].getAs<void>());
//     EXPECT_EQ(instances.getInnerList()[1].tryGetImplementation()->getTypeId(), typeid(TestInheritClass5));
//     EXPECT_TRUE(instances.getInnerList()[2].isValid());
//     EXPECT_TRUE(instances.getInnerList()[2].getAs<void>());
//     EXPECT_EQ(instances.getInnerList()[2].tryGetImplementation()->getTypeId(), typeid(TestInheritClass4));
// }
//
// TEST_F(ServiceInstanceResolverTest, ShouldCreateInstanceInPlace)
// {
//     ServiceProviderMock mock;
//     sb::di::details::ServiceInstanceCreator2 creator;
//     creator.setServiceProvider(mock);
//     const sb::di::details::ServiceDescriptorList
//     descriptors{sb::di::ServiceDescriber::describeSingleton<TestClass1>()};
//
//     const sb::di::details::ServiceInstancesResolver resolver{creator, descriptors};
//
//     const auto instance = resolver.createInPlace();
//
//     EXPECT_TRUE(instance);
//     EXPECT_TRUE(instance.isValid());
//     EXPECT_TRUE(instance.getAs<void>());
//     EXPECT_EQ(instance.tryGetImplementation()->getTypeId(), typeid(TestClass1));
// }
//
// TEST_F(ServiceInstanceResolverTest, ShouldCreateInheritedInstanceInPlace)
// {
//     ServiceProviderMock mock;
//     sb::di::details::ServiceInstanceCreator2 creator;
//     creator.setServiceProvider(mock);
//     sb::di::details::ServiceDescriptorList descriptors{
//         sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>()};
//     descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
//     descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
//
//     const sb::di::details::ServiceInstancesResolver resolver{creator, descriptors};
//
//     const auto instance = resolver.createInPlace();
//
//     EXPECT_TRUE(instance);
//     EXPECT_TRUE(instance.isValid());
//     EXPECT_TRUE(instance.getAs<void>());
//     EXPECT_EQ(instance.tryGetImplementation()->getTypeId(), typeid(TestInheritClass4));
// }
//
// TEST_F(ServiceInstanceResolverTest, ShouldCreateOneInstanceInPlace)
// {
//     ServiceProviderMock mock;
//     sb::di::details::ServiceInstanceCreator2 creator;
//     creator.setServiceProvider(mock);
//     const sb::di::details::ServiceDescriptorList
//     descriptors{sb::di::ServiceDescriber::describeSingleton<TestClass1>()};
//
//     const sb::di::details::ServiceInstancesResolver resolver{creator, descriptors};
//
//     const auto instances = resolver.createOneInPlace();
//
//     EXPECT_EQ(instances.size(), 1);
//     EXPECT_TRUE(instances.isSealed());
//     EXPECT_TRUE(instances.getInnerList()[0].isValid());
//     EXPECT_TRUE(instances.getInnerList()[0].getAs<void>());
//     EXPECT_EQ(instances.getInnerList()[0].tryGetImplementation()->getTypeId(), typeid(TestClass1));
// }
//
// TEST_F(ServiceInstanceResolverTest, ShouldCreateOneInheritedInstanceInPlace)
// {
//     ServiceProviderMock mock;
//     sb::di::details::ServiceInstanceCreator2 creator;
//     creator.setServiceProvider(mock);
//     sb::di::details::ServiceDescriptorList descriptors{
//         sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>()};
//     descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
//     descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
//
//     const sb::di::details::ServiceInstancesResolver resolver{creator, descriptors};
//
//     const auto instances = resolver.createOneInPlace();
//
//     EXPECT_EQ(instances.size(), 1);
//     EXPECT_FALSE(instances.isSealed());
//     EXPECT_TRUE(instances.getInnerList()[0].isValid());
//     EXPECT_TRUE(instances.getInnerList()[0].getAs<void>());
//     EXPECT_EQ(instances.getInnerList()[0].tryGetImplementation()->getTypeId(), typeid(TestInheritClass4));
// }
//
// TEST_F(ServiceInstanceResolverTest, ShouldCreateAllInstancesInPlace)
// {
//     ServiceProviderMock mock;
//     sb::di::details::ServiceInstanceCreator2 creator;
//     creator.setServiceProvider(mock);
//     const sb::di::details::ServiceDescriptorList
//     descriptors{sb::di::ServiceDescriber::describeSingleton<TestClass1>()};
//
//     const sb::di::details::ServiceInstancesResolver resolver{creator, descriptors};
//
//     const auto instances = resolver.createAllInPlace();
//
//     EXPECT_EQ(instances.size(), 1);
//     EXPECT_TRUE(instances.isSealed());
//     EXPECT_TRUE(instances.getInnerList()[0].isValid());
//     EXPECT_TRUE(instances.getInnerList()[0].getAs<void>());
//     EXPECT_EQ(instances.getInnerList()[0].tryGetImplementation()->getTypeId(), typeid(TestClass1));
// }
//
// TEST_F(ServiceInstanceResolverTest, ShouldCreateAllInheritedInstancesInPlace)
// {
//     ServiceProviderMock mock;
//     sb::di::details::ServiceInstanceCreator2 creator;
//     creator.setServiceProvider(mock);
//     sb::di::details::ServiceDescriptorList descriptors{
//         sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>()};
//     descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
//     descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
//
//     const sb::di::details::ServiceInstancesResolver resolver{creator, descriptors};
//
//     const auto instances = resolver.createAllInPlace();
//
//     EXPECT_EQ(instances.size(), 3);
//     EXPECT_TRUE(instances.isSealed());
//     EXPECT_TRUE(instances.getInnerList()[0].isValid());
//     EXPECT_TRUE(instances.getInnerList()[0].getAs<void>());
//     EXPECT_EQ(instances.getInnerList()[0].tryGetImplementation()->getTypeId(), typeid(TestInheritClass3));
//     EXPECT_TRUE(instances.getInnerList()[1].isValid());
//     EXPECT_TRUE(instances.getInnerList()[1].getAs<void>());
//     EXPECT_EQ(instances.getInnerList()[1].tryGetImplementation()->getTypeId(), typeid(TestInheritClass5));
//     EXPECT_TRUE(instances.getInnerList()[2].isValid());
//     EXPECT_TRUE(instances.getInnerList()[2].getAs<void>());
//     EXPECT_EQ(instances.getInnerList()[2].tryGetImplementation()->getTypeId(), typeid(TestInheritClass4));
// }
//
// TEST_F(ServiceInstanceResolverTest, ShouldCreateRestInheritedInstancesInPlace)
// {
//     ServiceProviderMock mock;
//     sb::di::details::ServiceInstanceCreator2 creator;
//     creator.setServiceProvider(mock);
//     sb::di::details::ServiceDescriptorList descriptors{
//         sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass3>()};
//     descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass5>());
//     descriptors.add(sb::di::ServiceDescriber::describeSingleton<TestInheritClass1, TestInheritClass4>());
//
//     const sb::di::details::ServiceInstancesResolver resolver{creator, descriptors};
//
//     sb::di::details::ServiceInstanceList instances{
//         sb::di::ServiceInstance{descriptors.last().getImplementationFactory()->createInstance(mock, true)}};
//     resolver.createRestInPlace(instances);
//
//     EXPECT_EQ(instances.size(), 3);
//     EXPECT_TRUE(instances.isSealed());
//     EXPECT_TRUE(instances.getInnerList()[0].isValid());
//     EXPECT_TRUE(instances.getInnerList()[0].getAs<void>());
//     EXPECT_EQ(instances.getInnerList()[0].tryGetImplementation()->getTypeId(), typeid(TestInheritClass3));
//     EXPECT_TRUE(instances.getInnerList()[1].isValid());
//     EXPECT_TRUE(instances.getInnerList()[1].getAs<void>());
//     EXPECT_EQ(instances.getInnerList()[1].tryGetImplementation()->getTypeId(), typeid(TestInheritClass5));
//     EXPECT_TRUE(instances.getInnerList()[2].isValid());
//     EXPECT_TRUE(instances.getInnerList()[2].getAs<void>());
//     EXPECT_EQ(instances.getInnerList()[2].tryGetImplementation()->getTypeId(), typeid(TestInheritClass4));
// }
//
// // TEST_F(ServiceInstanceResolverTest, ShouldCreateAlias)
// // {
// //     ServiceProviderMock mock;
// //     sb::di::details::ServiceInstanceCreator creator;
// //     creator.setServiceProvider(mock);
// //     sb::di::details::ServiceDescriptorList descriptors{
// //         sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass3>()};
// //     descriptors.add(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass4>());
// //     descriptors.add(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass5>());
// //
// //     const sb::di::details::ServiceInstancesResolver resolver{creator, descriptors};
// //
// //     TestInheritClass6 test;
// //     const sb::di::ServiceInstance external{
// //         std::make_unique<sb::di::details::ExternalService<TestInheritClass6>>(&test)};
// //
// //     const auto instance = resolver.createAlias(external);
// //
// //     EXPECT_TRUE(instance);
// //     EXPECT_TRUE(instance.isValid());
// //     EXPECT_TRUE(instance.getAs<void>());
// //     EXPECT_EQ(instance.tryGetImplementation()->getTypeId(), typeid(TestInheritClass5));
// // }
// //
// // TEST_F(ServiceInstanceResolverTest, ShouldCreateOneAlias)
// // {
// //     ServiceProviderMock mock;
// //     sb::di::details::ServiceInstanceCreator creator;
// //     creator.setServiceProvider(mock);
// //     sb::di::details::ServiceDescriptorList descriptors{
// //         sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass3>()};
// //     descriptors.add(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass4>());
// //     descriptors.add(sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass5>());
// //
// //     const sb::di::details::ServiceInstancesResolver resolver{creator, descriptors};
// //
// //     TestInheritClass6 test;
// //     sb::di::ServiceInstance
// external{std::make_unique<sb::di::details::ExternalService<TestInheritClass6>>(&test)};
// //
// //     const auto instances = resolver.createOneAlias(external);
// //
// //     EXPECT_EQ(instances.size(), 1);
// //     EXPECT_FALSE(instances.isSealed());
// //     EXPECT_TRUE(instances.getInnerList()[0].isValid());
// //     EXPECT_TRUE(instances.getInnerList()[0].getAs<void>());
// //     EXPECT_EQ(instances.getInnerList()[0].tryGetImplementation()->getTypeId(), typeid(TestInheritClass5));
// // }
// //
// // TEST_F(ServiceInstanceResolverTest, ShouldCreateAllAliases)
// // {
// //     ServiceProviderMock mock;
// //     sb::di::details::ServiceInstanceCreator creator;
// //     creator.setServiceProvider(mock);
// //     sb::di::details::ServiceDescriptorList descriptors{
// //         sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass2>()};
// //
// //     const sb::di::details::ServiceInstancesResolver resolver{creator, descriptors};
// //
// //     TestInheritClass3 test3;
// //     TestInheritClass4 test4;
// //     TestInheritClass5 test5;
// //     sb::di::details::ServiceInstanceList externals{
// //         sb::di::ServiceInstance{std::make_unique<sb::di::details::ExternalService<TestInheritClass3>>(&test3)}};
// //     externals.add(
// //         sb::di::ServiceInstance{std::make_unique<sb::di::details::ExternalService<TestInheritClass4>>(&test4)});
// //     externals.add(
// //         sb::di::ServiceInstance{std::make_unique<sb::di::details::ExternalService<TestInheritClass5>>(&test5)});
// //
// //     const auto instances = resolver.createAllAliases(externals.getInnerList());
// //
// //     EXPECT_EQ(instances.size(), 3);
// //     EXPECT_TRUE(instances.isSealed());
// //     EXPECT_TRUE(instances.getInnerList()[0].isValid());
// //     EXPECT_TRUE(instances.getInnerList()[0].getAs<void>());
// //     EXPECT_EQ(instances.getInnerList()[0].tryGetImplementation()->getTypeId(), typeid(TestInheritClass2));
// //     EXPECT_TRUE(instances.getInnerList()[1].isValid());
// //     EXPECT_TRUE(instances.getInnerList()[1].getAs<void>());
// //     EXPECT_EQ(instances.getInnerList()[1].tryGetImplementation()->getTypeId(), typeid(TestInheritClass2));
// //     EXPECT_TRUE(instances.getInnerList()[2].isValid());
// //     EXPECT_TRUE(instances.getInnerList()[2].getAs<void>());
// //     EXPECT_EQ(instances.getInnerList()[2].tryGetImplementation()->getTypeId(), typeid(TestInheritClass2));
// // }
// //
// // TEST_F(ServiceInstanceResolverTest, ShouldCreateRestAliases)
// // {
// //     ServiceProviderMock mock;
// //     sb::di::details::ServiceInstanceCreator creator;
// //     creator.setServiceProvider(mock);
// //     sb::di::details::ServiceDescriptorList descriptors{
// //         sb::di::ServiceDescriber::describeAlias<TestInheritClass1, TestInheritClass2>()};
// //
// //     const sb::di::details::ServiceInstancesResolver resolver{creator, descriptors};
// //
// //     TestInheritClass3 test3;
// //     TestInheritClass4 test4;
// //     TestInheritClass5 test5;
// //     sb::di::details::ServiceInstanceList externals{
// //         sb::di::ServiceInstance{std::make_unique<sb::di::details::ExternalService<TestInheritClass3>>(&test3)}};
// //     externals.add(
// //         sb::di::ServiceInstance{std::make_unique<sb::di::details::ExternalService<TestInheritClass4>>(&test4)});
// //     externals.add(
// //         sb::di::ServiceInstance{std::make_unique<sb::di::details::ExternalService<TestInheritClass5>>(&test5)});
// //
// //     sb::di::details::ServiceInstanceList instances{
// //         sb::di::ServiceInstance{std::make_unique<sb::di::details::ExternalService<TestInheritClass2>>(&test5)}};
// //
// //     auto &_ = resolver.createRestAliases(externals.getInnerList(), instances);
// //
// //     EXPECT_EQ(instances.size(), 3);
// //     EXPECT_TRUE(instances.isSealed());
// //     EXPECT_TRUE(instances.getInnerList()[0].isValid());
// //     EXPECT_TRUE(instances.getInnerList()[0].getAs<void>());
// //     EXPECT_EQ(instances.getInnerList()[0].tryGetImplementation()->getTypeId(), typeid(TestInheritClass2));
// //     EXPECT_TRUE(instances.getInnerList()[1].isValid());
// //     EXPECT_TRUE(instances.getInnerList()[1].getAs<void>());
// //     EXPECT_EQ(instances.getInnerList()[1].tryGetImplementation()->getTypeId(), typeid(TestInheritClass2));
// //     EXPECT_TRUE(instances.getInnerList()[2].isValid());
// //     EXPECT_TRUE(instances.getInnerList()[2].getAs<void>());
// //     EXPECT_EQ(instances.getInnerList()[2].tryGetImplementation()->getTypeId(), typeid(TestInheritClass2));
// // }
