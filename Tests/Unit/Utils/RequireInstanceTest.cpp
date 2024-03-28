#include <gtest/gtest.h>

#include "../../Helpers/Classes/Basic.hpp"
#include "SevenBit/DI/Details/Services/ExternalService.hpp"
#include "SevenBit/DI/Details/Services/InPlaceService.hpp"
#include "SevenBit/DI/Details/Services/UniquePtrService.hpp"
#include "SevenBit/DI/Details/Utils/RequireInstance.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceDescriber.hpp"

class RequireInstanceTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    RequireInstanceTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~RequireInstanceTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(RequireInstanceTest, ShoulRequireValidInstance)
{
    TestClass1 test;
    EXPECT_THROW(sb::di::details::RequireInstance::valid(nullptr), sb::di::NullPointerException);
    EXPECT_THROW(sb::di::details::RequireInstance::valid(sb::di::ServiceInstance{}), sb::di::InvalidServiceException);
    EXPECT_THROW(sb::di::details::RequireInstance::valid(
                     sb::di::ServiceInstance{std::make_unique<sb::di::details::ExternalService<TestClass1>>(nullptr)}),
                 sb::di::InvalidServiceException);
    EXPECT_NO_THROW(sb::di::details::RequireInstance::valid(
        sb::di::ServiceInstance{std::make_unique<sb::di::details::ExternalService<TestClass1>>(&test)}));
    EXPECT_THROW(sb::di::details::RequireInstance::valid(
                     sb::di::ServiceInstance{std::make_unique<sb::di::details::UniquePtrService<TestClass1>>(nullptr)}),
                 sb::di::InvalidServiceException);
    EXPECT_NO_THROW(sb::di::details::RequireInstance::valid(sb::di::ServiceInstance{
        std::make_unique<sb::di::details::UniquePtrService<TestClass1>>(std::make_unique<TestClass1>())}));
    EXPECT_NO_THROW(sb::di::details::RequireInstance::valid(
        sb::di::ServiceInstance{std::make_unique<sb::di::details::InPlaceService<TestClass1>>()}));
}
