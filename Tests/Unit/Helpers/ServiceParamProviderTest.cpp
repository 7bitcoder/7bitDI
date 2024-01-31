#include <gtest/gtest.h>

#include "../../Helpers/Classes/Dependencies.hpp"
#include "../../Helpers/Mocks/ServiceProviderMock.hpp"
#include "SevenBit/DI/Details/Helpers/ServiceParamProvider.hpp"
#include "SevenBit/DI/Details/Services/ExternalService.hpp"
#include "SevenBit/DI/Details/Services/InPlaceService.hpp"
#include "SevenBit/DI/Details/Services/UniquePtrService.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

class ServiceParamProviderTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceParamProviderTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceParamProviderTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ServiceParamProviderTest, ShouldGetDependencyInPlaceService)
{
    ServiceProviderMock mock;
    auto test1 = std::make_unique<sb::di::details::services::InPlaceService<TestDependencyClass>>();

    EXPECT_CALL(mock.getMock(), createInstanceInPlace(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(std::move(test1)));

    auto act = [&] {
        TestDependencyClass instance =
            sb::di::details::helpers::ServiceParamProvider<TestDependencyClass>::getParam(mock);
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceParamProviderTest, ShouldGetDependencyPtr1Service)
{
    ServiceProviderMock mock;
    sb::di::details::services::InPlaceService<TestDependencyClass> test1;

    EXPECT_CALL(mock.getMock(), tryGetInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&test1));

    auto act = [&] {
        TestDependencyClass *instance =
            sb::di::details::helpers::ServiceParamProvider<TestDependencyClass *>::getParam(mock);
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceParamProviderTest, ShouldGetDependencyPtr2Service)
{
    ServiceProviderMock mock;
    sb::di::details::services::InPlaceService<TestDependencyClass> test1;

    EXPECT_CALL(mock.getMock(), tryGetInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&test1));

    auto act = [&] {
        const TestDependencyClass *instance =
            sb::di::details::helpers::ServiceParamProvider<const TestDependencyClass *>::getParam(mock);
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceParamProviderTest, ShouldGetDependencyPtr3Service)
{
    ServiceProviderMock mock;
    sb::di::details::services::InPlaceService<TestDependencyClass> test1;

    EXPECT_CALL(mock.getMock(), tryGetInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&test1));

    auto act = [&] {
        const TestDependencyClass *const instance =
            sb::di::details::helpers::ServiceParamProvider<const TestDependencyClass *const>::getParam(mock);
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceParamProviderTest, ShouldGetDependencyPtr4Service)
{
    ServiceProviderMock mock;
    sb::di::details::services::InPlaceService<TestDependencyClass> test1;

    EXPECT_CALL(mock.getMock(), tryGetInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&test1));

    auto act = [&] {
        TestDependencyClass *const instance =
            sb::di::details::helpers::ServiceParamProvider<TestDependencyClass *const>::getParam(mock);
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceParamProviderTest, ShouldGetDependencyRef1Service)
{
    ServiceProviderMock mock;
    sb::di::details::services::InPlaceService<TestDependencyClass> test1;

    EXPECT_CALL(mock.getMock(), getInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::ReturnRef(test1));

    auto act = [&] {
        TestDependencyClass &instance =
            sb::di::details::helpers::ServiceParamProvider<TestDependencyClass &>::getParam(mock);
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceParamProviderTest, ShouldGetDependencyRef2Service)
{
    ServiceProviderMock mock;
    sb::di::details::services::InPlaceService<TestDependencyClass> test1;

    EXPECT_CALL(mock.getMock(), getInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::ReturnRef(test1));

    auto act = [&] {
        const TestDependencyClass &instance =
            sb::di::details::helpers::ServiceParamProvider<const TestDependencyClass &>::getParam(mock);
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceParamProviderTest, ShouldGetDependencyUniq1Service)
{
    ServiceProviderMock mock;
    auto service = std::make_unique<TestDependencyClass>();
    auto test1 = std::make_unique<sb::di::details::services::UniquePtrService<TestDependencyClass>>(std::move(service));

    EXPECT_CALL(mock.getMock(), createInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(std::move(test1)));

    auto act = [&] {
        std::unique_ptr<TestDependencyClass> instance =
            sb::di::details::helpers::ServiceParamProvider<std::unique_ptr<TestDependencyClass>>::getParam(mock);
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceParamProviderTest, ShouldGetDependencyUniq2Service)
{
    ServiceProviderMock mock;
    auto service = std::make_unique<TestDependencyClass>();
    auto test1 = std::make_unique<sb::di::details::services::UniquePtrService<TestDependencyClass>>(std::move(service));

    EXPECT_CALL(mock.getMock(), createInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(std::move(test1)));

    auto act = [&] {
        const std::unique_ptr<TestDependencyClass> instance =
            sb::di::details::helpers::ServiceParamProvider<const std::unique_ptr<TestDependencyClass>>::getParam(mock);
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceParamProviderTest, ShouldGetDependencyVec1Service)
{
    ServiceProviderMock mock;
    auto test1 = std::make_unique<sb::di::details::services::InPlaceService<TestDependencyClass>>();
    sb::di::OneOrList<sb::di::ServiceInstance::Ptr> result{std::move(test1)};

    EXPECT_CALL(mock.getMock(), tryGetInstances(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&result));

    auto act = [&] {
        std::vector<TestDependencyClass *> instance =
            sb::di::details::helpers::ServiceParamProvider<std::vector<TestDependencyClass *>>::getParam(mock);
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceParamProviderTest, ShouldGetDependencyVec2Service)
{
    ServiceProviderMock mock;
    auto test1 = std::make_unique<sb::di::details::services::InPlaceService<TestDependencyClass>>();
    sb::di::OneOrList<sb::di::ServiceInstance::Ptr> result{std::move(test1)};

    EXPECT_CALL(mock.getMock(), tryGetInstances(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&result));

    auto act = [&] {
        const std::vector<TestDependencyClass *> instance =
            sb::di::details::helpers::ServiceParamProvider<const std::vector<TestDependencyClass *>>::getParam(mock);
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceParamProviderTest, ShouldGetDependencyVec3Service)
{
    ServiceProviderMock mock;
    auto service = std::make_unique<TestDependencyClass>();
    auto test1 = std::make_unique<sb::di::details::services::UniquePtrService<TestDependencyClass>>(std::move(service));
    sb::di::OneOrList<sb::di::ServiceInstance::Ptr> result{std::move(test1)};

    EXPECT_CALL(mock.getMock(), tryCreateInstances(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(std::make_optional(std::move(result))));

    auto act = [&] {
        std::vector<std::unique_ptr<TestDependencyClass>> instance =
            sb::di::details::helpers::ServiceParamProvider<std::vector<std::unique_ptr<TestDependencyClass>>>::getParam(
                mock);
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceParamProviderTest, ShouldGetDependencyVec4Service)
{
    ServiceProviderMock mock;
    auto service = std::make_unique<TestDependencyClass>();
    auto test1 = std::make_unique<sb::di::details::services::UniquePtrService<TestDependencyClass>>(std::move(service));
    sb::di::OneOrList<sb::di::ServiceInstance::Ptr> result{std::move(test1)};

    EXPECT_CALL(mock.getMock(), tryCreateInstances(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(std::make_optional(std::move(result))));

    auto act = [&] {
        const std::vector<std::unique_ptr<TestDependencyClass>> instance =
            sb::di::details::helpers::ServiceParamProvider<
                const std::vector<std::unique_ptr<TestDependencyClass>>>::getParam(mock);
    };

    EXPECT_NO_THROW(act());
}
