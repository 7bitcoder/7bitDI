#include <gtest/gtest.h>
#include <memory>

#include "Classes/BasicTest.hpp"
#include "SevenBit/DI/Details/Services/UniquePtrService.hpp"
#include "SevenBit/DI/Exceptions.hpp"

class ServiceOwnerTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceOwnerTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceOwnerTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ServiceOwnerTest, ShouldProperelyCreateExternalService)
{
    const sb::di::details::services::UniquePtrService owner{std::make_unique<TestClass1>()};

    EXPECT_TRUE(owner);
    EXPECT_TRUE(owner.isValid());
    EXPECT_TRUE(owner.get());
    EXPECT_EQ(owner.getTypeId(), typeid(TestClass1));
}

TEST_F(ServiceOwnerTest, ShouldProperelyCreateExternalNullService)
{
    const sb::di::details::services::UniquePtrService<TestClass1> owner{nullptr};

    EXPECT_FALSE(owner);
    EXPECT_FALSE(owner.isValid());
    EXPECT_FALSE(owner.get());
    EXPECT_EQ(owner.getTypeId(), typeid(TestClass1));
}

TEST_F(ServiceOwnerTest, ShouldThrowOnMoveOut)
{
    sb::di::details::services::UniquePtrService owner{std::make_unique<TestClass1>()};

    const auto ptr = owner.release();
    EXPECT_TRUE(ptr);
    delete static_cast<TestClass1 *>(ptr);
}
