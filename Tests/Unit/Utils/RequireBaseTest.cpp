#include <gtest/gtest.h>

#include "../../Helpers/Classes/Basic.hpp"
#include "SevenBit/DI/Details/Utils/RequireBase.hpp"
#include "SevenBit/DI/Exceptions.hpp"

class RequireBaseTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    RequireBaseTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~RequireBaseTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(RequireBaseTest, ShouldRequireNotNull)
{
    TestClass1 test1;
    std::unique_ptr<TestClass1> testPtr{new TestClass1};
    std::shared_ptr<TestClass1> testSPtr{new TestClass1};
    EXPECT_NO_THROW(sb::di::details::RequireBase::notNull(&test1));
    EXPECT_NO_THROW(sb::di::details::RequireBase::notNullAndGet(&test1));
    EXPECT_NO_THROW(sb::di::details::RequireBase::notNull(testPtr));
    EXPECT_NO_THROW(sb::di::details::RequireBase::notNullAndGet(std::move(testPtr)));
    EXPECT_NO_THROW(sb::di::details::RequireBase::notNull(testSPtr));
    EXPECT_NO_THROW(sb::di::details::RequireBase::notNullAndGet(std::move(testSPtr)));
}

TEST_F(RequireBaseTest, ShouldFailRequireNotNull)
{
    std::unique_ptr<TestClass1> testPtr;
    std::unique_ptr<TestClass1> testSPtr;
    EXPECT_THROW(sb::di::details::RequireBase::notNull<TestClass1>(nullptr), sb::di::NullPointerException);
    EXPECT_THROW(sb::di::details::RequireBase::notNullAndGet<TestClass1>(nullptr), sb::di::NullPointerException);
    EXPECT_THROW(sb::di::details::RequireBase::notNull(testPtr), sb::di::NullPointerException);
    EXPECT_THROW(sb::di::details::RequireBase::notNullAndGet(std::move(testPtr)), sb::di::NullPointerException);
    EXPECT_THROW(sb::di::details::RequireBase::notNull(testSPtr), sb::di::NullPointerException);
    EXPECT_THROW(sb::di::details::RequireBase::notNullAndGet(std::move(testSPtr)), sb::di::NullPointerException);
}

TEST_F(RequireBaseTest, ShouldRequireValidEnum)
{
    enum TestEnum
    {
        A,
        B,
        C,
        Count,
    };
    EXPECT_THROW(sb::di::details::RequireBase::validEnum(static_cast<TestEnum>(123)), sb::di::InjectorException);
    EXPECT_THROW(sb::di::details::RequireBase::validEnum(static_cast<TestEnum>(-123)),
                 sb::di::InjectorException);
    EXPECT_THROW(sb::di::details::RequireBase::validEnum(static_cast<TestEnum>(-1)), sb::di::InjectorException);
    EXPECT_THROW(sb::di::details::RequireBase::validEnum(TestEnum::Count), sb::di::InjectorException);
    EXPECT_NO_THROW(sb::di::details::RequireBase::validEnum(TestEnum::A));
    EXPECT_NO_THROW(sb::di::details::RequireBase::validEnum(TestEnum::B));
    EXPECT_NO_THROW(sb::di::details::RequireBase::validEnum(TestEnum::C));
}
