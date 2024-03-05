#include <gtest/gtest.h>

#include "SevenBit/DI/Details/Utils/Check.hpp"

class CheckBaseTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    CheckBaseTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~CheckBaseTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(CheckBaseTest, ShouldCheckNotNull)
{
    EXPECT_FALSE(sb::di::details::Check::notNull<int>(nullptr));
    EXPECT_FALSE(sb::di::details::Check::notNull(std::unique_ptr<int>()));
    EXPECT_FALSE(sb::di::details::Check::notNull(std::shared_ptr<int>()));

    int intTest = 123;
    EXPECT_TRUE(sb::di::details::Check::notNull(&intTest));
    EXPECT_TRUE(sb::di::details::Check::notNull(std::make_unique<int>(intTest)));
    EXPECT_TRUE(sb::di::details::Check::notNull(std::make_shared<int>(intTest)));
}

TEST_F(CheckBaseTest, ShouldCheckEnum)
{
    enum TestEnum
    {
        A,
        B,
        C,
        Count,
    };
    EXPECT_FALSE(sb::di::details::Check::enumValidity(static_cast<TestEnum>(123)));
    EXPECT_FALSE(sb::di::details::Check::enumValidity(static_cast<TestEnum>(-123)));
    EXPECT_FALSE(sb::di::details::Check::enumValidity(static_cast<TestEnum>(-1)));
    EXPECT_FALSE(sb::di::details::Check::enumValidity(TestEnum::Count));
    EXPECT_TRUE(sb::di::details::Check::enumValidity(TestEnum::A));
    EXPECT_TRUE(sb::di::details::Check::enumValidity(TestEnum::B));
    EXPECT_TRUE(sb::di::details::Check::enumValidity(TestEnum::C));
}
