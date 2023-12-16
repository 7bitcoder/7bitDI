#include <gtest/gtest.h>

#include "../../Helpers/Classes/Basic.hpp"
#include "SevenBit/DI/Details/Helpers/CircularDependencyGuard.hpp"
#include "SevenBit/DI/Exceptions.hpp"

class CircularDependencyGuardTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    CircularDependencyGuardTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~CircularDependencyGuardTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(CircularDependencyGuardTest, ShouldNotDetectCirtularDependency)
{
    sb::di::details::helpers::CircularDependencyGuard guard;
    auto act = [&] {
        auto a = guard(typeid(TestClass1));
        {
            auto b = guard(typeid(TestClass2));
            {
                auto c = guard(typeid(TestClass3));
            }
        }
    };

    EXPECT_NO_THROW(act());
}

TEST_F(CircularDependencyGuardTest, ShouldDetectCirtularDependency)
{
    sb::di::details::helpers::CircularDependencyGuard guard;
    auto act = [&] {
        auto a = guard(typeid(TestClass1));
        {
            auto b = guard(typeid(TestClass2));
            {
                auto c = guard(typeid(TestClass3));
                {
                    auto d = guard(typeid(TestClass1));
                }
            }
        }
    };

    EXPECT_THROW(act(), sb::di::CircularDependencyException);
}
