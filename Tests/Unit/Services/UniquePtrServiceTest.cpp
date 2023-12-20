#include <gtest/gtest.h>
#include <memory>

#include "../../Helpers/Classes/Basic.hpp"
#include "SevenBit/DI/Details/Services/UniquePtrService.hpp"
#include "SevenBit/DI/Exceptions.hpp"

class UniquePtrServiceTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    UniquePtrServiceTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~UniquePtrServiceTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(UniquePtrServiceTest, ShouldCreateUniquePtrService)
{
    const sb::di::details::services::UniquePtrService service{std::make_unique<TestClass1>()};

    EXPECT_TRUE(service);
    EXPECT_TRUE(service.isValid());
    EXPECT_TRUE(service.get());
    EXPECT_EQ(service.getTypeId(), typeid(TestClass1));
}

TEST_F(UniquePtrServiceTest, ShouldSuccedMoveAsUniquePtrUniquePtrService)
{
    TestClass1 test;
    sb::di::details::services::UniquePtrService service{std::make_unique<TestClass1>()};

    EXPECT_TRUE(service);
    EXPECT_TRUE(service.isValid());
    EXPECT_TRUE(service.moveOutAsUniquePtr<TestClass1>());
    EXPECT_FALSE(service.isValid());
}

TEST_F(UniquePtrServiceTest, ShouldSuccedMoveOutUniquePtrService)
{
    TestClass1 test;
    sb::di::details::services::UniquePtrService service{std::make_unique<TestClass1>()};

    EXPECT_TRUE(service);
    EXPECT_TRUE(service.isValid());
    EXPECT_NO_THROW(service.moveOutAs<TestClass1>());
    EXPECT_TRUE(service.isValid());
}

TEST_F(UniquePtrServiceTest, ShouldCreateExternalNullService)
{
    const sb::di::details::services::UniquePtrService<TestClass1> service{nullptr};

    EXPECT_FALSE(service);
    EXPECT_FALSE(service.isValid());
    EXPECT_FALSE(service.get());
    EXPECT_EQ(service.getTypeId(), typeid(TestClass1));
}

TEST_F(UniquePtrServiceTest, ShouldFailMoveAsUniquePtrExternalNullService)
{
    TestClass1 test;
    sb::di::details::services::UniquePtrService<TestClass1> service{nullptr};

    EXPECT_FALSE(service);
    EXPECT_FALSE(service.isValid());
    EXPECT_FALSE(service.moveOutAsUniquePtr<TestClass1>());
    EXPECT_FALSE(service.isValid());
}

TEST_F(UniquePtrServiceTest, ShouldFailMoveOutExternalNullService)
{
    TestClass1 test;
    sb::di::details::services::UniquePtrService<TestClass1> service{nullptr};

    EXPECT_FALSE(service);
    EXPECT_FALSE(service.isValid());
    EXPECT_NO_THROW(service.moveOutAs<TestClass1>());
    EXPECT_FALSE(service.isValid());
}