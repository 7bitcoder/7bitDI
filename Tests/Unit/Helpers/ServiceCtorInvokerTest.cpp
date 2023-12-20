#include <gtest/gtest.h>

#include "../../Helpers/Classes/Dependencies.hpp"
#include "../../Helpers/Mocks/ServiceProviderMock.hpp"
#include "SevenBit/DI/Details/Factories/ServiceFactory.hpp"
#include "SevenBit/DI/Details/Helpers/ServiceCtorInvoker.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

class ServiceCtorInvokerTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceCtorInvokerTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceCtorInvokerTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ServiceCtorInvokerTest, ShouldInvokeFuncWithCtorParams)
{
    ServiceProviderMock mock;
    sb::di::details::services::InPlaceService<TestDependencyClass> test1;

    EXPECT_CALL(mock.getMock(), tryGetInstance(sb::di::TypeId{typeid(TestDependencyClass)}))
        .WillOnce(testing::Return(&test1));

    sb::di::details::helpers::ServiceCtorInvoker<TestDependencyPtrClass1> invoker{mock};

    auto func = [&](TestDependencyClass *test1) {
        EXPECT_TRUE(test1);
        return 1;
    };
    EXPECT_EQ(invoker.invokeWithCtorParams(func), 1);
}