#include <gtest/gtest.h>

#include "SevenBit/DI/Details/Utils/Check.hpp"

#include "../../Helpers/Classes/Basic.hpp"
#include "SevenBit/DI/Details/Services/ExternalService.hpp"
#include "SevenBit/DI/Details/Services/InPlaceService.hpp"
#include "SevenBit/DI/Details/Services/UniquePtrService.hpp"
#include "SevenBit/DI/Details/Utils/Require.hpp"

class RequireTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    RequireTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~RequireTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(RequireTest, ShouldRequireNotNull)
{
    TestClass1 test1;
    std::unique_ptr<TestClass1> testPtr{new TestClass1};
    std::shared_ptr<TestClass1> testSPtr{new TestClass1};
    EXPECT_NO_THROW(sb::di::details::utils::Require::notNull(&test1));
    EXPECT_NO_THROW(sb::di::details::utils::Require::notNullAndGet(&test1));
    EXPECT_NO_THROW(sb::di::details::utils::Require::notNull(testPtr));
    EXPECT_NO_THROW(sb::di::details::utils::Require::notNullAndGet(std::move(testPtr)));
    EXPECT_NO_THROW(sb::di::details::utils::Require::notNull(testSPtr));
    EXPECT_NO_THROW(sb::di::details::utils::Require::notNullAndGet(std::move(testSPtr)));
}

TEST_F(RequireTest, ShouldFailRequireNotNull)
{
    TestClass1 test1;
    std::unique_ptr<TestClass1> testPtr;
    std::unique_ptr<TestClass1> testSPtr;
    EXPECT_THROW(sb::di::details::utils::Require::notNull<TestClass1>(nullptr), sb::di::NullPointerException);
    EXPECT_THROW(sb::di::details::utils::Require::notNullAndGet<TestClass1>(nullptr), sb::di::NullPointerException);
    EXPECT_THROW(sb::di::details::utils::Require::notNull(testPtr), sb::di::NullPointerException);
    EXPECT_THROW(sb::di::details::utils::Require::notNullAndGet(std::move(testPtr)), sb::di::NullPointerException);
    EXPECT_THROW(sb::di::details::utils::Require::notNull(testSPtr), sb::di::NullPointerException);
    EXPECT_THROW(sb::di::details::utils::Require::notNullAndGet(std::move(testSPtr)), sb::di::NullPointerException);
}

TEST_F(RequireTest, ShoulRequireInstanceValidity)
{
    TestClass1 test;
    EXPECT_THROW(sb::di::details::utils::Require::validInstance(nullptr), sb::di::NullPointerException);
    EXPECT_THROW(sb::di::details::utils::Require::validInstance(
                     std::make_unique<sb::di::details::services::ExternalService<TestClass1>>(nullptr)),
                 sb::di::InvalidServiceException);
    EXPECT_NO_THROW(sb::di::details::utils::Require::validInstance(
        std::make_unique<sb::di::details::services::ExternalService<TestClass1>>(&test)));
    EXPECT_THROW(sb::di::details::utils::Require::validInstance(
                     std::make_unique<sb::di::details::services::UniquePtrService<TestClass1>>(nullptr)),
                 sb::di::InvalidServiceException);
    EXPECT_NO_THROW(sb::di::details::utils::Require::validInstance(
        std::make_unique<sb::di::details::services::UniquePtrService<TestClass1>>(std::make_unique<TestClass1>())));
    EXPECT_NO_THROW(sb::di::details::utils::Require::validInstance(
        std::make_unique<sb::di::details::services::InPlaceService<TestClass1>>()));
}
