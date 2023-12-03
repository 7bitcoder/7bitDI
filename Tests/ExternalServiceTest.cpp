#include <gtest/gtest.h>
#include <iostream>

#include "Classes/BasicTest.hpp"
#include "SevenBit/DI/Details/ExternalService.hpp"
#include "SevenBit/DI/Exceptions.hpp"

class ExternalServiceTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ExternalServiceTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ExternalServiceTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(ExternalServiceTest, ShouldProperelyCreateExternalService)
{
    TestClass1 test;
    sb::di::details::ExternalService external{&test};

    EXPECT_TRUE(external);
    EXPECT_TRUE(external.isValid());
    EXPECT_EQ(external.get(), &test);
    EXPECT_EQ(external.getTypeId(), typeid(TestClass1));
}

TEST_F(ExternalServiceTest, ShouldProperelyCreateExternalNullService)
{
    TestClass1 test;
    sb::di::details::ExternalService<TestClass1> external{nullptr};

    EXPECT_FALSE(external);
    EXPECT_FALSE(external.isValid());
    EXPECT_NE(external.get(), &test);
    EXPECT_EQ(external.getTypeId(), typeid(TestClass1));
}

TEST_F(ExternalServiceTest, ShouldThrowOnMoveOut)
{
    TestClass1 test;
    sb::di::details::ExternalService external{&test};

    EXPECT_THROW(external.release(), sb::di::CannotMoveOutServiceException);
}
