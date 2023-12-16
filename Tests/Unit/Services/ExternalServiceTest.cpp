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

    EXPECT_TRUE(service);
    EXPECT_TRUE(service.isValid());
    EXPECT_EQ(service.get(), &test);
    EXPECT_EQ(service.getTypeId(), typeid(TestClass1));
}

TEST_F(ExternalServiceTest, ShouldUpdateExternalService)
{
    TestClass1 test;
    sb::di::details::services::ExternalService service{&test};

    EXPECT_TRUE(service);
    EXPECT_TRUE(service.isValid());
    EXPECT_EQ(service.get(), &test);

    TestClass1 test2;
    service.update(&test2);

    EXPECT_TRUE(service);
    EXPECT_TRUE(service.isValid());
    EXPECT_EQ(service.get(), &test2);
}

TEST_F(ExternalServiceTest, ShouldFailMoveAsUniquePtrExternalService)
{
    TestClass1 test;
    sb::di::details::services::ExternalService service{&test};

    EXPECT_TRUE(service);
    EXPECT_TRUE(service.isValid());
    EXPECT_THROW(service.moveOutAsUniquePtr<TestClass1>(), sb::di::CannotReleaseServiceException);
}

TEST_F(ExternalServiceTest, ShouldFailMoveOutExternalService)
{
    TestClass1 test;
    sb::di::details::services::ExternalService service{&test};

    EXPECT_TRUE(service);
    EXPECT_TRUE(service.isValid());
    EXPECT_THROW(service.moveOutAs<TestClass1>(), sb::di::CannotMoveOutServiceException);
}

TEST_F(ExternalServiceTest, ShouldCreateExternalNullService)
{
    TestClass1 test;
    const sb::di::details::services::ExternalService<TestClass1> service{nullptr};

    EXPECT_FALSE(service);
    EXPECT_FALSE(service.isValid());
    EXPECT_FALSE(service.get());
    EXPECT_EQ(service.getTypeId(), typeid(TestClass1));
}

TEST_F(ExternalServiceTest, ShouldFailMoveAsUniquePtrExternalNullService)
{
    TestClass1 test;
    sb::di::details::services::ExternalService<TestClass1> service{nullptr};

    EXPECT_FALSE(service);
    EXPECT_FALSE(service.isValid());
    EXPECT_THROW(service.moveOutAsUniquePtr<TestClass1>(), sb::di::CannotReleaseServiceException);
}

TEST_F(ExternalServiceTest, ShouldFailMoveOutExternalNullService)
{
    TestClass1 test;
    sb::di::details::services::ExternalService<TestClass1> service{nullptr};

    EXPECT_FALSE(service);
    EXPECT_FALSE(service.isValid());
    EXPECT_THROW(service.moveOutAs<TestClass1>(), sb::di::CannotMoveOutServiceException);
}
