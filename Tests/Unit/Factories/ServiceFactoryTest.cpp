#include <gtest/gtest.h>

#include "../../Helpers/Classes/Basic.hpp"
#include "../../Helpers/Classes/Complex.hpp"
#include "../../Helpers/Classes/Dependencies.hpp"
#include "../../Helpers/Mocks/ServiceProviderMock.hpp"
#include "SevenBit/DI/Details/Factories/ServiceFactory.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

class ServiceFactoryTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceFactoryTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceFactoryTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ServiceFactoryTest, ShouldCreateService)
{
    ServiceProviderMock mock;
    const sb::di::details::factories::ServiceFactory<TestClass1> factory;

    const auto instance = factory.createInstance(mock, false);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::UniquePtrService<TestClass1> *>(instance.get()));
}

TEST_F(ServiceFactoryTest, ShouldCreateInPlaceService)
{
    ServiceProviderMock mock;
    const sb::di::details::factories::ServiceFactory<TestClass1> factory;

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::InPlaceService<TestClass1> *>(instance.get()));
}

TEST_F(ServiceFactoryTest, ShouldCreateDependencyInPlaceService)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::services::InPlaceService<TestDependencyClass>>()};

    EXPECT_CALL(mock.getMock(), createInstanceInPlace(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(std::move(test1)));

    const sb::di::details::factories::ServiceFactory<TestDependencyInPlaceClass> factory;

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::InPlaceService<TestDependencyInPlaceClass> *>(instance.get()));
}

TEST_F(ServiceFactoryTest, ShouldCreateDependencyPtr1Service)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::services::InPlaceService<TestDependencyClass>>()};

    EXPECT_CALL(mock.getMock(), tryGetInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&test1));

    const sb::di::details::factories::ServiceFactory<TestDependencyPtrClass1> factory;

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::InPlaceService<TestDependencyPtrClass1> *>(instance.get()));
}

TEST_F(ServiceFactoryTest, ShouldCreateDependencyPtr2Service)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::services::InPlaceService<TestDependencyClass>>()};

    EXPECT_CALL(mock.getMock(), tryGetInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&test1));

    const sb::di::details::factories::ServiceFactory<TestDependencyPtrClass2> factory;

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::InPlaceService<TestDependencyPtrClass2> *>(instance.get()));
}

TEST_F(ServiceFactoryTest, ShouldCreateDependencyPtr3Service)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::services::InPlaceService<TestDependencyClass>>()};

    EXPECT_CALL(mock.getMock(), tryGetInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&test1));

    const sb::di::details::factories::ServiceFactory<TestDependencyPtrClass3> factory;

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::InPlaceService<TestDependencyPtrClass3> *>(instance.get()));
}

TEST_F(ServiceFactoryTest, ShouldCreateDependencyPtr4Service)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::services::InPlaceService<TestDependencyClass>>()};

    EXPECT_CALL(mock.getMock(), tryGetInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&test1));

    const sb::di::details::factories::ServiceFactory<TestDependencyPtrClass4> factory;

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::InPlaceService<TestDependencyPtrClass4> *>(instance.get()));
}

TEST_F(ServiceFactoryTest, ShouldCreateDependencyRef1Service)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::services::InPlaceService<TestDependencyClass>>()};

    EXPECT_CALL(mock.getMock(), getInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::ReturnRef(test1));

    const sb::di::details::factories::ServiceFactory<TestDependencyRefClass1> factory;

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::InPlaceService<TestDependencyRefClass1> *>(instance.get()));
}

TEST_F(ServiceFactoryTest, ShouldCreateDependencyRef2Service)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::services::InPlaceService<TestDependencyClass>>()};

    EXPECT_CALL(mock.getMock(), getInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::ReturnRef(test1));

    const sb::di::details::factories::ServiceFactory<TestDependencyRefClass2> factory;

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::InPlaceService<TestDependencyRefClass2> *>(instance.get()));
}

TEST_F(ServiceFactoryTest, ShouldCreateDependencyUniq1Service)
{
    ServiceProviderMock mock;
    auto service = std::make_unique<TestDependencyClass>();
    sb::di::ServiceInstance test1{
        std::make_unique<sb::di::details::services::UniquePtrService<TestDependencyClass>>(std::move(service))};

    EXPECT_CALL(mock.getMock(), createInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(std::move(test1)));

    const sb::di::details::factories::ServiceFactory<TestDependencyUniquePtrClass1> factory;

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(
        dynamic_cast<sb::di::details::services::InPlaceService<TestDependencyUniquePtrClass1> *>(instance.get()));
}

TEST_F(ServiceFactoryTest, ShouldCreateDependencyUniq2Service)
{
    ServiceProviderMock mock;
    auto service = std::make_unique<TestDependencyClass>();
    sb::di::ServiceInstance test1{
        std::make_unique<sb::di::details::services::UniquePtrService<TestDependencyClass>>(std::move(service))};

    EXPECT_CALL(mock.getMock(), createInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(std::move(test1)));

    const sb::di::details::factories::ServiceFactory<TestDependencyUniquePtrClass2> factory;

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(
        dynamic_cast<sb::di::details::services::InPlaceService<TestDependencyUniquePtrClass2> *>(instance.get()));
}

TEST_F(ServiceFactoryTest, ShouldCreateDependencyVec1Service)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::services::InPlaceService<TestDependencyClass>>()};

    sb::di::OneOrList result{std::move(test1)};

    EXPECT_CALL(mock.getMock(), tryGetInstances(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&result));

    const sb::di::details::factories::ServiceFactory<TestDependencyVecClass1> factory;

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::InPlaceService<TestDependencyVecClass1> *>(instance.get()));
}

TEST_F(ServiceFactoryTest, ShouldCreateDependencyVec2Service)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::services::InPlaceService<TestDependencyClass>>()};

    sb::di::OneOrList result{std::move(test1)};

    EXPECT_CALL(mock.getMock(), tryGetInstances(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&result));

    const sb::di::details::factories::ServiceFactory<TestDependencyVecClass2> factory;

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::InPlaceService<TestDependencyVecClass2> *>(instance.get()));
}

TEST_F(ServiceFactoryTest, ShouldCreateDependencyVec3Service)
{
    ServiceProviderMock mock;
    auto service = std::make_unique<TestDependencyClass>();
    sb::di::ServiceInstance test1{
        std::make_unique<sb::di::details::services::UniquePtrService<TestDependencyClass>>(std::move(service))};
    sb::di::OneOrList result{std::move(test1)};

    EXPECT_CALL(mock.getMock(), tryCreateInstances(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(std::make_optional(std::move(result))));

    const sb::di::details::factories::ServiceFactory<TestDependencyVecClass3> factory;

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::InPlaceService<TestDependencyVecClass3> *>(instance.get()));
}

TEST_F(ServiceFactoryTest, ShouldCreateDependencyVec4Service)
{
    ServiceProviderMock mock;
    auto service = std::make_unique<TestDependencyClass>();
    sb::di::ServiceInstance test1{
        std::make_unique<sb::di::details::services::UniquePtrService<TestDependencyClass>>(std::move(service))};

    sb::di::OneOrList result{std::move(test1)};

    EXPECT_CALL(mock.getMock(), tryCreateInstances(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(std::make_optional(std::move(result))));

    const sb::di::details::factories::ServiceFactory<TestDependencyVecClass4> factory;

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::InPlaceService<TestDependencyVecClass4> *>(instance.get()));
}
