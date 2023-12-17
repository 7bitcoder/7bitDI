#include <gtest/gtest.h>

#include "SevenBit/DI/ServiceLifeTime.hpp"

class ServiceLifeTime : public testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceLifeTime() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceLifeTime() override = default;

    static void TearDownTestSuite() {}
};

TEST_F(ServiceLifeTime, ShouldConstructProperLifetime)
{
    auto act = [] {
        sb::di::ServiceLifeTime sin{sb::di::ServiceLifeTime::Singleton};
        sb::di::ServiceLifeTime scope{sb::di::ServiceLifeTime::Scoped};
        sb::di::ServiceLifeTime transient{sb::di::ServiceLifeTime::Transient};
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceLifeTime, ShouldConstructProperLifetimeUsingFactories)
{
    auto act = [] {
        sb::di::ServiceLifeTime::singleton();
        sb::di::ServiceLifeTime::scoped();
        sb::di::ServiceLifeTime::transient();
    };

    EXPECT_NO_THROW(act());
}

TEST_F(ServiceLifeTime, ShouldCheckLifetime)
{
    const auto lifeTime = sb::di::ServiceLifeTime::singleton();

    EXPECT_TRUE(lifeTime.isSingleton());
    EXPECT_TRUE(lifeTime.is(sb::di::ServiceLifeTime::Singleton));
    EXPECT_FALSE(lifeTime.is(sb::di::ServiceLifeTime::Scoped));
    EXPECT_FALSE(lifeTime.is(sb::di::ServiceLifeTime::Transient));
    EXPECT_FALSE(lifeTime.is(static_cast<sb::di::ServiceLifeTime::Type>(12)));
    EXPECT_FALSE(lifeTime.isScoped());
    EXPECT_FALSE(lifeTime.isTransient());
}

TEST_F(ServiceLifeTime, ShouldCompare)
{
    EXPECT_EQ(sb::di::ServiceLifeTime::singleton(), sb::di::ServiceLifeTime::singleton());
    EXPECT_EQ(sb::di::ServiceLifeTime::scoped(), sb::di::ServiceLifeTime::scoped());
    EXPECT_EQ(sb::di::ServiceLifeTime::transient(), sb::di::ServiceLifeTime::transient());
    EXPECT_NE(sb::di::ServiceLifeTime::singleton(), sb::di::ServiceLifeTime::scoped());
    EXPECT_NE(sb::di::ServiceLifeTime::singleton(), sb::di::ServiceLifeTime::transient());
    EXPECT_NE(sb::di::ServiceLifeTime::scoped(), sb::di::ServiceLifeTime::transient());
}
