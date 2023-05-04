#include <gtest/gtest.h>
#include <iostream>
#include <memory>

#include "Classes/BasicTest.hpp"
#include "SevenBit/Exceptions.hpp"
#include "SevenBit/_Internal/ServiceOwner.hpp"

class ServiceOwnerTest : public ::testing::Test
{
  protected:
    static void TearUpTestSuite() {}

    ServiceOwnerTest() {}

    void SetUp() override {}

    void TearDown() override {}

    ~ServiceOwnerTest() {}

    static void TearDownTestSuite() {}
};

TEST_F(ServiceOwnerTest, ShouldProperelyCreateExternalService)
{
    sb::internal::ServiceOwner<TestClass1> owner{std::make_unique<TestClass1>()};

    EXPECT_TRUE(owner);
    EXPECT_TRUE(owner.isValid());
    EXPECT_TRUE(owner.get());
    EXPECT_EQ(owner.getTypeId(), typeid(TestClass1));
}

TEST_F(ServiceOwnerTest, ShouldProperelyCreateExternalNullService)
{
    sb::internal::ServiceOwner<TestClass1> owner{nullptr};

    EXPECT_FALSE(owner);
    EXPECT_FALSE(owner.isValid());
    EXPECT_FALSE(owner.get());
    EXPECT_EQ(owner.getTypeId(), typeid(TestClass1));
}

TEST_F(ServiceOwnerTest, ShouldThrowOnMoveOut)
{
    sb::internal::ServiceOwner<TestClass1> owner{std::make_unique<TestClass1>()};

    auto ptr = owner.moveOut();
    EXPECT_TRUE(ptr);
    delete (TestClass1 *)ptr;
}