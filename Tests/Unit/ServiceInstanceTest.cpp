#include <gtest/gtest.h>

#include "../Helpers/Classes/Basic.hpp"
#include "SevenBit/DI/Details/Services/InPlaceService.hpp"
#include "SevenBit/DI/Exceptions.hpp"
#include "SevenBit/DI/ServiceInstance.hpp"

class ServiceInstanceTest : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceInstanceTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceInstanceTest() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ServiceInstanceTest, ShouldCreateEmptyInstance)
{
    sb::di::ServiceInstance instance;

    EXPECT_FALSE(instance);
    EXPECT_FALSE(instance.isValid());
    EXPECT_FALSE(instance.tryGetImplementation());
    EXPECT_THROW(instance.getImplementation(), sb::di::NullPointerException);
    EXPECT_EQ(instance.getCastOffset(), 0);
}

TEST_F(ServiceInstanceTest, ShouldCreateInstance)
{
    auto implementation = std::make_unique<sb::di::details::services::InPlaceService<TestClass1>>();
    sb::di::ServiceInstance instance{std::move(implementation), 12};

    EXPECT_TRUE(instance);
    EXPECT_TRUE(instance.isValid());
    EXPECT_TRUE(instance.tryGetImplementation());
    EXPECT_NO_THROW(instance.getImplementation());
    EXPECT_EQ(instance.getCastOffset(), 12);
    EXPECT_TRUE(instance.getAs<void>());
}

TEST_F(ServiceInstanceTest, ShouldAddCastOffset)
{
    auto implementation = std::make_unique<sb::di::details::services::InPlaceService<TestClass1>>();
    sb::di::ServiceInstance instance{std::move(implementation), 12};

    EXPECT_TRUE(instance);
    EXPECT_EQ(instance.getCastOffset(), 12);
    instance.addCastOffset(4);
    EXPECT_EQ(instance.getCastOffset(), 16);
}

TEST_F(ServiceInstanceTest, ShouldSetCastOffset)
{
    auto implementation = std::make_unique<sb::di::details::services::InPlaceService<TestClass1>>();
    sb::di::ServiceInstance instance{std::move(implementation), 12};

    EXPECT_TRUE(instance);
    EXPECT_EQ(instance.getCastOffset(), 12);
    instance.setCastOffset(4);
    EXPECT_EQ(instance.getCastOffset(), 4);
}

TEST_F(ServiceInstanceTest, ShouldClear)
{
    auto implementation = std::make_unique<sb::di::details::services::InPlaceService<TestClass1>>();
    sb::di::ServiceInstance instance{std::move(implementation), 12};

    EXPECT_TRUE(instance);
    instance.clear();
    EXPECT_FALSE(instance);
    EXPECT_EQ(instance.getCastOffset(), 0);
    EXPECT_FALSE(instance.tryGetImplementation());
}
