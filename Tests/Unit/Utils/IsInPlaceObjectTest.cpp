#include <gtest/gtest.h>

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
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectV<TestComplexClass1>);
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectV<TestComplexClass2>);
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectV<TestComplexClass3>);
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectV<TestComplexClass4>);
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectV<TestComplexClass5>);
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectV<int>);
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectV<double>);
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectV<float>);
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectV<size_t>);
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectV<int[12]>);
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectV<TestComplexClass1[12]>);
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectV<TestEnum>);
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectV<NoCopyableMovable>);
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectV<Copyable>);
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectV<Movable>);
}

TEST_F(IsInPlaceObjectTest, ShouldCheckIfObjectIsInPlacePtr)
{
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<TestComplexClass1 *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<TestComplexClass2 *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<TestComplexClass3 *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<TestComplexClass4 *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<TestComplexClass5 *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<int *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<double *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<float *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<size_t *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<TestEnum *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<NoCopyableMovable *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<Copyable *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<Movable *>);
}

TEST_F(IsInPlaceObjectTest, ShouldCheckIfObjectIsInPlaceRef)
{
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<TestComplexClass1 &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<TestComplexClass2 &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<TestComplexClass3 &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<TestComplexClass4 &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<TestComplexClass5 &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<int &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<double &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<float &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<size_t &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<TestEnum &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<NoCopyableMovable &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<Copyable &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectV<Movable &>);
}

TEST_F(IsInPlaceObjectTest, ShouldCheckIfObjectIsInPlaceConstructable)
{
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectConstructableV<TestComplexClass1>);
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectConstructableV<TestComplexClass2>);
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectConstructableV<TestComplexClass3>);
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectConstructableV<TestComplexClass4>);
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectConstructableV<TestComplexClass5>);
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectConstructableV<int>);
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectConstructableV<double>);
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectConstructableV<float>);
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectConstructableV<size_t>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<int[12]>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<TestComplexClass1[12]>);
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectConstructableV<TestEnum>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<NoCopyableMovable>);
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectConstructableV<Copyable>);
    EXPECT_TRUE(sb::di::details::IsInPlaceObjectConstructableV<Movable>);
}

TEST_F(IsInPlaceObjectTest, ShouldCheckIfObjectIsInPlaceConstructablePtr)
{
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<TestComplexClass1 *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<TestComplexClass2 *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<TestComplexClass3 *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<TestComplexClass4 *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<TestComplexClass5 *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<int *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<double *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<float *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<size_t *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<TestEnum *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<NoCopyableMovable *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<Copyable *>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<Movable *>);
}

TEST_F(IsInPlaceObjectTest, ShouldCheckIfObjectIsInPlaceConstructableRef)
{
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<TestComplexClass1 &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<TestComplexClass2 &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<TestComplexClass3 &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<TestComplexClass4 &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<TestComplexClass5 &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<int &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<double &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<float &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<size_t &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<TestEnum &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<NoCopyableMovable &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<Copyable &>);
    EXPECT_FALSE(sb::di::details::IsInPlaceObjectConstructableV<Movable &>);
}
