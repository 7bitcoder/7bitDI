#include <gtest/gtest.h>

#include "SevenBit/DI/Details/Utils/Check.hpp"

#include "../TestHelpers/Classes/Complex.hpp"
#include "SevenBit/DI/Details/Utils/CtorParamsNumber.hpp"

class CtorParamsNumberTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    CtorParamsNumberTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~CtorParamsNumberTest() override = default;

    static void TearDownTestSuite() {}
};

struct LotOfParams
{
    LotOfParams(int a, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11,
                int a12)
    {
    }
};

struct Ambigious
{
    Ambigious(int a, int b) {}
    Ambigious(int a, int b, int d) {}
    Ambigious(int a, int b, int d, int e) {}
};
TEST_F(CtorParamsNumberTest, ShouldGetProperCtorParamsNumber)
{
    EXPECT_EQ(sb::di::details::utils::ctorParamsNumber2<TestComplexClass1>(), 0);
    EXPECT_EQ(sb::di::details::utils::ctorParamsNumber2<TestComplexClass2>(), 1);
    EXPECT_EQ(sb::di::details::utils::ctorParamsNumber2<TestComplexClass3>(), 2);
    EXPECT_EQ(sb::di::details::utils::ctorParamsNumber2<TestComplexClass4>(), 3);
    EXPECT_EQ(sb::di::details::utils::ctorParamsNumber2<TestComplexClass5>(), 3);
    EXPECT_EQ(sb::di::details::utils::ctorParamsNumber2<LotOfParams>(), 13);
    EXPECT_EQ(sb::di::details::utils::ctorParamsNumber2<Ambigious>(), 2);
}
