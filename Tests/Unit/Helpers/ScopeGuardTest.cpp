#include <gtest/gtest.h>

#include "../../Helpers/Classes/Basic.hpp"
#include "SevenBit/DI/Details/Helpers/ScopedGuard.hpp"
#include "SevenBit/DI/Exceptions.hpp"

class ScopeGuardTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ScopeGuardTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ScopeGuardTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ScopeGuardTest, ShouldNotDetectCirtularDependency)
{
    std::unordered_set<sb::di::TypeId> typeIdsUnderConstruction;
    auto act = [&] {
        sb::di::details::ScopedGuard guard{typeid(TestClass1), typeIdsUnderConstruction};
        {
            sb::di::details::ScopedGuard guard2{typeid(TestClass2), typeIdsUnderConstruction};
            {
                sb::di::details::ScopedGuard guard3{typeid(TestClass3), typeIdsUnderConstruction};
            }
        }
    };

    EXPECT_NO_THROW(act());
    EXPECT_TRUE(typeIdsUnderConstruction.empty());
}

TEST_F(ScopeGuardTest, ShouldDetectCirtularDependency)
{
    std::unordered_set<sb::di::TypeId> typeIdsUnderConstruction;
    auto act = [&] {
        sb::di::details::ScopedGuard guard{typeid(TestClass1), typeIdsUnderConstruction};
        {
            sb::di::details::ScopedGuard guard2{typeid(TestClass2), typeIdsUnderConstruction};
            {
                sb::di::details::ScopedGuard guard3{typeid(TestClass3), typeIdsUnderConstruction};
                {
                    sb::di::details::ScopedGuard guar4{typeid(TestClass4), typeIdsUnderConstruction};
                    {
                        sb::di::details::ScopedGuard guard5{typeid(TestClass5), typeIdsUnderConstruction};
                        {
                            sb::di::details::ScopedGuard guard6{typeid(TestClass1), typeIdsUnderConstruction};
                        }
                    }
                }
            }
        }
    };

    EXPECT_THROW(act(), sb::di::CircularDependencyException);
    EXPECT_TRUE(typeIdsUnderConstruction.empty());
}
