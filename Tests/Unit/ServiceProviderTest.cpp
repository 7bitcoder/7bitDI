#include <gtest/gtest.h>

#include "SevenBit/DI/Details/Core/ServiceInstanceProviderRoot.hpp"
#include "SevenBit/DI/ServiceProvider.hpp"

class ServiceProviderTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceProviderTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceProviderTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ServiceProviderTest, ShouldCreateServiceProvider)
{
    sb::di::IServiceInstanceProvider::Ptr instanceProvider =
        std::make_unique<sb::di::details::ServiceInstanceProviderRoot>();

    const auto providerPtr = instanceProvider.get();
    sb::di::ServiceProvider provider{std::move(instanceProvider)};

    EXPECT_EQ(&provider.getInstanceProvider(), providerPtr);
}

TEST_F(ServiceProviderTest, ShouldFailCreateServiceProvider)
{
    EXPECT_THROW(sb::di::ServiceProvider{nullptr}, sb::di::NullPointerException);
}
