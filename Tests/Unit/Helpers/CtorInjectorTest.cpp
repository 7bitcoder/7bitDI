#include <gtest/gtest.h>

#include "../../Helpers/Classes/Dependencies.hpp"
#include "../../Helpers/Mocks/ServiceProviderMock.hpp"
#include "SevenBit/DI/Details/Helpers/CtorInjector.hpp"
#include "SevenBit/DI/Details/Services/InPlaceService.hpp"

class CtorInjectorTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    CtorInjectorTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~CtorInjectorTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(CtorInjectorTest, ShouldInvokeFuncWithCtorParams)
{
    ServiceProviderMock mock;
    sb::di::ServiceInstance test1{std::make_unique<sb::di::details::InPlaceService<TestDependencyClass>>()};

    EXPECT_CALL(mock.getMock(), tryGetInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&test1));

    sb::di::details::CtorInjector<TestDependencyPtrClass1> invoker{mock};

    EXPECT_TRUE(invoker.makeUnique<TestDependencyPtrClass1>());
}
