#include <gtest/gtest.h>

#include "../../Helpers/Classes/Complex.hpp"
#include "SevenBit/DI/Details/Utils/IsUniquePtr.hpp"

class IsUniquePtrTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    IsUniquePtrTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~IsUniquePtrTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(IsUniquePtrTest, ShouldCheckIfObjectIsUniquePtr)
{
    EXPECT_TRUE(sb::di::details::utils::IsUniquePtrV<std::unique_ptr<TestComplexClass1>>);
    EXPECT_TRUE(sb::di::details::utils::IsUniquePtrV<std::unique_ptr<TestComplexClass2>>);
    EXPECT_TRUE(sb::di::details::utils::IsUniquePtrV<std::unique_ptr<TestComplexClass3>>);
    EXPECT_TRUE(sb::di::details::utils::IsUniquePtrV<std::unique_ptr<TestComplexClass4>>);
    EXPECT_TRUE(sb::di::details::utils::IsUniquePtrV<std::unique_ptr<TestComplexClass5>>);
    EXPECT_TRUE(sb::di::details::utils::IsUniquePtrV<std::unique_ptr<int>>);
    EXPECT_FALSE(sb::di::details::utils::IsUniquePtrV<TestComplexClass2>);
    EXPECT_FALSE(sb::di::details::utils::IsUniquePtrV<TestComplexClass3>);
    EXPECT_FALSE(sb::di::details::utils::IsUniquePtrV<TestComplexClass4>);
    EXPECT_FALSE(sb::di::details::utils::IsUniquePtrV<TestComplexClass5>);
    EXPECT_FALSE(sb::di::details::utils::IsUniquePtrV<int>);
    EXPECT_FALSE(sb::di::details::utils::IsUniquePtrV<double>);
    EXPECT_FALSE(sb::di::details::utils::IsUniquePtrV<float>);
    EXPECT_FALSE(sb::di::details::utils::IsUniquePtrV<size_t>);
    EXPECT_FALSE(sb::di::details::utils::IsUniquePtrV<int[12]>);
    EXPECT_FALSE(sb::di::details::utils::IsUniquePtrV<TestComplexClass1[12]>);
}
