#include <gtest/gtest.h>
#include <memory>

#include "../../Helpers/Classes/Basic.hpp"
#include "../../Helpers/Classes/Dependencies.hpp"
#include "../../Helpers/Mocks/ServiceProviderMock.hpp"
#include "SevenBit/DI/Details/Factories/ServiceFcnFactory.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

class ServiceFcnFactoryTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceFcnFactoryTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceFcnFactoryTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ServiceFcnFactoryTest, ShouldCreateService)
{
    ServiceProviderMock mock;
    const sb::di::details::factories::ServiceFcnFactory factory{[&] { return std::make_unique<TestClass1>(); }};

    const auto instance = factory.createInstance(mock, false);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::UniquePtrService<TestClass1> *>(instance.get()));
}

TEST_F(ServiceFcnFactoryTest, ShouldCreateWithInPlaceService)
{
    ServiceProviderMock mock;
    const sb::di::details::factories::ServiceFcnFactory factory{[&] { return TestClass1(); }};

    const auto instance = factory.createInstance(mock, false);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::UniquePtrService<TestClass1> *>(instance.get()));
}

TEST_F(ServiceFcnFactoryTest, ShouldCreateInPlaceService)
{
    ServiceProviderMock mock;
    const sb::di::details::factories::ServiceFcnFactory factory{[&] { return TestClass1(); }};

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::InPlaceService<TestClass1> *>(instance.get()));
}

TEST_F(ServiceFcnFactoryTest, ShouldCreateDependencyInPlaceService)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::services::InPlaceService<TestDependencyClass>>()};

    EXPECT_CALL(mock.getMock(), createInstanceInPlace(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(std::move(test1)));

    const sb::di::details::factories::ServiceFcnFactory factory{
        [&](const TestDependencyClass ob) { return TestDependencyInPlaceClass(ob); }};

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::InPlaceService<TestDependencyInPlaceClass> *>(instance.get()));
}

TEST_F(ServiceFcnFactoryTest, ShouldCreateDependencyPtr1Service)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::services::InPlaceService<TestDependencyClass>>()};

    EXPECT_CALL(mock.getMock(), tryGetInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&test1));

    const sb::di::details::factories::ServiceFcnFactory factory{
        [&](TestDependencyClass *ob) { return TestDependencyPtrClass1(ob); }};

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::InPlaceService<TestDependencyPtrClass1> *>(instance.get()));
}

TEST_F(ServiceFcnFactoryTest, ShouldCreateDependencyPtr2Service)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::services::InPlaceService<TestDependencyClass>>()};

    EXPECT_CALL(mock.getMock(), tryGetInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&test1));

    const sb::di::details::factories::ServiceFcnFactory factory{
        [&](const TestDependencyClass *ob) { return TestDependencyPtrClass2(ob); }};

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::InPlaceService<TestDependencyPtrClass2> *>(instance.get()));
}

TEST_F(ServiceFcnFactoryTest, ShouldCreateDependencyPtr3Service)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::services::InPlaceService<TestDependencyClass>>()};

    EXPECT_CALL(mock.getMock(), tryGetInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&test1));

    const sb::di::details::factories::ServiceFcnFactory factory{
        [&](const TestDependencyClass *const ob) { return TestDependencyPtrClass3(ob); }};

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::InPlaceService<TestDependencyPtrClass3> *>(instance.get()));
}

TEST_F(ServiceFcnFactoryTest, ShouldCreateDependencyPtr4Service)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::services::InPlaceService<TestDependencyClass>>()};

    EXPECT_CALL(mock.getMock(), tryGetInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&test1));

    const sb::di::details::factories::ServiceFcnFactory factory{
        [&](TestDependencyClass *const ob) { return TestDependencyPtrClass4(ob); }};

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::InPlaceService<TestDependencyPtrClass4> *>(instance.get()));
}

TEST_F(ServiceFcnFactoryTest, ShouldCreateDependencyRef1Service)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::services::InPlaceService<TestDependencyClass>>()};

    EXPECT_CALL(mock.getMock(), getInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::ReturnRef(test1));

    const sb::di::details::factories::ServiceFcnFactory factory{
        [&](TestDependencyClass &ob) { return TestDependencyRefClass1(ob); }};

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::InPlaceService<TestDependencyRefClass1> *>(instance.get()));
}

TEST_F(ServiceFcnFactoryTest, ShouldCreateDependencyRef2Service)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::services::InPlaceService<TestDependencyClass>>()};

    EXPECT_CALL(mock.getMock(), getInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::ReturnRef(test1));

    const sb::di::details::factories::ServiceFcnFactory factory{
        [&](const TestDependencyClass &ob) { return TestDependencyRefClass2(ob); }};

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::InPlaceService<TestDependencyRefClass2> *>(instance.get()));
}

TEST_F(ServiceFcnFactoryTest, ShouldCreateDependencyUniq1Service)
{
    ServiceProviderMock mock;
    auto service = std::make_unique<TestDependencyClass>();
    sb::di::ServiceInstance test1{
        std::make_unique<sb::di::details::services::UniquePtrService<TestDependencyClass>>(std::move(service))};

    EXPECT_CALL(mock.getMock(), createInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(std::move(test1)));

    const sb::di::details::factories::ServiceFcnFactory factory{
        [&](std::unique_ptr<TestDependencyClass> ob) { return TestDependencyUniquePtrClass1(std::move(ob)); }};

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(
        dynamic_cast<sb::di::details::services::InPlaceService<TestDependencyUniquePtrClass1> *>(instance.get()));
}

TEST_F(ServiceFcnFactoryTest, ShouldCreateDependencyUniq2Service)
{
    ServiceProviderMock mock;
    auto service = std::make_unique<TestDependencyClass>();
    sb::di::ServiceInstance test1{
        std::make_unique<sb::di::details::services::UniquePtrService<TestDependencyClass>>(std::move(service))};

    EXPECT_CALL(mock.getMock(), createInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(std::move(test1)));

    const sb::di::details::factories::ServiceFcnFactory factory{[&](const std::unique_ptr<TestDependencyClass> ob) {
        return std::make_unique<TestDependencyUniquePtrClass2>(nullptr);
    }};

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(
        dynamic_cast<sb::di::details::services::UniquePtrService<TestDependencyUniquePtrClass2> *>(instance.get()));
}

TEST_F(ServiceFcnFactoryTest, ShouldCreateDependencyVec1Service)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::services::InPlaceService<TestDependencyClass>>()};

    sb::di::OneOrList result{std::move(test1)};

    EXPECT_CALL(mock.getMock(), tryGetInstances(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&result));

    const sb::di::details::factories::ServiceFcnFactory factory{
        [&](std::vector<TestDependencyClass *> ob) { return TestDependencyVecClass1(std::move(ob)); }};

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::InPlaceService<TestDependencyVecClass1> *>(instance.get()));
}

TEST_F(ServiceFcnFactoryTest, ShouldCreateDependencyVec2Service)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::services::InPlaceService<TestDependencyClass>>()};
    sb::di::OneOrList result{std::move(test1)};

    EXPECT_CALL(mock.getMock(), tryGetInstances(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&result));

    const sb::di::details::factories::ServiceFcnFactory factory{[&](const std::vector<TestDependencyClass *> ob) {
        return std::make_unique<TestDependencyVecClass2>(std::vector<TestDependencyClass *>{});
    }};

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::UniquePtrService<TestDependencyVecClass2> *>(instance.get()));
}

TEST_F(ServiceFcnFactoryTest, ShouldCreateDependencyVec3Service)
{
    ServiceProviderMock mock;
    auto service = std::make_unique<TestDependencyClass>();
    sb::di::ServiceInstance test1{
        std::make_unique<sb::di::details::services::UniquePtrService<TestDependencyClass>>(std::move(service))};
    sb::di::OneOrList result{std::move(test1)};

    EXPECT_CALL(mock.getMock(), tryCreateInstances(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(std::make_optional(std::move(result))));

    const sb::di::details::factories::ServiceFcnFactory factory{
        [&](std::vector<std::unique_ptr<TestDependencyClass>> ob) { return TestDependencyVecClass3(std::move(ob)); }};

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::InPlaceService<TestDependencyVecClass3> *>(instance.get()));
}

TEST_F(ServiceFcnFactoryTest, ShouldCreateDependencyVec4Service)
{
    ServiceProviderMock mock;
    auto service = std::make_unique<TestDependencyClass>();
    sb::di::ServiceInstance test1{
        std::make_unique<sb::di::details::services::UniquePtrService<TestDependencyClass>>(std::move(service))};

    sb::di::OneOrList result{std::move(test1)};

    EXPECT_CALL(mock.getMock(), tryCreateInstances(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(std::make_optional(std::move(result))));

    const sb::di::details::factories::ServiceFcnFactory factory{
        [&](const std::vector<std::unique_ptr<TestDependencyClass>> ob) {
            return std::make_unique<TestDependencyVecClass4>(std::vector<std::unique_ptr<TestDependencyClass>>{});
        }};

    const auto instance = factory.createInstance(mock, true);

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance->isValid());
    EXPECT_TRUE(instance->get());
    EXPECT_TRUE(dynamic_cast<sb::di::details::services::UniquePtrService<TestDependencyVecClass4> *>(instance.get()));
}
