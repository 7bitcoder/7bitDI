#include <gtest/gtest.h>

#include "SevenBit/DI/Details/Utils/Check.hpp"

#include "../../Helpers/Classes/Complex.hpp"
#include "../../Helpers/Classes/MultiInherit.hpp"
#include "SevenBit/DI/Details/Utils/Inheritance.hpp"

class InheritanceTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    InheritanceTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~InheritanceTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(InheritanceTest, ShouldCheckInheritance)
{
    EXPECT_TRUE((sb::di::details::utils::InheritanceV<int, int>));
    EXPECT_TRUE((sb::di::details::utils::InheritanceV<const double, const double>));
    EXPECT_TRUE((sb::di::details::utils::InheritanceV<TestComplexClass1, TestComplexClass1>));
    EXPECT_TRUE((sb::di::details::utils::InheritanceV<TestMultiInheritClass3, TestMultiInheritClass3>));
    EXPECT_TRUE((sb::di::details::utils::InheritanceV<TestMultiInherit3Class1, TestMultiInheritClass3>));
    EXPECT_TRUE((sb::di::details::utils::InheritanceV<TestMultiInheritClass2, TestMultiInheritClass3>));
    EXPECT_TRUE((sb::di::details::utils::InheritanceV<TestMultiInheritClass4, TestMultiInheritClass4>));
    EXPECT_TRUE((sb::di::details::utils::InheritanceV<TestMultiInheritClass1, TestMultiInheritClass4>));
    EXPECT_TRUE((sb::di::details::utils::InheritanceV<TestMultiInherit2Class1, TestMultiInheritClass4>));
    EXPECT_TRUE((sb::di::details::utils::InheritanceV<TestMultiInherit3Class1, TestMultiInheritClass4>));
    EXPECT_TRUE((sb::di::details::utils::InheritanceV<const TestComplexClass1, TestComplexClass1>));
    EXPECT_TRUE((sb::di::details::utils::InheritanceV<const TestComplexClass1, const TestComplexClass1>));
    EXPECT_TRUE((sb::di::details::utils::InheritanceV<ITestComplexClass1, TestComplexClass1>));
    EXPECT_TRUE((sb::di::details::utils::InheritanceV<const ITestComplexClass1, TestComplexClass1>));
    EXPECT_TRUE((sb::di::details::utils::InheritanceV<const ITestComplexClass1, const TestComplexClass1>));
    EXPECT_TRUE((sb::di::details::utils::InheritanceV<TestComplexClass1, const TestComplexClass1>));
    EXPECT_FALSE((sb::di::details::utils::InheritanceV<const double, int>));
    EXPECT_FALSE((sb::di::details::utils::InheritanceV<double, int>));
    EXPECT_FALSE((sb::di::details::utils::InheritanceV<TestComplexClass2, TestComplexClass1>));
    EXPECT_FALSE((sb::di::details::utils::InheritanceV<TestComplexClass2, const TestComplexClass1>));
    EXPECT_FALSE((sb::di::details::utils::InheritanceV<TestComplexClass2, TestComplexClass3>));
}
