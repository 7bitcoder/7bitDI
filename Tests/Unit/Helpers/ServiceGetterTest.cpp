#include <gtest/gtest.h>

#include "../../Helpers/Classes/Dependencies.hpp"
#include "../../Helpers/Mocks/ServiceProviderMock.hpp"
#include <SevenBit/DI/Details/Meta/ServiceGetter.hpp>
#include <SevenBit/DI/Details/Services/InPlaceService.hpp>
#include <SevenBit/DI/Details/Services/UniquePtrService.hpp>

class ServiceGetterTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceGetterTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceGetterTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ServiceGetterTest, ShouldGetDependencyInPlaceService)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::InPlaceService<TestDependencyClass>>()};

    EXPECT_CALL(mock.getMock(), createInstanceInPlace(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(std::move(test1)));

    auto act = [&] { TestDependencyClass instance = sb::di::details::ServiceGetter<TestDependencyClass>::get(mock); };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceGetterTest, ShouldGetDependencyPtr1Service)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::InPlaceService<TestDependencyClass>>()};

    EXPECT_CALL(mock.getMock(), tryGetInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&test1));

    auto act = [&] {
        TestDependencyClass *instance = sb::di::details::ServiceGetter<TestDependencyClass *>::get(mock);
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceGetterTest, ShouldGetDependencyPtr2Service)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::InPlaceService<TestDependencyClass>>()};

    EXPECT_CALL(mock.getMock(), tryGetInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&test1));

    auto act = [&] {
        const TestDependencyClass *instance = sb::di::details::ServiceGetter<const TestDependencyClass *>::get(mock);
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceGetterTest, ShouldGetDependencyRef1Service)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::InPlaceService<TestDependencyClass>>()};

    EXPECT_CALL(mock.getMock(), getInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::ReturnRef(test1));

    auto act = [&] {
        TestDependencyClass &instance = sb::di::details::ServiceGetter<TestDependencyClass &>::get(mock);
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceGetterTest, ShouldGetDependencyUniq1Service)
{
    ServiceProviderMock mock;
    auto service = std::make_unique<TestDependencyClass>();
    sb::di::ServiceInstance test1{
        std::make_unique<sb::di::details::UniquePtrService<TestDependencyClass>>(std::move(service))};

    EXPECT_CALL(mock.getMock(), createInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(std::move(test1)));

    auto act = [&] {
        std::unique_ptr<TestDependencyClass> instance =
            sb::di::details::ServiceGetter<std::unique_ptr<TestDependencyClass>>::get(mock);
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceGetterTest, ShouldGetDependencyVec1Service)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::InPlaceService<TestDependencyClass>>()};

    sb::di::OneOrList result{std::move(test1)};

    EXPECT_CALL(mock.getMock(), tryGetInstances(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&result));

    auto act = [&] {
        std::vector<TestDependencyClass *> instance =
            sb::di::details::ServiceGetter<std::vector<TestDependencyClass *>>::get(mock);
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceGetterTest, ShouldGetDependencyVec3Service)
{
    ServiceProviderMock mock;
    auto service = std::make_unique<TestDependencyClass>();
    sb::di::ServiceInstance test1{
        std::make_unique<sb::di::details::UniquePtrService<TestDependencyClass>>(std::move(service))};
    sb::di::OneOrList result{std::move(test1)};

    EXPECT_CALL(mock.getMock(), tryCreateInstances(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(std::move(result)));

    auto act = [&] {
        std::vector<std::unique_ptr<TestDependencyClass>> instance =
            sb::di::details::ServiceGetter<std::vector<std::unique_ptr<TestDependencyClass>>>::get(mock);
    };

    EXPECT_NO_THROW(act());
}
