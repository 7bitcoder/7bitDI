#include <gtest/gtest.h>

#include "Classes/BasicTest.hpp"
#include "SevenBit/DI/Details/Services/ExternalService.hpp"
#include "SevenBit/DI/Exceptions.hpp"

class ExternalServiceTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ExternalServiceTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ExternalServiceTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ExternalServiceTest, ShouldProperelyCreateExternalService)
{
    TestClass1 test;
    sb::di::details::services::ExternalService external{&test};

    EXPECT_TRUE(external);
    EXPECT_TRUE(external.isValid());
    EXPECT_EQ(external.get(), &test);
    EXPECT_THROW(external.release(), sb::di::CannotReleaseServiceException);
    EXPECT_THROW(external.getForMoveOut(), sb::di::CannotMoveOutServiceException);
    EXPECT_EQ(external.getTypeId(), typeid(TestClass1));
}

TEST_F(ExternalServiceTest, ShouldFailCreateExternalNullService)
{
    TestClass1 test;
    sb::di::details::services::ExternalService<TestClass1> external{nullptr};

    EXPECT_FALSE(external);
    EXPECT_FALSE(external.isValid());
    EXPECT_FALSE(external.get());
    EXPECT_THROW(external.release(), sb::di::CannotReleaseServiceException);
    EXPECT_THROW(external.getForMoveOut(), sb::di::CannotMoveOutServiceException);
    EXPECT_EQ(external.getTypeId(), typeid(TestClass1));
}
