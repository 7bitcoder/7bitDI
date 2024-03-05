#include <gtest/gtest.h>

#include "SevenBit/DI/Details/Utils/CheckBase.hpp"

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
    EXPECT_FALSE(sb::di::details::CheckBase::notNull<int>(nullptr));
    EXPECT_FALSE(sb::di::details::CheckBase::notNull(std::unique_ptr<int>()));
    EXPECT_FALSE(sb::di::details::CheckBase::notNull(std::shared_ptr<int>()));

    int intTest = 123;
    EXPECT_TRUE(sb::di::details::CheckBase::notNull(&intTest));
    EXPECT_TRUE(sb::di::details::CheckBase::notNull(std::make_unique<int>(intTest)));
    EXPECT_TRUE(sb::di::details::CheckBase::notNull(std::make_shared<int>(intTest)));
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
    EXPECT_FALSE(sb::di::details::CheckBase::enumValidity(static_cast<TestEnum>(123)));
    EXPECT_FALSE(sb::di::details::CheckBase::enumValidity(static_cast<TestEnum>(-123)));
    EXPECT_FALSE(sb::di::details::CheckBase::enumValidity(static_cast<TestEnum>(-1)));
    EXPECT_FALSE(sb::di::details::CheckBase::enumValidity(TestEnum::Count));
    EXPECT_TRUE(sb::di::details::CheckBase::enumValidity(TestEnum::A));
    EXPECT_TRUE(sb::di::details::CheckBase::enumValidity(TestEnum::B));
    EXPECT_TRUE(sb::di::details::CheckBase::enumValidity(TestEnum::C));
}
