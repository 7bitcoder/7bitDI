#include <gtest/gtest.h>

#include "../../Helpers/Classes/Complex.hpp"
#include <SevenBit/DI/Details/Helpers/FunctorInjector.hpp>

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

struct LotOfRefs
{
    int &a1;
    int &a2;
    int &a3;
    int &a4;
    int &a5;
    int &a6;
    int &a7;
    int &a8;
    int &a9;
    int &a10;
    int &a11;
};

struct Ambigious
{
    Ambigious(int a, int b) {}
    Ambigious(int a, int b, int d) {}
    Ambigious(int a, int b, int d, int e) {}
};

TEST_F(CtorParamsNumberTest, ShouldGetProperCtorParamsNumber)
{
    //    EXPECT_EQ(sb::di::details::CtorParamsNumber<TestComplexClass1>::value, 0);
    //    EXPECT_EQ(sb::di::details::CtorParamsNumber<TestComplexClass2>::value, 1);
    //    EXPECT_EQ(sb::di::details::CtorParamsNumber<TestComplexClass3>::value, 2);
    //    EXPECT_EQ(sb::di::details::CtorParamsNumber<TestComplexClass4>::value, 3);
    //    EXPECT_EQ(sb::di::details::CtorParamsNumber<TestComplexClass5>::value, 3);
    //    EXPECT_EQ(sb::di::details::CtorParamsNumber<LotOfParams>::value, 13);
    //    EXPECT_EQ(sb::di::details::CtorParamsNumber<LotOfRefs>::value, 11);
    //    EXPECT_EQ(sb::di::details::CtorParamsNumber<Ambigious>::value, 2);
}
