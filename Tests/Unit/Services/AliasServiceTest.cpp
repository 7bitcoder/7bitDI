#include <gtest/gtest.h>

#include "../../Helpers/Classes/Basic.hpp"
#include "SevenBit/DI/Details/Services/AliasService.hpp"
#include "SevenBit/DI/Exceptions.hpp"

class AliasServiceTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    AliasServiceTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~AliasServiceTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(AliasServiceTest, ShouldCreateAliasService)
{
    TestClass1 test;
    const sb::di::details::AliasService service{&test, typeid(TestClass1)};

    EXPECT_TRUE(service.isValid());
    EXPECT_EQ(service.get(), &test);
    EXPECT_EQ(service.getTypeId(), typeid(TestClass1));
}

TEST_F(AliasServiceTest, ShouldFailReleaseAliasService)
{
    TestClass1 test;
    sb::di::details::AliasService service{&test, typeid(TestClass1)};

    EXPECT_TRUE(service.isValid());
    EXPECT_THROW(service.release(), sb::di::CannotReleaseServiceException);
}

TEST_F(AliasServiceTest, ShouldFailMoveOutAliasService)
{
    TestClass1 test;
    sb::di::details::AliasService service{&test, typeid(TestClass1)};

    EXPECT_TRUE(service.isValid());
    EXPECT_THROW(service.getForMoveOut(), sb::di::CannotMoveOutServiceException);
}

TEST_F(AliasServiceTest, ShouldCreateAliasNullService)
{
    const sb::di::details::AliasService service{nullptr, typeid(TestClass1)};

    EXPECT_FALSE(service.isValid());
    EXPECT_FALSE(service.get());
    EXPECT_EQ(service.getTypeId(), typeid(TestClass1));
}

TEST_F(AliasServiceTest, ShouldFailReleaseAliasNullService)
{
    sb::di::details::AliasService service{nullptr, typeid(TestClass1)};

    EXPECT_FALSE(service.isValid());
    EXPECT_THROW(service.release(), sb::di::CannotReleaseServiceException);
}

TEST_F(AliasServiceTest, ShouldFailMoveOutAliasNullService)
{
    sb::di::details::AliasService service{nullptr, typeid(TestClass1)};

    EXPECT_FALSE(service.isValid());
    EXPECT_THROW(service.getForMoveOut(), sb::di::CannotMoveOutServiceException);
}
