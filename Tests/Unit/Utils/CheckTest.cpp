#include <gtest/gtest.h>

#include "SevenBit/DI/Details/Utils/Check.hpp"

#include "../../Helpers/Classes/Basic.hpp"
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
