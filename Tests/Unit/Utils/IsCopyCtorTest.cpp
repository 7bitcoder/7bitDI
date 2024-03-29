#include <gtest/gtest.h>

#include "../../Helpers/Classes/Complex.hpp"
#include "SevenBit/DI/Details/Utils/IsCopyCtor.hpp"

class IsCopyCtorTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    IsCopyCtorTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~IsCopyCtorTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(IsCopyCtorTest, ShouldCheckForCopyCtor)
{
    EXPECT_TRUE((sb::di::details::IsCopyCtorV<TestComplexClass1, TestComplexClass1>));
    EXPECT_TRUE((sb::di::details::IsCopyCtorV<TestComplexClass1, const TestComplexClass1>));
    EXPECT_FALSE((sb::di::details::IsCopyCtorV<TestComplexClass2, TestComplexClass1>));
    EXPECT_FALSE((sb::di::details::IsCopyCtorV<TestComplexClass2, const TestComplexClass1>));
    EXPECT_FALSE((sb::di::details::IsCopyCtorV<TestComplexClass2, TestComplexClass3>));
}
