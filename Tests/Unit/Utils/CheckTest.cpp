#include <gtest/gtest.h>

#include "SevenBit/DI/Details/Utils/Check.hpp"

#include "../../Helpers/Classes/Basic.hpp"
#include "../../Helpers/Classes/Inherit.hpp"
#include "SevenBit/DI/Details/Services/ExternalService.hpp"
#include "SevenBit/DI/Details/Services/InPlaceService.hpp"
#include "SevenBit/DI/Details/Services/UniquePtrService.hpp"

class CheckTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    CheckTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~CheckTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(CheckTest, ShouldCheckNotNull)
{
    EXPECT_FALSE(sb::di::details::utils::Check::notNull<int>(nullptr));
    EXPECT_FALSE(sb::di::details::utils::Check::notNull(std::unique_ptr<int>()));
    EXPECT_FALSE(sb::di::details::utils::Check::notNull(std::shared_ptr<int>()));

    int intTest = 123;
    EXPECT_TRUE(sb::di::details::utils::Check::notNull(&intTest));
    EXPECT_TRUE(sb::di::details::utils::Check::notNull(std::make_unique<int>(intTest)));
    EXPECT_TRUE(sb::di::details::utils::Check::notNull(std::make_shared<int>(intTest)));
}

TEST_F(CheckTest, ShouldCheckEnum)
{
    enum TestEnum
    {
        A,
        B,
        C,
        Count,
    };
    EXPECT_FALSE(sb::di::details::utils::Check::enumValidity(static_cast<TestEnum>(123)));
    EXPECT_FALSE(sb::di::details::utils::Check::enumValidity(static_cast<TestEnum>(-123)));
    EXPECT_FALSE(sb::di::details::utils::Check::enumValidity(static_cast<TestEnum>(-1)));
    EXPECT_FALSE(sb::di::details::utils::Check::enumValidity(TestEnum::Count));
    EXPECT_TRUE(sb::di::details::utils::Check::enumValidity(TestEnum::A));
    EXPECT_TRUE(sb::di::details::utils::Check::enumValidity(TestEnum::B));
    EXPECT_TRUE(sb::di::details::utils::Check::enumValidity(TestEnum::C));
}

TEST_F(CheckTest, ShouldCheckInstanceValidity)
{
    TestClass1 test;
    EXPECT_FALSE(sb::di::details::utils::Check::instanceValidity(nullptr));
    EXPECT_FALSE(sb::di::details::utils::Check::instanceValidity(
        std::make_unique<sb::di::details::services::ExternalService<TestClass1>>(nullptr)));
    EXPECT_TRUE(sb::di::details::utils::Check::instanceValidity(
        std::make_unique<sb::di::details::services::ExternalService<TestClass1>>(&test)));
    EXPECT_FALSE(sb::di::details::utils::Check::instanceValidity(
        std::make_unique<sb::di::details::services::UniquePtrService<TestClass1>>(nullptr)));
    EXPECT_TRUE(sb::di::details::utils::Check::instanceValidity(
        std::make_unique<sb::di::details::services::UniquePtrService<TestClass1>>(std::make_unique<TestClass1>())));
    EXPECT_TRUE(sb::di::details::utils::Check::instanceValidity(
        std::make_unique<sb::di::details::services::InPlaceService<TestClass1>>()));
}
