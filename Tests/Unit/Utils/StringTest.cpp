#include <gtest/gtest.h>

#include "../../Helpers/Classes/Basic.hpp"
#include <SevenBit/DI/Details/Utils/String.hpp>
#include <SevenBit/DI/Exceptions.hpp>

class StringTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    StringTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~StringTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(StringTest, ShouldFail)
{
    EXPECT_THROW(sb::di::details::String::fmt("{", 8), std::runtime_error);
    EXPECT_THROW(sb::di::details::String::fmt("{}{", 8, 4), std::runtime_error);
}

TEST_F(StringTest, ShouldFormatInt)
{
    EXPECT_EQ(sb::di::details::String::fmt("", 8), "");
    EXPECT_EQ(sb::di::details::String::fmt("{}", 999), "999");
    EXPECT_EQ(sb::di::details::String::fmt("{+}", 999), "+999");
    EXPECT_EQ(sb::di::details::String::fmt("{}", -999), "-999");
    EXPECT_EQ(sb::di::details::String::fmt("{}", 0), "0");
    EXPECT_EQ(sb::di::details::String::fmt("{}{}", 8, 2), "82");
    EXPECT_EQ(sb::di::details::String::fmt("{04}{}", 8, 2), "00082");
    EXPECT_EQ(sb::di::details::String::fmt("{} n {}", 8, 2), "8 n 2");
    EXPECT_EQ(sb::di::details::String::fmt("alice", 8, 2), "alice");
    EXPECT_EQ(sb::di::details::String::fmt("alice {}", 8, 2), "alice 8");
    EXPECT_EQ(sb::di::details::String::fmt("{} alice '{}'", 8, 2), "8 alice '2'");
    EXPECT_EQ(sb::di::details::String::fmt("{} alice '{}' {} hop", 8, 2), "8 alice '2' {} hop");
}

TEST_F(StringTest, ShouldFormatLong)
{
    EXPECT_EQ(sb::di::details::String::fmt("", 8l), "");
    EXPECT_EQ(sb::di::details::String::fmt("{}", 999l), "999");
    EXPECT_EQ(sb::di::details::String::fmt("{+}", 999l), "+999");
    EXPECT_EQ(sb::di::details::String::fmt("{}", -999l), "-999");
    EXPECT_EQ(sb::di::details::String::fmt("{}", 0l), "0");
    EXPECT_EQ(sb::di::details::String::fmt("{}{}", 8l, 2l), "82");
    EXPECT_EQ(sb::di::details::String::fmt("{04}{}", 8l, 2l), "00082");
    EXPECT_EQ(sb::di::details::String::fmt("{} n {}", 8l, 2l), "8 n 2");
    EXPECT_EQ(sb::di::details::String::fmt("alice", 8l, 2l), "alice");
    EXPECT_EQ(sb::di::details::String::fmt("alice {}", 8l, 2l), "alice 8");
    EXPECT_EQ(sb::di::details::String::fmt("{} alice '{}'", 8l, 2l), "8 alice '2'");
    EXPECT_EQ(sb::di::details::String::fmt("{} alice '{}' {} hop", 8l, 2l), "8 alice '2' {} hop");
}

TEST_F(StringTest, ShouldFormatLongLong)
{
    EXPECT_EQ(sb::di::details::String::fmt("", 8ll), "");
    EXPECT_EQ(sb::di::details::String::fmt("{}", 999ll), "999");
    EXPECT_EQ(sb::di::details::String::fmt("{+}", 999ll), "+999");
    EXPECT_EQ(sb::di::details::String::fmt("{}", -999ll), "-999");
    EXPECT_EQ(sb::di::details::String::fmt("{}", 0ll), "0");
    EXPECT_EQ(sb::di::details::String::fmt("{}{}", 8ll, 2ll), "82");
    EXPECT_EQ(sb::di::details::String::fmt("{04}{}", 8ll, 2ll), "00082");
    EXPECT_EQ(sb::di::details::String::fmt("{} no {}", 8ll, 2ll), "8 no 2");
    EXPECT_EQ(sb::di::details::String::fmt("alice", 8ll, 2ll), "alice");
    EXPECT_EQ(sb::di::details::String::fmt("alice {}", 8ll, 2ll), "alice 8");
    EXPECT_EQ(sb::di::details::String::fmt("{} alice '{}'", 8ll, 2ll), "8 alice '2'");
    EXPECT_EQ(sb::di::details::String::fmt("{} alice '{}' {} hop", 8ll, 2ll), "8 alice '2' {} hop");
}

TEST_F(StringTest, ShouldFormatUnsigned)
{
    EXPECT_EQ(sb::di::details::String::fmt("", 8u), "");
    EXPECT_EQ(sb::di::details::String::fmt("{}", 999u), "999");
    EXPECT_EQ(sb::di::details::String::fmt("{}", 0u), "0");
    EXPECT_EQ(sb::di::details::String::fmt("{}{}", 8u, 2u), "82");
    EXPECT_EQ(sb::di::details::String::fmt("{04}{}", 8u, 2u), "00082");
    EXPECT_EQ(sb::di::details::String::fmt("{} n {}", 8u, 2u), "8 n 2");
    EXPECT_EQ(sb::di::details::String::fmt("alice", 8u, 2u), "alice");
    EXPECT_EQ(sb::di::details::String::fmt("alice {}", 8u, 2u), "alice 8");
    EXPECT_EQ(sb::di::details::String::fmt("{} alice '{}'", 8u, 2u), "8 alice '2'");
    EXPECT_EQ(sb::di::details::String::fmt("{} alice '{}' {} hop", 8u, 2u), "8 alice '2' {} hop");
}

TEST_F(StringTest, ShouldFormatUnsignedLong)
{
    EXPECT_EQ(sb::di::details::String::fmt("", 8ul), "");
    EXPECT_EQ(sb::di::details::String::fmt("{}", 999ul), "999");
    EXPECT_EQ(sb::di::details::String::fmt("{}", 0ul), "0");
    EXPECT_EQ(sb::di::details::String::fmt("{}{}", 8ul, 2ul), "82");
    EXPECT_EQ(sb::di::details::String::fmt("{04}{}", 8ul, 2ul), "00082");
    EXPECT_EQ(sb::di::details::String::fmt("{} n {}", 8ul, 2ul), "8 n 2");
    EXPECT_EQ(sb::di::details::String::fmt("alice", 8ul, 2ul), "alice");
    EXPECT_EQ(sb::di::details::String::fmt("alice {}", 8ul, 2ul), "alice 8");
    EXPECT_EQ(sb::di::details::String::fmt("{} alice '{}'", 8ul, 2ul), "8 alice '2'");
    EXPECT_EQ(sb::di::details::String::fmt("{} alice '{}' {} hop", 8ul, 2ul), "8 alice '2' {} hop");
}

TEST_F(StringTest, ShouldFormatUnsignedLongLong)
{
    EXPECT_EQ(sb::di::details::String::fmt("", 8ull), "");
    EXPECT_EQ(sb::di::details::String::fmt("{}", 999ull), "999");
    EXPECT_EQ(sb::di::details::String::fmt("{}", 0ull), "0");
    EXPECT_EQ(sb::di::details::String::fmt("{}{}", 8ull, 2ull), "82");
    EXPECT_EQ(sb::di::details::String::fmt("{04}{}", 8ull, 2ull), "00082");
    EXPECT_EQ(sb::di::details::String::fmt("{} n {}", 8ull, 2ull), "8 n 2");
    EXPECT_EQ(sb::di::details::String::fmt("alice", 8ull, 2ull), "alice");
    EXPECT_EQ(sb::di::details::String::fmt("alice {}", 8ull, 2ull), "alice 8");
    EXPECT_EQ(sb::di::details::String::fmt("{} alice '{}'", 8ull, 2ull), "8 alice '2'");
    EXPECT_EQ(sb::di::details::String::fmt("{} alice '{}' {} hop", 8ull, 2ull), "8 alice '2' {} hop");
}

TEST_F(StringTest, ShouldFormatDouble)
{
    EXPECT_EQ(sb::di::details::String::fmt("", 8.1), "");
    EXPECT_EQ(sb::di::details::String::fmt("{}", 999.9), "999.900000");
    EXPECT_EQ(sb::di::details::String::fmt("{+}", 999.1), "+999.100000");
    EXPECT_EQ(sb::di::details::String::fmt("{.2}", -999.8), "-999.80");
    EXPECT_EQ(sb::di::details::String::fmt("{}", 0.0), "0.000000");
    EXPECT_EQ(sb::di::details::String::fmt("{}{}", 8.2, 2.2), "8.2000002.200000");
    EXPECT_EQ(sb::di::details::String::fmt("{08.3}{}", 8.2, 2.2), "0008.2002.200000");
    EXPECT_EQ(sb::di::details::String::fmt("{} n {}", 8.2, 2.2), "8.200000 n 2.200000");
    EXPECT_EQ(sb::di::details::String::fmt("alice", 8.2, 2.2), "alice");
    EXPECT_EQ(sb::di::details::String::fmt("alice {}", 8.2, 2.2), "alice 8.200000");
    EXPECT_EQ(sb::di::details::String::fmt("{} alice '{}'", 8.2, 2.2), "8.200000 alice '2.200000'");
    EXPECT_EQ(sb::di::details::String::fmt("{} alice '{}' {} hop", 8.2, 2.2), "8.200000 alice '2.200000' {} hop");
}

TEST_F(StringTest, ShouldFormatFloat)
{
    EXPECT_EQ(sb::di::details::String::fmt("", 8.1f), "");
    EXPECT_EQ(sb::di::details::String::fmt("{.2}", 999.9f), "999.90");
    EXPECT_EQ(sb::di::details::String::fmt("{+.3}", 999.1f), "+999.100");
    EXPECT_EQ(sb::di::details::String::fmt("{.2}", -999.8f), "-999.80");
    EXPECT_EQ(sb::di::details::String::fmt("{}", 0.0f), "0.000000");
    EXPECT_EQ(sb::di::details::String::fmt("{}{}", 8.2f, 2.2f), "8.2000002.200000");
    EXPECT_EQ(sb::di::details::String::fmt("{08.3}{}", 8.2f, 2.2f), "0008.2002.200000");
    EXPECT_EQ(sb::di::details::String::fmt("{} n {}", 8.2f, 2.2f), "8.200000 n 2.200000");
    EXPECT_EQ(sb::di::details::String::fmt("alice", 8.2f, 2.2f), "alice");
    EXPECT_EQ(sb::di::details::String::fmt("alice {}", 8.2f, 2.2f), "alice 8.200000");
    EXPECT_EQ(sb::di::details::String::fmt("{} alice '{}'", 8.2f, 2.2f), "8.200000 alice '2.200000'");
    EXPECT_EQ(sb::di::details::String::fmt("{} alice '{}' {} hop", 8.2f, 2.2f), "8.200000 alice '2.200000' {} hop");
}

TEST_F(StringTest, ShouldFormatString)
{
    EXPECT_EQ(sb::di::details::String::fmt("", ""), "");
    EXPECT_EQ(sb::di::details::String::fmt("yes {-20}", std::string(101, '1')), ("yes " + std::string(101, '1')));
    EXPECT_EQ(sb::di::details::String::fmt("{}", "yes"), "yes");
    EXPECT_EQ(sb::di::details::String::fmt("{}{}", "yes", std::string_view{"no"}), "yesno");
    EXPECT_EQ(sb::di::details::String::fmt("alice", std::string("yes"), "no"), "alice");
    EXPECT_EQ(sb::di::details::String::fmt("alice {}", std::string("yes"), "no"), "alice yes");
    EXPECT_EQ(sb::di::details::String::fmt("{} alice '{}'", "yes", "no"), "yes alice 'no'");
    EXPECT_EQ(sb::di::details::String::fmt("{} alice '{}' {} hop", std::string("yes"), "no"), "yes alice 'no' {} hop");
}
