#include <gtest/gtest.h>

#include "../../Helpers/Classes/Complex.hpp"
#include "SevenBit/DI/Details/Utils/RemoveUniquePtr.hpp"

class RemoveUniquePtrTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    RemoveUniquePtrTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~RemoveUniquePtrTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(RemoveUniquePtrTest, ShouldRemoveUniquePtrTest)
{
    EXPECT_TRUE((std::is_same_v<sb::di::details::utils::RemoveUniquePtrT<std::unique_ptr<TestComplexClass1>>,
                                TestComplexClass1>));
    EXPECT_TRUE((std::is_same_v<sb::di::details::utils::RemoveUniquePtrT<std::unique_ptr<TestComplexClass2>>,
                                TestComplexClass2>));
    EXPECT_FALSE((std::is_same_v<sb::di::details::utils::RemoveUniquePtrT<std::unique_ptr<TestComplexClass2>>,
                                 TestComplexClass1>));
}
