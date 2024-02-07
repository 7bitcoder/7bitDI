#include <gtest/gtest.h>

#include "../../Helpers/Classes/Basic.hpp"
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

TEST_F(ExternalServiceTest, ShouldCreateExternalService)
{
    TestClass1 test;
    const sb::di::details::services::ExternalService service{&test};

    EXPECT_TRUE(service.isValid());
    EXPECT_EQ(service.get(), &test);
    EXPECT_EQ(service.getTypeId(), typeid(TestClass1));
}

TEST_F(ExternalServiceTest, ShouldFailReleaseExternalService)
{
    TestClass1 test;
    sb::di::details::services::ExternalService service{&test};

    EXPECT_TRUE(service.isValid());
    EXPECT_THROW(service.release(), sb::di::CannotReleaseServiceException);
}

TEST_F(ExternalServiceTest, ShouldFailMoveOutExternalService)
{
    TestClass1 test;
    sb::di::details::services::ExternalService service{&test};

    EXPECT_TRUE(service.isValid());
    EXPECT_THROW(service.getForMoveOut(), sb::di::CannotMoveOutServiceException);
}

TEST_F(ExternalServiceTest, ShouldCreateExternalNullService)
{
    const sb::di::details::services::ExternalService<TestClass1> service{nullptr};

    EXPECT_FALSE(service.isValid());
    EXPECT_FALSE(service.get());
    EXPECT_EQ(service.getTypeId(), typeid(TestClass1));
}

TEST_F(ExternalServiceTest, ShouldFailReleaseExternalNullService)
{
    sb::di::details::services::ExternalService<TestClass1> service{nullptr};

    EXPECT_FALSE(service.isValid());
    EXPECT_THROW(service.release(), sb::di::CannotReleaseServiceException);
}

TEST_F(ExternalServiceTest, ShouldFailMoveOutExternalNullService)
{
    sb::di::details::services::ExternalService<TestClass1> service{nullptr};

    EXPECT_FALSE(service.isValid());
    EXPECT_THROW(service.getForMoveOut(), sb::di::CannotMoveOutServiceException);
}
