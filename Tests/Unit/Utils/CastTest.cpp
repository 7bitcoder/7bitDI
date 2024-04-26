#include <gtest/gtest.h>

#include "../../Helpers/Classes/MultiInherit.hpp"
#include <SevenBit/DI/Details/Services/InPlaceService.hpp>
#include <SevenBit/DI/Details/Utils/Cast.hpp>

class CastTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    CastTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~CastTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(CastTest, ShouldGetCastOffset)
{
    EXPECT_FALSE((sb::di::details::Cast::getCastOffset<int, int>()));
    EXPECT_FALSE((sb::di::details::Cast::getCastOffset<char, char>()));
    EXPECT_FALSE((sb::di::details::Cast::getCastOffset<double, double>()));
    EXPECT_FALSE((sb::di::details::Cast::getCastOffset<TestMultiInheritClass2, TestMultiInheritClass2>()));
    EXPECT_FALSE((sb::di::details::Cast::getCastOffset<TestMultiInheritClass1, TestMultiInheritClass2>()));
    EXPECT_FALSE((sb::di::details::Cast::getCastOffset<TestMultiInheritClass1, TestMultiInheritClass3>()));
    EXPECT_FALSE((sb::di::details::Cast::getCastOffset<TestMultiInheritClass2, TestMultiInheritClass3>()));
    EXPECT_FALSE((sb::di::details::Cast::getCastOffset<TestMultiInheritClass3, TestMultiInheritClass3>()));
    EXPECT_FALSE((sb::di::details::Cast::getCastOffset<TestMultiInheritClass3, TestMultiInheritClass3>()));
    EXPECT_FALSE((sb::di::details::Cast::getCastOffset<TestMultiInheritClass1, TestMultiInheritClass4>()));
    EXPECT_FALSE((sb::di::details::Cast::getCastOffset<TestMultiInheritClass2, TestMultiInheritClass4>()));
    EXPECT_FALSE((sb::di::details::Cast::getCastOffset<TestMultiInheritClass3, TestMultiInheritClass4>()));
    EXPECT_FALSE((sb::di::details::Cast::getCastOffset<TestMultiInheritClass3, TestMultiInheritClass4>()));
    EXPECT_FALSE((sb::di::details::Cast::getCastOffset<TestMultiInheritClass4, TestMultiInheritClass4>()));
    EXPECT_TRUE((sb::di::details::Cast::getCastOffset<TestMultiInherit2Class1, TestMultiInheritClass2>()));
    EXPECT_TRUE((sb::di::details::Cast::getCastOffset<TestMultiInherit2Class1, TestMultiInheritClass3>()));
    EXPECT_TRUE((sb::di::details::Cast::getCastOffset<TestMultiInherit3Class1, TestMultiInheritClass3>()));
    EXPECT_TRUE((sb::di::details::Cast::getCastOffset<TestMultiInherit2Class1, TestMultiInheritClass4>()));
    EXPECT_TRUE((sb::di::details::Cast::getCastOffset<TestMultiInherit3Class1, TestMultiInheritClass4>()));
    EXPECT_TRUE((sb::di::details::Cast::getCastOffset<TestMultiInherit4Class4, TestMultiInheritClass4>()));
}

TEST_F(CastTest, ShouldApplyCastOffset)
{
    EXPECT_EQ((sb::di::details::Cast::applyCastOffset(reinterpret_cast<void *>(1), 0)),
              reinterpret_cast<void *>(1));
    EXPECT_EQ((sb::di::details::Cast::applyCastOffset(reinterpret_cast<void *>(1), 1)),
              reinterpret_cast<void *>(2));
    EXPECT_EQ((sb::di::details::Cast::applyCastOffset(reinterpret_cast<void *>(22), 11)),
              reinterpret_cast<void *>(33));
    EXPECT_EQ((sb::di::details::Cast::applyCastOffset(reinterpret_cast<void *>(22), -11)),
              reinterpret_cast<void *>(11));
}
