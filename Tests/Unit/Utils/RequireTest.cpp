#include <gtest/gtest.h>

#include "../../Helpers/Classes/Basic.hpp"
#include "SevenBit/DI/Details/Utils/Require.hpp"
#include "SevenBit/DI/Exceptions.hpp"

class RequireTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    RequireTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~RequireTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(RequireTest, ShouldRequireNotNull)
{
    TestClass1 test1;
    std::unique_ptr<TestClass1> testPtr{new TestClass1};
    std::shared_ptr<TestClass1> testSPtr{new TestClass1};
    EXPECT_NO_THROW(sb::di::details::Require::notNull(&test1));
    EXPECT_NO_THROW(sb::di::details::Require::notNullAndGet(&test1));
    EXPECT_NO_THROW(sb::di::details::Require::notNull(testPtr));
    EXPECT_NO_THROW(sb::di::details::Require::notNullAndGet(std::move(testPtr)));
    EXPECT_NO_THROW(sb::di::details::Require::notNull(testSPtr));
    EXPECT_NO_THROW(sb::di::details::Require::notNullAndGet(std::move(testSPtr)));
}

TEST_F(RequireTest, ShouldFailRequireNotNull)
{
    std::unique_ptr<TestClass1> testPtr;
    std::unique_ptr<TestClass1> testSPtr;
    EXPECT_THROW(sb::di::details::Require::notNull<TestClass1>(nullptr), sb::di::NullPointerException);
    EXPECT_THROW(sb::di::details::Require::notNullAndGet<TestClass1>(nullptr), sb::di::NullPointerException);
    EXPECT_THROW(sb::di::details::Require::notNull(testPtr), sb::di::NullPointerException);
    EXPECT_THROW(sb::di::details::Require::notNullAndGet(std::move(testPtr)), sb::di::NullPointerException);
    EXPECT_THROW(sb::di::details::Require::notNull(testSPtr), sb::di::NullPointerException);
    EXPECT_THROW(sb::di::details::Require::notNullAndGet(std::move(testSPtr)), sb::di::NullPointerException);
}

TEST_F(RequireTest, ShouldRequireValidEnum)
{
    enum TestEnum
    {
        A,
        B,
        C,
        Count,
    };
    EXPECT_THROW(sb::di::details::Require::validEnum(static_cast<TestEnum>(123)), sb::di::InjectorException);
    EXPECT_THROW(sb::di::details::Require::validEnum(static_cast<TestEnum>(-123)), sb::di::InjectorException);
    EXPECT_THROW(sb::di::details::Require::validEnum(static_cast<TestEnum>(-1)), sb::di::InjectorException);
    EXPECT_THROW(sb::di::details::Require::validEnum(TestEnum::Count), sb::di::InjectorException);
    EXPECT_NO_THROW(sb::di::details::Require::validEnum(TestEnum::A));
    EXPECT_NO_THROW(sb::di::details::Require::validEnum(TestEnum::B));
    EXPECT_NO_THROW(sb::di::details::Require::validEnum(TestEnum::C));
}
