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
    EXPECT_FALSE((sb::di::details::Cast::getOffset<int, int>()));
    EXPECT_FALSE((sb::di::details::Cast::getOffset<char, char>()));
    EXPECT_FALSE((sb::di::details::Cast::getOffset<double, double>()));
    EXPECT_FALSE((sb::di::details::Cast::getOffset<TestMultiInheritClass2, TestMultiInheritClass2>()));
    EXPECT_FALSE((sb::di::details::Cast::getOffset<TestMultiInheritClass1, TestMultiInheritClass2>()));
    EXPECT_FALSE((sb::di::details::Cast::getOffset<TestMultiInheritClass1, TestMultiInheritClass3>()));
    EXPECT_FALSE((sb::di::details::Cast::getOffset<TestMultiInheritClass2, TestMultiInheritClass3>()));
    EXPECT_FALSE((sb::di::details::Cast::getOffset<TestMultiInheritClass3, TestMultiInheritClass3>()));
    EXPECT_FALSE((sb::di::details::Cast::getOffset<TestMultiInheritClass3, TestMultiInheritClass3>()));
    EXPECT_FALSE((sb::di::details::Cast::getOffset<TestMultiInheritClass1, TestMultiInheritClass4>()));
    EXPECT_FALSE((sb::di::details::Cast::getOffset<TestMultiInheritClass2, TestMultiInheritClass4>()));
    EXPECT_FALSE((sb::di::details::Cast::getOffset<TestMultiInheritClass3, TestMultiInheritClass4>()));
    EXPECT_FALSE((sb::di::details::Cast::getOffset<TestMultiInheritClass3, TestMultiInheritClass4>()));
    EXPECT_FALSE((sb::di::details::Cast::getOffset<TestMultiInheritClass4, TestMultiInheritClass4>()));
    EXPECT_TRUE((sb::di::details::Cast::getOffset<TestMultiInherit2Class1, TestMultiInheritClass2>()));
    EXPECT_TRUE((sb::di::details::Cast::getOffset<TestMultiInherit2Class1, TestMultiInheritClass3>()));
    EXPECT_TRUE((sb::di::details::Cast::getOffset<TestMultiInherit3Class1, TestMultiInheritClass3>()));
    EXPECT_TRUE((sb::di::details::Cast::getOffset<TestMultiInherit2Class1, TestMultiInheritClass4>()));
    EXPECT_TRUE((sb::di::details::Cast::getOffset<TestMultiInherit3Class1, TestMultiInheritClass4>()));
    EXPECT_TRUE((sb::di::details::Cast::getOffset<TestMultiInherit4Class4, TestMultiInheritClass4>()));
}

TEST_F(CastTest, ShouldApplyCastOffset)
{
    EXPECT_EQ((sb::di::details::Cast::applyOffset(reinterpret_cast<void *>(1), 0)), reinterpret_cast<void *>(1));
    EXPECT_EQ((sb::di::details::Cast::applyOffset(reinterpret_cast<void *>(1), 1)), reinterpret_cast<void *>(2));
    EXPECT_EQ((sb::di::details::Cast::applyOffset(reinterpret_cast<void *>(22), 11)), reinterpret_cast<void *>(33));
    EXPECT_EQ((sb::di::details::Cast::applyOffset(reinterpret_cast<void *>(22), -11)), reinterpret_cast<void *>(11));
}
