#include <gtest/gtest.h>

#include "../../Helpers/Classes/Basic.hpp"
#include <SevenBit/DI/Details/Utils/Format.hpp>
#include <SevenBit/DI/Exceptions.hpp>

class FormatTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    FormatTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~FormatTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(FormatTest, ShouldFormatInt)
{
    EXPECT_EQ(sb::di::details::Format::fmt("", 8), "");
    EXPECT_EQ(sb::di::details::Format::fmt("{}", 8u), "8");
    EXPECT_EQ(sb::di::details::Format::fmt("{04}", 8u), "0008");
    EXPECT_EQ(sb::di::details::Format::fmt("{}{}", 8, 2l), "82");
    EXPECT_EQ(sb::di::details::Format::fmt("{04}{}", 8, 2l), "00082");
    EXPECT_EQ(sb::di::details::Format::fmt("alice", 8, 2), "alice");
    EXPECT_EQ(sb::di::details::Format::fmt("alice {}", 8ll, 2), "alice 8");
    EXPECT_EQ(sb::di::details::Format::fmt("{} alice '{}'", 8, 2ull), "8 alice '2'");
    EXPECT_EQ(sb::di::details::Format::fmt("{} alice '{}' {} hop", 8ul, 2), "8 alice '2' {} hop");
}

TEST_F(FormatTest, ShouldFormatFloat)
{
    EXPECT_EQ(sb::di::details::Format::fmt("", 8.f), "");
    EXPECT_EQ(sb::di::details::Format::fmt("{}", 8.f), "8.000000");
    EXPECT_EQ(sb::di::details::Format::fmt("{}{}", 8.2, 2.2f), "8.2000002.200000");
    EXPECT_EQ(sb::di::details::Format::fmt("alice", 8.4, 2), "alice");
    EXPECT_EQ(sb::di::details::Format::fmt("alice {}", 8.0, 2.5f), "alice 8.000000");
    EXPECT_EQ(sb::di::details::Format::fmt("{} alice '{}'", 8.4, 2.1), "8.400000 alice '2.100000'");
    EXPECT_EQ(sb::di::details::Format::fmt("{} alice '{}' {} hop", 8.2, 2.6), "8.200000 alice '2.600000' {} hop");
}

TEST_F(FormatTest, ShouldFormatString)
{
    EXPECT_EQ(sb::di::details::Format::fmt("", ""), "");
    EXPECT_EQ(sb::di::details::Format::fmt("{}", "yes"), "yes");
    EXPECT_EQ(sb::di::details::Format::fmt("{}{}", "yes", std::string_view{"no"}), "yesno");
    EXPECT_EQ(sb::di::details::Format::fmt("alice", std::string("yes"), "no"), "alice");
    EXPECT_EQ(sb::di::details::Format::fmt("alice {}", std::string("yes"), "no"), "alice yes");
    EXPECT_EQ(sb::di::details::Format::fmt("{} alice '{}'", "yes", "no"), "yes alice 'no'");
    EXPECT_EQ(sb::di::details::Format::fmt("{} alice '{}' {} hop", std::string("yes"), "no"), "yes alice 'no' {} hop");
}
