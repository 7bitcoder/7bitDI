#include <gtest/gtest.h>

#include "SevenBit/DI/Details/Utils/Check.hpp"

#include "../../Helpers/Classes/Complex.hpp"
#include "SevenBit/DI/Details/Utils/IsInPlaceObject.hpp"

class IsInPlaceObjectTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    IsInPlaceObjectTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~IsInPlaceObjectTest() override = default;

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

TEST_F(IsInPlaceObjectTest, ShouldCheckIfObjectIsInPlace)
{
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectV<TestComplexClass1>);
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectV<TestComplexClass2>);
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectV<TestComplexClass3>);
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectV<TestComplexClass4>);
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectV<TestComplexClass5>);
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectV<int>);
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectV<double>);
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectV<float>);
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectV<size_t>);
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectV<int[12]>);
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectV<TestComplexClass1[12]>);
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectV<TestEnum>);
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectV<NoCopyableMovable>);
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectV<Copyable>);
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectV<Movable>);
}

TEST_F(IsInPlaceObjectTest, ShouldCheckIfObjectIsInPlacePtr)
{
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<TestComplexClass1 *>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<TestComplexClass2 *>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<TestComplexClass3 *>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<TestComplexClass4 *>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<TestComplexClass5 *>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<int *>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<double *>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<float *>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<size_t *>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<TestEnum *>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<NoCopyableMovable *>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<Copyable *>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<Movable *>);
}

TEST_F(IsInPlaceObjectTest, ShouldCheckIfObjectIsInPlaceRef)
{
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<TestComplexClass1 &>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<TestComplexClass2 &>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<TestComplexClass3 &>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<TestComplexClass4 &>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<TestComplexClass5 &>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<int &>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<double &>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<float &>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<size_t &>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<TestEnum &>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<NoCopyableMovable &>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<Copyable &>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectV<Movable &>);
}

TEST_F(IsInPlaceObjectTest, ShouldCheckIfObjectIsInPlaceConstructable)
{
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectConstructableV<TestComplexClass1>);
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectConstructableV<TestComplexClass2>);
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectConstructableV<TestComplexClass3>);
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectConstructableV<TestComplexClass4>);
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectConstructableV<TestComplexClass5>);
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectConstructableV<int>);
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectConstructableV<double>);
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectConstructableV<float>);
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectConstructableV<size_t>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<int[12]>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<TestComplexClass1[12]>);
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectConstructableV<TestEnum>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<NoCopyableMovable>);
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectConstructableV<Copyable>);
    EXPECT_TRUE(sb::di::details::utils::IsInPlaceObjectConstructableV<Movable>);
}

TEST_F(IsInPlaceObjectTest, ShouldCheckIfObjectIsInPlaceConstructablePtr)
{
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<TestComplexClass1 *>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<TestComplexClass2 *>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<TestComplexClass3 *>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<TestComplexClass4 *>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<TestComplexClass5 *>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<int *>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<double *>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<float *>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<size_t *>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<TestEnum *>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<NoCopyableMovable *>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<Copyable *>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<Movable *>);
}

TEST_F(IsInPlaceObjectTest, ShouldCheckIfObjectIsInPlaceConstructableRef)
{
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<TestComplexClass1 &>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<TestComplexClass2 &>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<TestComplexClass3 &>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<TestComplexClass4 &>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<TestComplexClass5 &>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<int &>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<double &>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<float &>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<size_t &>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<TestEnum &>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<NoCopyableMovable &>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<Copyable &>);
    EXPECT_FALSE(sb::di::details::utils::IsInPlaceObjectConstructableV<Movable &>);
}
