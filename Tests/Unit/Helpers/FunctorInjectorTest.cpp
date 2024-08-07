#include <gtest/gtest.h>

#include "../../Helpers/Classes/Dependencies.hpp"
#include "../../Helpers/Mocks/ServiceProviderMock.hpp"
#include <SevenBit/DI/Details/Helpers/FunctorInjector.hpp>
#include <SevenBit/DI/Details/Services/InPlaceService.hpp>

class FunctorInjectorTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    FunctorInjectorTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~FunctorInjectorTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(FunctorInjectorTest, ShouldInvokeFuncFactory)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::InPlaceService<TestDependencyClass>>()};

    EXPECT_CALL(mock.getMock(), tryGetInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&test1));

    auto func = [&](TestDependencyClass *service) {
        EXPECT_TRUE(service);
        return 1;
    };

    sb::di::details::FunctorInjector<decltype(func)> invoker{func, mock};

    EXPECT_EQ(invoker(), 1);
}
