#include <gtest/gtest.h>
#include <memory>

#include "../../Helpers/Classes/Basic.hpp"
#include "SevenBit/DI/Details/Services/InPlaceService.hpp"
#include "SevenBit/DI/Exceptions.hpp"

class InPlaceServiceTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    InPlaceServiceTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~InPlaceServiceTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(InPlaceServiceTest, ShouldCreateInPlaceService)
{
    const sb::di::details::services::InPlaceService<TestClass1> service{};

    EXPECT_TRUE(service.isValid());
    EXPECT_TRUE(service.get());
    EXPECT_EQ(service.getTypeId(), typeid(TestClass1));
}

TEST_F(InPlaceServiceTest, ShouldFailMoveAsUniquePtrInPlaceService)
{
    TestClass1 test;
    sb::di::details::services::InPlaceService<TestClass1> service{};

    EXPECT_TRUE(service.isValid());
    EXPECT_THROW(service.release(), sb::di::CannotReleaseServiceException);
    EXPECT_TRUE(service.isValid());
}

TEST_F(InPlaceServiceTest, ShouldSuccedMoveOutInPlaceService)
{
    TestClass1 test;
    sb::di::details::services::InPlaceService<TestClass1> service{};

    EXPECT_TRUE(service.isValid());
    EXPECT_NO_THROW(service.getForMoveOut());
    EXPECT_TRUE(service.isValid());
}
