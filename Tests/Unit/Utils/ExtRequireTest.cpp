#include <gtest/gtest.h>

#include "../../Helpers/Classes/Basic.hpp"
#include "SevenBit/DI/Details/Services/ExternalService.hpp"
#include "SevenBit/DI/Details/Services/InPlaceService.hpp"
#include "SevenBit/DI/Details/Services/UniquePtrService.hpp"
#include "SevenBit/DI/Details/Utils/ExtRequire.hpp"
#include "SevenBit/DI/Exceptions.hpp"

class ExtRequireTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ExtRequireTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ExtRequireTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ExtRequireTest, ShoulRequireExtValidInstance)
{
    TestClass1 test;
    EXPECT_THROW(sb::di::details::ExtRequire::validInstance(nullptr), sb::di::NullPointerException);
    EXPECT_THROW(sb::di::details::ExtRequire::validInstance(sb::di::ServiceInstance{}),
                 sb::di::InvalidServiceException);
    EXPECT_THROW(sb::di::details::ExtRequire::validInstance(
                     sb::di::ServiceInstance{std::make_unique<sb::di::details::ExternalService<TestClass1>>(nullptr)}),
                 sb::di::InvalidServiceException);
    EXPECT_NO_THROW(sb::di::details::ExtRequire::validInstance(
        sb::di::ServiceInstance{std::make_unique<sb::di::details::ExternalService<TestClass1>>(&test)}));
    EXPECT_THROW(sb::di::details::ExtRequire::validInstance(
                     sb::di::ServiceInstance{std::make_unique<sb::di::details::UniquePtrService<TestClass1>>(nullptr)}),
                 sb::di::InvalidServiceException);
    EXPECT_NO_THROW(sb::di::details::ExtRequire::validInstance(sb::di::ServiceInstance{
        std::make_unique<sb::di::details::UniquePtrService<TestClass1>>(std::make_unique<TestClass1>())}));
    EXPECT_NO_THROW(sb::di::details::ExtRequire::validInstance(
        sb::di::ServiceInstance{std::make_unique<sb::di::details::InPlaceService<TestClass1>>()}));
}
