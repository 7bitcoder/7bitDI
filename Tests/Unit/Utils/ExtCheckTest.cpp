#include <gtest/gtest.h>

#include "../../Helpers/Classes/Basic.hpp"
#include "SevenBit/DI/Details/Services/ExternalService.hpp"
#include "SevenBit/DI/Details/Services/InPlaceService.hpp"
#include "SevenBit/DI/Details/Services/UniquePtrService.hpp"
#include "SevenBit/DI/Details/Utils/ExtCheck.hpp"

class ExtCheckTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ExtCheckTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ExtCheckTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ExtCheckTest, ShoulCheckInstanceValidity)
{
    TestClass1 test;
    EXPECT_FALSE(sb::di::details::ExtCheck::instanceValidity(nullptr));
    EXPECT_FALSE(sb::di::details::ExtCheck::instanceValidity(sb::di::ServiceInstance{}));
    EXPECT_FALSE(sb::di::details::ExtCheck::instanceValidity(
        sb::di::ServiceInstance{std::make_unique<sb::di::details::ExternalService<TestClass1>>(nullptr)}));
    EXPECT_TRUE(sb::di::details::ExtCheck::instanceValidity(
        sb::di::ServiceInstance{std::make_unique<sb::di::details::ExternalService<TestClass1>>(&test)}));
    EXPECT_FALSE(sb::di::details::ExtCheck::instanceValidity(
        sb::di::ServiceInstance{std::make_unique<sb::di::details::UniquePtrService<TestClass1>>(nullptr)}));
    EXPECT_TRUE(sb::di::details::ExtCheck::instanceValidity(sb::di::ServiceInstance{
        std::make_unique<sb::di::details::UniquePtrService<TestClass1>>(std::make_unique<TestClass1>())}));
    EXPECT_TRUE(sb::di::details::ExtCheck::instanceValidity(
        sb::di::ServiceInstance{std::make_unique<sb::di::details::InPlaceService<TestClass1>>()}));
}
