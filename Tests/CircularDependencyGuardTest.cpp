#include <gtest/gtest.h>
#include <iostream>

#include "Classes/BasicTest.hpp"
#include "SevenBit/DI/Details/CircularDependencyGuard.hpp"
#include "SevenBit/DI/Exceptions.hpp"

class CircularDependencyGuardTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    CircularDependencyGuardTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~CircularDependencyGuardTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(CircularDependencyGuardTest, ShouldNotDetectCirtularDependency)
{
    sb::di::details::CircularDependencyGuard guard;
    auto act = [&]() {
        auto _ = guard(typeid(TestClass1));
        {
            auto _ = guard(typeid(TestClass2));
            {
                auto _ = guard(typeid(TestClass3));
            }
        }
    };

    EXPECT_NO_THROW((act()));
}

TEST_F(CircularDependencyGuardTest, ShouldDetectCirtularDependency)
{
    sb::di::details::CircularDependencyGuard guard;
    auto act = [&]() {
        auto _ = guard(typeid(TestClass1));
        {
            auto _ = guard(typeid(TestClass2));
            {
                auto _ = guard(typeid(TestClass3));
                {
                    auto _ = guard(typeid(TestClass1));
                }
            }
        }
    };

    EXPECT_THROW((act()), sb::di::CircularDependencyException);
}