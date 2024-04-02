#include <gtest/gtest.h>

#include "../../Helpers/Classes/Complex.hpp"
#include "../../Helpers/Classes/MultiInherit.hpp"
#include "SevenBit/DI/Details/Utils/Meta.hpp"

class MetaTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    MetaTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~MetaTest() override = default;

    static void TearDownTestSuite() {}
};

struct Copyable
{
    Copyable(const Copyable &) = default;
    Copyable(Copyable &&) = delete;
};

struct Movable
{
    Movable(const Movable &) = delete;
    Movable(Movable &&) = default;
};

struct NoCopyableMovable
{
    NoCopyableMovable(const NoCopyableMovable &) = delete;
    NoCopyableMovable(NoCopyableMovable &&) = delete;
};

enum TestEnum
{
    ONE,
    TWO
};

TEST_F(MetaTest, ShouldCheckIfObjectIsInPlace)
{
    EXPECT_TRUE(sb::di::details::IsServiceTypeV<TestComplexClass1>);
    EXPECT_TRUE(sb::di::details::IsServiceTypeV<TestComplexClass2>);
    EXPECT_TRUE(sb::di::details::IsServiceTypeV<TestComplexClass3>);
    EXPECT_TRUE(sb::di::details::IsServiceTypeV<TestComplexClass4>);
    EXPECT_TRUE(sb::di::details::IsServiceTypeV<TestComplexClass5>);
    EXPECT_TRUE(sb::di::details::IsServiceTypeV<int>);
    EXPECT_TRUE(sb::di::details::IsServiceTypeV<double>);
    EXPECT_TRUE(sb::di::details::IsServiceTypeV<float>);
    EXPECT_TRUE(sb::di::details::IsServiceTypeV<size_t>);
    EXPECT_TRUE(sb::di::details::IsServiceTypeV<int[12]>);
    EXPECT_TRUE(sb::di::details::IsServiceTypeV<TestComplexClass1[12]>);
    EXPECT_TRUE(sb::di::details::IsServiceTypeV<TestEnum>);
    EXPECT_TRUE(sb::di::details::IsServiceTypeV<NoCopyableMovable>);
    EXPECT_TRUE(sb::di::details::IsServiceTypeV<Copyable>);
    EXPECT_TRUE(sb::di::details::IsServiceTypeV<Movable>);
}

TEST_F(MetaTest, ShouldCheckIfObjectIsInPlacePtr)
{
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<TestComplexClass1 *>);
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<TestComplexClass2 *>);
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<TestComplexClass3 *>);
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<TestComplexClass4 *>);
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<TestComplexClass5 *>);
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<int *>);
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<double *>);
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<float *>);
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<size_t *>);
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<TestEnum *>);
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<NoCopyableMovable *>);
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<Copyable *>);
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<Movable *>);
}

TEST_F(MetaTest, ShouldCheckIfObjectIsInPlaceRef)
{
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<TestComplexClass1 &>);
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<TestComplexClass2 &>);
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<TestComplexClass3 &>);
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<TestComplexClass4 &>);
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<TestComplexClass5 &>);
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<int &>);
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<double &>);
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<float &>);
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<size_t &>);
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<TestEnum &>);
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<NoCopyableMovable &>);
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<Copyable &>);
    EXPECT_FALSE(sb::di::details::IsServiceTypeV<Movable &>);
}

TEST_F(MetaTest, ShouldCheckIfObjectIsInPlaceConstructable)
{
    EXPECT_TRUE(sb::di::details::IsInPlaceServiceV<TestComplexClass1>);
    EXPECT_TRUE(sb::di::details::IsInPlaceServiceV<TestComplexClass2>);
    EXPECT_TRUE(sb::di::details::IsInPlaceServiceV<TestComplexClass3>);
    EXPECT_TRUE(sb::di::details::IsInPlaceServiceV<TestComplexClass4>);
    EXPECT_TRUE(sb::di::details::IsInPlaceServiceV<TestComplexClass5>);
    EXPECT_TRUE(sb::di::details::IsInPlaceServiceV<int>);
    EXPECT_TRUE(sb::di::details::IsInPlaceServiceV<double>);
    EXPECT_TRUE(sb::di::details::IsInPlaceServiceV<float>);
    EXPECT_TRUE(sb::di::details::IsInPlaceServiceV<size_t>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<int[12]>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<TestComplexClass1[12]>);
    EXPECT_TRUE(sb::di::details::IsInPlaceServiceV<TestEnum>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<NoCopyableMovable>);
    EXPECT_TRUE(sb::di::details::IsInPlaceServiceV<Copyable>);
    EXPECT_TRUE(sb::di::details::IsInPlaceServiceV<Movable>);
}

TEST_F(MetaTest, ShouldCheckIfObjectIsInPlaceConstructablePtr)
{
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<TestComplexClass1 *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<TestComplexClass2 *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<TestComplexClass3 *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<TestComplexClass4 *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<TestComplexClass5 *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<int *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<double *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<float *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<size_t *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<TestEnum *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<NoCopyableMovable *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<Copyable *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<Movable *>);
}

TEST_F(MetaTest, ShouldCheckIfObjectIsInPlaceConstructableRef)
{
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<TestComplexClass1 &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<TestComplexClass2 &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<TestComplexClass3 &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<TestComplexClass4 &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<TestComplexClass5 &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<int &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<double &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<float &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<size_t &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<TestEnum &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<NoCopyableMovable &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<Copyable &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceServiceV<Movable &>);
}

TEST_F(MetaTest, ShouldCheckIfObjectIsUniquePtr)
{
    EXPECT_TRUE(sb::di::details::IsUniquePtrV<std::unique_ptr<TestComplexClass1>>);
    EXPECT_TRUE(sb::di::details::IsUniquePtrV<std::unique_ptr<TestComplexClass2>>);
    EXPECT_TRUE(sb::di::details::IsUniquePtrV<std::unique_ptr<TestComplexClass3>>);
    EXPECT_TRUE(sb::di::details::IsUniquePtrV<std::unique_ptr<TestComplexClass4>>);
    EXPECT_TRUE(sb::di::details::IsUniquePtrV<std::unique_ptr<TestComplexClass5>>);
    EXPECT_TRUE(sb::di::details::IsUniquePtrV<std::unique_ptr<int>>);
    EXPECT_FALSE(sb::di::details::IsUniquePtrV<TestComplexClass2>);
    EXPECT_FALSE(sb::di::details::IsUniquePtrV<TestComplexClass3>);
    EXPECT_FALSE(sb::di::details::IsUniquePtrV<TestComplexClass4>);
    EXPECT_FALSE(sb::di::details::IsUniquePtrV<TestComplexClass5>);
    EXPECT_FALSE(sb::di::details::IsUniquePtrV<int>);
    EXPECT_FALSE(sb::di::details::IsUniquePtrV<double>);
    EXPECT_FALSE(sb::di::details::IsUniquePtrV<float>);
    EXPECT_FALSE(sb::di::details::IsUniquePtrV<size_t>);
    EXPECT_FALSE(sb::di::details::IsUniquePtrV<int[12]>);
    EXPECT_FALSE(sb::di::details::IsUniquePtrV<TestComplexClass1[12]>);
}

TEST_F(MetaTest, ShouldCheckForCopyCtor)
{
    EXPECT_TRUE((sb::di::details::IsCopyCtorV<TestComplexClass1, TestComplexClass1>));
    EXPECT_TRUE((sb::di::details::IsCopyCtorV<TestComplexClass1, const TestComplexClass1>));
    EXPECT_FALSE((sb::di::details::IsCopyCtorV<TestComplexClass2, TestComplexClass1>));
    EXPECT_FALSE((sb::di::details::IsCopyCtorV<TestComplexClass2, const TestComplexClass1>));
    EXPECT_FALSE((sb::di::details::IsCopyCtorV<TestComplexClass2, TestComplexClass3>));
}

TEST_F(MetaTest, ShouldCheckInheritance)
{
    EXPECT_TRUE((sb::di::details::InheritanceV<int, int>));
    EXPECT_TRUE((sb::di::details::InheritanceV<const double, const double>));
    EXPECT_TRUE((sb::di::details::InheritanceV<TestComplexClass1, TestComplexClass1>));
    EXPECT_TRUE((sb::di::details::InheritanceV<TestMultiInheritClass3, TestMultiInheritClass3>));
    EXPECT_TRUE((sb::di::details::InheritanceV<TestMultiInherit3Class1, TestMultiInheritClass3>));
    EXPECT_TRUE((sb::di::details::InheritanceV<TestMultiInheritClass2, TestMultiInheritClass3>));
    EXPECT_TRUE((sb::di::details::InheritanceV<TestMultiInheritClass4, TestMultiInheritClass4>));
    EXPECT_TRUE((sb::di::details::InheritanceV<TestMultiInheritClass1, TestMultiInheritClass4>));
    EXPECT_TRUE((sb::di::details::InheritanceV<TestMultiInherit2Class1, TestMultiInheritClass4>));
    EXPECT_TRUE((sb::di::details::InheritanceV<TestMultiInherit3Class1, TestMultiInheritClass4>));
    EXPECT_TRUE((sb::di::details::InheritanceV<const TestComplexClass1, TestComplexClass1>));
    EXPECT_TRUE((sb::di::details::InheritanceV<const TestComplexClass1, const TestComplexClass1>));
    EXPECT_TRUE((sb::di::details::InheritanceV<ITestComplexClass1, TestComplexClass1>));
    EXPECT_TRUE((sb::di::details::InheritanceV<const ITestComplexClass1, TestComplexClass1>));
    EXPECT_TRUE((sb::di::details::InheritanceV<const ITestComplexClass1, const TestComplexClass1>));
    EXPECT_TRUE((sb::di::details::InheritanceV<TestComplexClass1, const TestComplexClass1>));
    EXPECT_FALSE((sb::di::details::InheritanceV<const double, int>));
    EXPECT_FALSE((sb::di::details::InheritanceV<double, int>));
    EXPECT_FALSE((sb::di::details::InheritanceV<TestComplexClass2, TestComplexClass1>));
    EXPECT_FALSE((sb::di::details::InheritanceV<TestComplexClass2, const TestComplexClass1>));
    EXPECT_FALSE((sb::di::details::InheritanceV<TestComplexClass2, TestComplexClass3>));
}

TEST_F(MetaTest, ShouldRemoveUniquePtrTest)
{
    EXPECT_TRUE(
        (std::is_same_v<sb::di::details::RemoveUniquePtrT<std::unique_ptr<TestComplexClass1>>, TestComplexClass1>));
    EXPECT_TRUE(
        (std::is_same_v<sb::di::details::RemoveUniquePtrT<std::unique_ptr<TestComplexClass2>>, TestComplexClass2>));
    EXPECT_FALSE(
        (std::is_same_v<sb::di::details::RemoveUniquePtrT<std::unique_ptr<TestComplexClass2>>, TestComplexClass1>));
}
