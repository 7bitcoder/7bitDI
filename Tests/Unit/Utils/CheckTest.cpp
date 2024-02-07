#include <gtest/gtest.h>

#include "../../Helpers/Classes/Basic.hpp"
#include "SevenBit/DI/Details/Services/ExternalService.hpp"
#include "SevenBit/DI/Details/Services/InPlaceService.hpp"
#include "SevenBit/DI/Details/Services/UniquePtrService.hpp"
#include "SevenBit/DI/Details/Utils/Check.hpp"

class CheckExtTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    CheckExtTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~CheckExtTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(CheckExtTest, ShoulCheckInstanceValidity)
{
    TestClass1 test;
    EXPECT_FALSE(sb::di::details::utils::Check::instanceValidity(nullptr));
    EXPECT_FALSE(sb::di::details::utils::Check::instanceValidity(sb::di::ServiceInstance{}));
    EXPECT_FALSE(sb::di::details::utils::Check::instanceValidity(
        sb::di::ServiceInstance{std::make_unique<sb::di::details::services::ExternalService<TestClass1>>(nullptr)}));
    EXPECT_TRUE(sb::di::details::utils::Check::instanceValidity(
        sb::di::ServiceInstance{std::make_unique<sb::di::details::services::ExternalService<TestClass1>>(&test)}));
    EXPECT_FALSE(sb::di::details::utils::Check::instanceValidity(
        sb::di::ServiceInstance{std::make_unique<sb::di::details::services::UniquePtrService<TestClass1>>(nullptr)}));
    EXPECT_TRUE(sb::di::details::utils::Check::instanceValidity(sb::di::ServiceInstance{
        std::make_unique<sb::di::details::services::UniquePtrService<TestClass1>>(std::make_unique<TestClass1>())}));
    EXPECT_TRUE(sb::di::details::utils::Check::instanceValidity(
        sb::di::ServiceInstance{std::make_unique<sb::di::details::services::InPlaceService<TestClass1>>()}));
}
